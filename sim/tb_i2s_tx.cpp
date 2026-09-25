// Unit test for i2s_tx: drives samples at the S-DSP cadence (one per 768
// clocks), decodes the pins with a Philips-I2S receiver model and checks
// bit-exact data, clock ratios, edge alignment and latency.
#include <cstdint>
#include <cstdio>
#include <deque>
#include <random>
#include <vector>

#include "Vi2s_tx.h"
#include "verilated.h"

static constexpr int CLKS_PER_FRAME = 768;
static constexpr int CLKS_PER_BCLK = 12;
static constexpr int FRAMES = 2000;

struct Frame {
    std::uint16_t l, r;
    std::uint64_t t;
};

class I2sReceiver {
public:
    std::vector<Frame> frames;

    void rising_edge(bool lr, bool sd, std::uint64_t t) {
        if (lr != last_lr_) {
            // The bit clocked with the LRCLK change still belongs to the
            // previous slot; the channel's MSB follows on the next edge.
            last_lr_ = lr;
            pos_ = -1;
            return;
        }
        ++pos_;
        if (pos_ >= 0 && pos_ < 16) {
            word_ = static_cast<std::uint16_t>((word_ << 1) | sd);
            if (pos_ == 15) {
                if (!lr) {
                    left_ = word_;
                    have_left_ = true;
                } else if (have_left_) {
                    frames.push_back({left_, word_, t});
                    have_left_ = false;
                }
            }
        } else if (sd) {
            ++padding_errors;
        }
    }

    int padding_errors = 0;

private:
    bool last_lr_ = true;
    int pos_ = 99;
    std::uint16_t word_ = 0, left_ = 0;
    bool have_left_ = false;
};

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto* dut = new Vi2s_tx;

    std::mt19937 rng(0x5eed);
    std::vector<Frame> sent;
    const std::uint16_t corners[] = {0x8000, 0x7fff, 0xffff, 0x0001, 0xaaaa, 0x5555, 0x8001, 0x0000};
    for (int i = 0; i < FRAMES; ++i) {
        std::uint16_t l, r;
        if (i < 8) {
            l = corners[i];
            r = corners[7 - i];
        } else {
            l = static_cast<std::uint16_t>(rng());
            r = static_cast<std::uint16_t>(rng());
        }
        sent.push_back({l, r, 0});
    }

    I2sReceiver rx;
    int errors = 0;
    const int phase = 337; // Arbitrary DSP phase relative to the I2S frame.

    std::uint64_t t = 0;
    bool prev_bclk = false, prev_lr = true, prev_sd = false;
    std::uint64_t last_bclk_rise = 0, last_lr_rise = 0;
    int bclk_rises = 0, lr_rises = 0;
    int next_sample = 0;

    dut->resetn = 0;
    dut->clk = 0;
    dut->eval();
    for (int i = 0; i < 4; ++i) {
        dut->clk = 1; dut->eval();
        dut->clk = 0; dut->eval();
    }
    dut->resetn = 1;

    const std::uint64_t total = static_cast<std::uint64_t>(FRAMES + 4) * CLKS_PER_FRAME;
    for (; t < total; ++t) {
        const bool fire = (t % CLKS_PER_FRAME) == phase && next_sample < FRAMES;
        dut->sample_valid = fire;
        if (fire) {
            dut->sample_l = sent[next_sample].l;
            dut->sample_r = sent[next_sample].r;
            sent[next_sample].t = t;
            ++next_sample;
        }
        dut->clk = 1; dut->eval();
        dut->clk = 0; dut->eval();

        const bool bclk = dut->bclk, lr = dut->lrclk, sd = dut->sdata;
        const bool bclk_fell = prev_bclk && !bclk;
        if ((lr != prev_lr || sd != prev_sd) && !bclk_fell) {
            if (errors++ < 10)
                std::printf("FAIL t=%llu: LRCLK/SDATA changed outside a BCLK falling edge\n",
                            static_cast<unsigned long long>(t));
        }
        if (!prev_bclk && bclk) {
            if (bclk_rises++ > 0 && t - last_bclk_rise != CLKS_PER_BCLK && errors++ < 10)
                std::printf("FAIL t=%llu: BCLK period %llu\n", static_cast<unsigned long long>(t),
                            static_cast<unsigned long long>(t - last_bclk_rise));
            last_bclk_rise = t;
            rx.rising_edge(lr, sd, t);
        }
        if (!prev_lr && lr) {
            if (lr_rises++ > 0 && t - last_lr_rise != CLKS_PER_FRAME && errors++ < 10)
                std::printf("FAIL t=%llu: LRCLK period %llu\n", static_cast<unsigned long long>(t),
                            static_cast<unsigned long long>(t - last_lr_rise));
            last_lr_rise = t;
        }
        prev_bclk = bclk;
        prev_lr = lr;
        prev_sd = sd;
    }

    // Frames decoded before the first sample arrived carry the reset value.
    std::size_t k = 0;
    while (k < rx.frames.size() && rx.frames[k].l == 0 && rx.frames[k].r == 0)
        ++k;

    std::size_t matched = 0;
    std::uint64_t max_latency = 0;
    for (std::size_t i = 0; i < sent.size() && k + i < rx.frames.size(); ++i) {
        const Frame& got = rx.frames[k + i];
        if (got.l != sent[i].l || got.r != sent[i].r) {
            if (errors++ < 10)
                std::printf("FAIL frame %zu: sent %04x/%04x got %04x/%04x\n", i,
                            sent[i].l, sent[i].r, got.l, got.r);
            continue;
        }
        ++matched;
        if (got.t - sent[i].t > max_latency)
            max_latency = got.t - sent[i].t;
    }
    if (matched < sent.size() - 2) {
        std::printf("FAIL: only %zu of %zu frames decoded\n", matched, sent.size());
        ++errors;
    }
    if (max_latency > 2 * CLKS_PER_FRAME) {
        std::printf("FAIL: latency %llu clocks exceeds two frames\n",
                    static_cast<unsigned long long>(max_latency));
        ++errors;
    }
    if (rx.padding_errors) {
        std::printf("FAIL: %d non-zero padding bits\n", rx.padding_errors);
        ++errors;
    }

    std::printf("%s: %zu/%zu frames bit-exact, BCLK=%d clk, LRCLK=%d clk, max latency %llu clk (%.1f us @ 24.528 MHz)\n",
                errors ? "FAILED" : "PASSED", matched, sent.size(), CLKS_PER_BCLK, CLKS_PER_FRAME,
                static_cast<unsigned long long>(max_latency), max_latency / 24.528);
    delete dut;
    return errors ? 1 : 0;
}
