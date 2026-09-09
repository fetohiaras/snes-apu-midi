#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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

static void usage(const char* program) {
    std::fprintf(stderr,
        "Usage: %s [--seconds N] [--output FILE]\n"
        "  --seconds N   PCM duration to capture after the APU starts (default: %u)\n"
        "  --output FILE destination WAV (default: primer25k_nanospc_smoke.wav)\n",
        program, DEFAULT_SECONDS);
}

int main(int argc, char** argv) {
    std::uint32_t seconds = DEFAULT_SECONDS;
    const char* output_path = "primer25k_nanospc_smoke.wav";

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--seconds") == 0 && i + 1 < argc) {
            seconds = static_cast<std::uint32_t>(std::strtoul(argv[++i], nullptr, 10));
        } else if (std::strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_path = argv[++i];
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

    // Only dclk matters in the Verilator configuration: the hardware PLL is
    // replaced with clk_50m.  Each loop is one rising dclk edge.
    top->clk_50m = 0;
    top->eval();
    while (!Verilated::gotFinish() && frames_written < frames_requested) {
        top->clk_50m = 1;
        top->eval();
        ++sim_time;

        if (top->sim_snd_rdy) {
            const std::int16_t left = static_cast<std::int16_t>(top->sim_audio_l);
            const std::int16_t right = static_cast<std::int16_t>(top->sim_audio_r);
            write_u16_le(wav, static_cast<std::uint16_t>(left));
            write_u16_le(wav, static_cast<std::uint16_t>(right));
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
    return frames_written == frames_requested ? 0 : 1;
}
