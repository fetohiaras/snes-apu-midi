// Full-chain integration run of primer25k_nanospc_top:
//   midi_rx pin -> UART -> MIDI parser -> mailbox -> SPC700 driver -> S-DSP
//   -> I2S pins, plus the status LED.
//
// Usage: Vprimer25k_nanospc_top UART.txt FRAMES OUTDIR
//   UART.txt lines: "<clock> <byte>" -- start-bit time of each byte in clocks
//   since the APU started running (31250 baud, 8N1).
// Writes OUTDIR/top_midi.wav (parallel APU output), OUTDIR/top_midi_i2s.wav
// (every frame decoded from the I2S pins), OUTDIR/top_events.txt (parser
// events as "<clock> <status> <d1> <d2>", replayable by tb_apu_midi) and
// OUTDIR/top_summary.txt. check_top_midi.py evaluates them.
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "Vprimer25k_nanospc_top.h"
#include "verilated.h"

static constexpr double CLK_HZ = 24528000.0;
static constexpr double BAUD = 31250.0;

struct Frame {
    std::uint16_t l, r;
};

static void write_wav(const std::string& path, const std::vector<Frame>& frames) {
    std::FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) {
        std::perror(path.c_str());
        std::exit(1);
    }
    auto p16 = [&](std::uint16_t v) { std::fputc(v & 0xff, f); std::fputc(v >> 8, f); };
    auto p32 = [&](std::uint32_t v) { p16(static_cast<std::uint16_t>(v)); p16(static_cast<std::uint16_t>(v >> 16)); };
    const std::uint32_t n = static_cast<std::uint32_t>(frames.size());
    std::fwrite("RIFF", 1, 4, f); p32(36 + n * 4);
    std::fwrite("WAVEfmt ", 1, 8, f); p32(16); p16(1); p16(2); p32(31938); p32(31938 * 4); p16(4); p16(16);
    std::fwrite("data", 1, 4, f); p32(n * 4);
    for (const Frame& fr : frames) { p16(fr.l); p16(fr.r); }
    std::fclose(f);
}

class I2sReceiver {
public:
    std::vector<Frame> frames;
    void rising_edge(bool lr, bool sd) {
        if (lr != last_lr_) { last_lr_ = lr; pos_ = -1; return; }
        if (++pos_ >= 16) return;
        word_ = static_cast<std::uint16_t>((word_ << 1) | sd);
        if (pos_ != 15) return;
        if (!lr) { left_ = word_; have_left_ = true; }
        else if (have_left_) { frames.push_back({left_, word_}); have_left_ = false; }
    }
private:
    bool last_lr_ = true, have_left_ = false;
    int pos_ = 99;
    std::uint16_t word_ = 0, left_ = 0;
};

struct Edges {
    std::uint64_t rises = 0, toggles = 0, last_rise = 0, min_period = ~0ull, max_period = 0;
    bool prev = false, init = false;
    void sample(bool v, std::uint64_t t) {
        if (!init) { prev = v; init = true; return; }
        if (v != prev) ++toggles;
        if (v && !prev) {
            if (rises++ > 0) {
                const std::uint64_t p = t - last_rise;
                if (p < min_period) min_period = p;
                if (p > max_period) max_period = p;
            }
            last_rise = t;
        }
        prev = v;
    }
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    if (argc < 4) {
        std::fprintf(stderr, "usage: %s UART.txt FRAMES OUTDIR\n", argv[0]);
        return 2;
    }
    std::vector<std::pair<std::uint64_t, int>> bytes;
    if (std::FILE* s = std::fopen(argv[1], "r")) {
        unsigned long long c;
        int b;
        while (std::fscanf(s, "%llu %d", &c, &b) == 2) bytes.push_back({c, b});
        std::fclose(s);
    } else {
        std::perror(argv[1]);
        return 1;
    }
    const std::uint32_t total = static_cast<std::uint32_t>(std::strtoul(argv[2], nullptr, 10));
    const std::string out = argv[3];

    auto* top = new Vprimer25k_nanospc_top;
    top->midi_rx = 1;
    top->clk_50m = 0;
    top->eval();

    std::vector<Frame> parallel;
    I2sReceiver i2s;
    std::size_t i2s_before_audio = 0;
    Edges bclk, lrclk, sdata, led, rx;
    std::uint64_t lr_sd_violations = 0;
    std::vector<std::uint64_t> led_toggles;
    int led_at_start = -1;
    bool prev_bclk = false, prev_lr = true, prev_sd = false, prev_led = false;

    std::FILE* evlog = std::fopen((out + "/top_events.txt").c_str(), "w");
    std::uint64_t t = 0, run_clk = 0;
    std::size_t next_byte = 0;
    const double cpb = CLK_HZ / BAUD;

    while (parallel.size() < total) {
        // Serial line: each byte is start bit, 8 data bits LSB first, stop bit.
        if (top->sim_running && next_byte < bytes.size()) {
            const double rel = static_cast<double>(run_clk) - static_cast<double>(bytes[next_byte].first);
            if (rel >= 0) {
                const int bit = static_cast<int>(rel / cpb);
                if (bit == 0) top->midi_rx = 0;
                else if (bit <= 8) top->midi_rx = (bytes[next_byte].second >> (bit - 1)) & 1;
                else top->midi_rx = 1;
                if (bit >= 10) ++next_byte;
            }
        }

        top->clk_50m = 1;
        top->eval();
        top->clk_50m = 0;
        top->eval();
        ++t;
        if (top->sim_running) ++run_clk;

        if (top->sim_ev_valid)
            std::fprintf(evlog, "%llu %u %u %u\n", static_cast<unsigned long long>(run_clk),
                         top->sim_ev_status, top->sim_ev_data1, top->sim_ev_data2);

        const bool b = top->i2s_bclk, lr = top->i2s_lrclk, sd = top->i2s_sdata, l = top->led_ready;
        if ((lr != prev_lr || sd != prev_sd) && !(prev_bclk && !b)) ++lr_sd_violations;
        if (b && !prev_bclk) i2s.rising_edge(lr, sd);
        // Pin timing is measured once the APU runs (before that the DAC
        // clocks are held by the ~10.5 ms power-on reset).
        if (top->sim_running) {
            if (led_at_start < 0) led_at_start = l;
            bclk.sample(b, t);
            lrclk.sample(lr, t);
            sdata.sample(sd, t);
            if (led.init && l != prev_led) led_toggles.push_back(run_clk);
            led.sample(l, t);
        }
        rx.sample(top->midi_rx, t);
        prev_bclk = b; prev_lr = lr; prev_sd = sd; prev_led = l;

        if (top->sim_running && top->sim_snd_rdy) {
            if (parallel.empty()) i2s_before_audio = i2s.frames.size();
            parallel.push_back({top->sim_audio_l, top->sim_audio_r});
        }
    }
    // Let the I2S path flush the last frames.
    for (int i = 0; i < 3 * 768; ++i) {
        top->clk_50m = 1; top->eval(); top->clk_50m = 0; top->eval();
        const bool b = top->i2s_bclk;
        if (b && !prev_bclk) i2s.rising_edge(top->i2s_lrclk, top->i2s_sdata);
        prev_bclk = b;
    }
    std::fclose(evlog);

    write_wav(out + "/top_midi.wav", parallel);
    write_wav(out + "/top_midi_i2s.wav", i2s.frames);

    std::FILE* s = std::fopen((out + "/top_summary.txt").c_str(), "w");
    std::fprintf(s,
                 "frames %zu\ni2s_frames %zu\ni2s_before_audio %zu\nbytes_sent %zu\n"
                 "sent_count %u\ndropped %u\nack_timeout %u\nbusy_at_end %u\n"
                 "bclk_rises %llu\nbclk_min_period %llu\nbclk_max_period %llu\n"
                 "lrclk_rises %llu\nlrclk_min_period %llu\nlrclk_max_period %llu\n"
                 "lr_sd_off_edge_changes %llu\nsdata_toggles %llu\nled_toggles %llu\n"
                 "led_level_at_start %u\nmidi_rx_toggles %llu\n",
                 parallel.size(), i2s.frames.size(), i2s_before_audio, next_byte,
                 top->sim_mb_sent, top->sim_mb_dropped, top->sim_mb_timeout, top->sim_mb_busy,
                 (unsigned long long)bclk.rises, (unsigned long long)bclk.min_period, (unsigned long long)bclk.max_period,
                 (unsigned long long)lrclk.rises, (unsigned long long)lrclk.min_period, (unsigned long long)lrclk.max_period,
                 (unsigned long long)lr_sd_violations, (unsigned long long)sdata.toggles, (unsigned long long)led.toggles,
                 static_cast<unsigned>(led_at_start),
                 (unsigned long long)rx.toggles);
    std::FILE* lt = std::fopen((out + "/top_led.txt").c_str(), "w");
    for (std::uint64_t c : led_toggles) std::fprintf(lt, "%llu\n", (unsigned long long)c);
    std::fclose(lt);
    std::fclose(s);
    std::printf("full chain: %zu frames, %zu UART bytes sent, mailbox sent %u\n",
                parallel.size(), next_byte, top->sim_mb_sent);
    delete top;
    return 0;
}
