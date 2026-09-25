#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "Vprimer25k_nanospc_top.h"
#include "verilated.h"

// The generated PLL output is 24.528 MHz.  NanoSPC produces one stereo frame
// for every 768 DSP clocks, hence 31,937.5 frames/s.  WAV stores an integral
// sample rate, so 31,938 Hz has an insignificant 0.0016% rate error.
static constexpr std::uint32_t WAV_SAMPLE_RATE = 31938;
static constexpr std::uint32_t DEFAULT_SECONDS = 3;

static void write_u16_le(std::FILE* f, std::uint16_t v) {
    std::fputc(v & 0xff, f);
    std::fputc((v >> 8) & 0xff, f);
}

static void write_u32_le(std::FILE* f, std::uint32_t v) {
    write_u16_le(f, static_cast<std::uint16_t>(v));
    write_u16_le(f, static_cast<std::uint16_t>(v >> 16));
}

static void write_wav_header(std::FILE* f, std::uint32_t frames) {
    constexpr std::uint16_t channels = 2;
    constexpr std::uint16_t bits_per_sample = 16;
    constexpr std::uint16_t block_align = channels * bits_per_sample / 8;
    const std::uint32_t data_bytes = frames * block_align;

    std::fwrite("RIFF", 1, 4, f);
    write_u32_le(f, 36 + data_bytes);
    std::fwrite("WAVEfmt ", 1, 8, f);
    write_u32_le(f, 16);
    write_u16_le(f, 1); // PCM
    write_u16_le(f, channels);
    write_u32_le(f, WAV_SAMPLE_RATE);
    write_u32_le(f, WAV_SAMPLE_RATE * block_align);
    write_u16_le(f, block_align);
    write_u16_le(f, bits_per_sample);
    std::fwrite("data", 1, 4, f);
    write_u32_le(f, data_bytes);
}

struct StereoFrame {
    std::uint16_t l, r;
};

// Philips-I2S receiver model, sampled on BCLK rising edges like the PCM5102A.
class I2sReceiver {
public:
    std::vector<StereoFrame> frames;

    void rising_edge(bool lr, bool sd) {
        if (lr != last_lr_) {
            last_lr_ = lr;
            pos_ = -1;
            return;
        }
        if (++pos_ >= 16)
            return;
        word_ = static_cast<std::uint16_t>((word_ << 1) | sd);
        if (pos_ != 15)
            return;
        if (!lr) {
            left_ = word_;
            have_left_ = true;
        } else if (have_left_) {
            frames.push_back({left_, word_});
            have_left_ = false;
        }
    }

private:
    bool last_lr_ = true;
    int pos_ = 99;
    std::uint16_t word_ = 0, left_ = 0;
    bool have_left_ = false;
};

static bool write_wav(const char* path, const std::vector<StereoFrame>& frames) {
    std::FILE* f = std::fopen(path, "wb");
    if (f == nullptr) {
        std::perror(path);
        return false;
    }
    write_wav_header(f, static_cast<std::uint32_t>(frames.size()));
    for (const StereoFrame& fr : frames) {
        write_u16_le(f, fr.l);
        write_u16_le(f, fr.r);
    }
    std::fclose(f);
    return true;
}

static void usage(const char* program) {
    std::fprintf(stderr,
        "Usage: %s [--seconds N] [--output FILE] [--i2s-output FILE]\n"
        "  --seconds N        PCM duration to capture after the APU starts (default: %u)\n"
        "  --output FILE      destination WAV (default: primer25k_nanospc_smoke.wav)\n"
        "  --i2s-output FILE  also write the WAV decoded from the I2S pins\n",
        program, DEFAULT_SECONDS);
}

int main(int argc, char** argv) {
    std::uint32_t seconds = DEFAULT_SECONDS;
    const char* output_path = "primer25k_nanospc_smoke.wav";
    const char* i2s_output_path = nullptr;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            seconds = static_cast<std::uint32_t>(std::strtoul(argv[++i], nullptr, 10));
        } else if (std::strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_path = argv[++i];
        } else if (std::strcmp(argv[i], "--i2s-output") == 0 && i + 1 < argc) {
            i2s_output_path = argv[++i];
        } else if (std::strcmp(argv[i], "--help") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            usage(argv[0]);
            return 2;
        }
    }

    if (seconds == 0 || seconds > 60) {
        std::fprintf(stderr, "--seconds must be between 1 and 60\n");
        return 2;
    }

    const std::uint32_t frames_requested = seconds * WAV_SAMPLE_RATE;
    std::FILE* wav = std::fopen(output_path, "wb");
    if (wav == nullptr) {
        std::perror(output_path);
        return 1;
    }
    write_wav_header(wav, 0); // Back-patched once capture is complete.

    Verilated::commandArgs(argc, argv);
    auto* top = new Vprimer25k_nanospc_top;
    vluint64_t sim_time = 0;
    std::uint32_t frames_written = 0;

    std::vector<StereoFrame> parallel;
    I2sReceiver i2s;
    std::size_t i2s_frames_before_audio = 0;
    bool prev_bclk = false;

    // Only dclk matters in the Verilator configuration: the hardware PLL is
    // replaced with clk_50m.  Each loop is one rising dclk edge.  Run two
    // extra frames so the I2S path can flush the final captured samples.
    const std::uint64_t flush_clocks = 2 * 768;
    std::uint64_t flush_left = flush_clocks;
    top->midi_rx = 1;
    top->clk_50m = 0;
    top->eval();
    while (!Verilated::gotFinish() && flush_left > 0) {
        top->clk_50m = 1;
        top->eval();
        ++sim_time;

        if (top->i2s_bclk && !prev_bclk)
            i2s.rising_edge(top->i2s_lrclk, top->i2s_sdata);
        prev_bclk = top->i2s_bclk;

        if (frames_written >= frames_requested) {
            --flush_left;
        } else if (top->sim_snd_rdy) {
            if (frames_written == 0)
                i2s_frames_before_audio = i2s.frames.size();
            const std::uint16_t left = top->sim_audio_l;
            const std::uint16_t right = top->sim_audio_r;
            write_u16_le(wav, left);
            write_u16_le(wav, right);
            parallel.push_back({left, right});
            ++frames_written;
        }

        top->clk_50m = 0;
        top->eval();
        ++sim_time;
    }

    std::fseek(wav, 0, SEEK_SET);
    write_wav_header(wav, frames_written);
    std::fclose(wav);
    delete top;

    std::printf("Wrote %u stereo frames (%.3f s at %u Hz) to %s\n",
                frames_written,
                static_cast<double>(frames_written) / WAV_SAMPLE_RATE,
                WAV_SAMPLE_RATE, output_path);

    // The I2S stream must equal the parallel stream after a fixed pipeline
    // delay of a frame or two; try the small delays and require an exact match.
    int delay_found = -1;
    for (int delay = 0; delay <= 2 && delay_found < 0; ++delay) {
        const std::size_t base = i2s_frames_before_audio + delay;
        if (base + parallel.size() > i2s.frames.size())
            continue;
        bool same = true;
        for (std::size_t i = 0; i < parallel.size() && same; ++i)
            same = i2s.frames[base + i].l == parallel[i].l && i2s.frames[base + i].r == parallel[i].r;
        if (same)
            delay_found = delay;
    }
    if (delay_found >= 0) {
        std::printf("I2S check PASSED: %zu frames bit-exact vs parallel PCM (pipeline delay %d frame(s))\n",
                    parallel.size(), delay_found);
    } else {
        std::printf("I2S check FAILED: decoded %zu I2S frames, none align with the %zu parallel frames\n",
                    i2s.frames.size(), parallel.size());
    }

    if (i2s_output_path != nullptr) {
        const std::size_t start = i2s_frames_before_audio + (delay_found > 0 ? delay_found : 0);
        std::vector<StereoFrame> decoded;
        if (start < i2s.frames.size())
            decoded.assign(i2s.frames.begin() + static_cast<std::ptrdiff_t>(start), i2s.frames.end());
        if (write_wav(i2s_output_path, decoded))
            std::printf("Wrote %zu I2S-decoded frames to %s\n", decoded.size(), i2s_output_path);
    }

    return (frames_written == frames_requested && delay_found >= 0) ? 0 : 1;
}
