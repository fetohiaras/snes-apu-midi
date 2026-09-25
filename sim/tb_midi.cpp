// Tests for uart_rx + midi_parser.
//  1. Directed byte streams with hand-written expected events, sent over the
//     serial line at nominal and +/-2% baud with random idle gaps.
//  2. Framing-error, glitch and recovery checks on the UART.
//  3. Random byte streams fed straight into a parser and compared with an
//     independent C++ reference model.
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

#include "Vtb_midi_top.h"
#include "verilated.h"

static constexpr double CLK_HZ = 24528000.0;
static constexpr double BAUD = 31250.0;

struct Ev {
    std::uint8_t s, d1, d2;
    bool operator==(const Ev& o) const { return s == o.s && d1 == o.d1 && d2 == o.d2; }
};

class RefParser {
public:
    void feed(std::uint8_t b, std::vector<Ev>& out) {
        if (b >= 0xF8)
            return;
        if (b & 0x80) {
            got_ = 0;
            if (b < 0xF0) {
                run_ = b;
                skip_ = 0;
            } else {
                run_ = 0;
                skip_ = (b == 0xF1 || b == 0xF3) ? 1 : (b == 0xF2) ? 2 : 0;
            }
            return;
        }
        if (skip_) {
            --skip_;
            return;
        }
        if (!run_)
            return;
        const int type = run_ & 0xF0;
        const int need = (type == 0xC0 || type == 0xD0) ? 1 : 2;
        buf_[got_++] = b;
        if (got_ < need)
            return;
        got_ = 0;
        Ev e{run_, buf_[0], static_cast<std::uint8_t>(need == 2 ? buf_[1] : 0)};
        if (type == 0x90 && e.d2 == 0)
            e.s = static_cast<std::uint8_t>(0x80 | (run_ & 0x0F));
        out.push_back(e);
    }

private:
    std::uint8_t run_ = 0, buf_[2] = {0, 0};
    int got_ = 0, skip_ = 0;
};

class Bench {
public:
    Vtb_midi_top* dut = new Vtb_midi_top;
    std::vector<std::uint8_t> uart_bytes;
    std::vector<Ev> ser_evs, dir_evs;
    int ferr_count = 0;

    Bench() {
        dut->rx = 1;
        dut->resetn = 0;
        for (int i = 0; i < 8; ++i) tick();
        dut->resetn = 1;
    }
    ~Bench() { delete dut; }

    void tick() {
        dut->clk = 1; dut->eval();
        dut->clk = 0; dut->eval();
        if (dut->uart_valid) uart_bytes.push_back(dut->uart_data);
        if (dut->uart_ferr) ++ferr_count;
        if (dut->ser_ev_valid) ser_evs.push_back({dut->ser_ev_status, dut->ser_ev_data1, dut->ser_ev_data2});
        if (dut->dir_ev_valid) dir_evs.push_back({dut->dir_ev_status, dut->dir_ev_data1, dut->dir_ev_data2});
    }

    // Drive one 8N1 frame; stop_bit=false produces a framing error.
    void send_serial(std::uint8_t b, double baud, bool stop_bit = true) {
        const double clks_per_bit = CLK_HZ / baud;
        for (int bit = 0; bit < 10; ++bit) {
            int level = bit == 0 ? 0 : bit == 9 ? (stop_bit ? 1 : 0) : (b >> (bit - 1)) & 1;
            frac_ += clks_per_bit;
            const int n = static_cast<int>(frac_);
            frac_ -= n;
            dut->rx = level;
            for (int i = 0; i < n; ++i) tick();
        }
        dut->rx = 1;
    }

    void idle(int clocks) {
        dut->rx = 1;
        for (int i = 0; i < clocks; ++i) tick();
    }

    void send_direct(std::uint8_t b) {
        dut->byte_valid = 1;
        dut->byte_in = b;
        tick();
        dut->byte_valid = 0;
    }

private:
    double frac_ = 0.0;
};

static int errors = 0;

static void check(bool ok, const char* what) {
    if (!ok) {
        ++errors;
        std::printf("FAIL: %s\n", what);
    }
}

static bool same_events(const std::vector<Ev>& got, const std::vector<Ev>& want, const char* label) {
    bool ok = got.size() == want.size();
    for (std::size_t i = 0; ok && i < got.size(); ++i) ok = got[i] == want[i];
    if (!ok) {
        std::printf("FAIL %s: got %zu events, want %zu\n", label, got.size(), want.size());
        const std::size_t n = got.size() > want.size() ? got.size() : want.size();
        for (std::size_t i = 0; i < n && i < 20; ++i) {
            std::printf("  [%zu] got ", i);
            if (i < got.size()) std::printf("%02x %02x %02x", got[i].s, got[i].d1, got[i].d2);
            else std::printf("--------");
            std::printf("  want ");
            if (i < want.size()) std::printf("%02x %02x %02x", want[i].s, want[i].d1, want[i].d2);
            std::printf("\n");
        }
        ++errors;
    }
    return ok;
}

struct Case {
    const char* name;
    std::vector<std::uint8_t> bytes;
    std::vector<Ev> want;
};

static std::vector<Case> directed_cases() {
    return {
        {"stray data before any status", {0x12, 0x34}, {}},
        {"note on", {0x90, 0x3C, 0x64}, {{0x90, 0x3C, 0x64}}},
        {"running status, note on v=0 -> note off", {0x90, 0x3E, 0x50, 0x3C, 0x00},
         {{0x90, 0x3E, 0x50}, {0x80, 0x3C, 0x00}}},
        {"real-time bytes inside a message", {0x90, 0xF8, 0x40, 0xFE, 0x7F}, {{0x90, 0x40, 0x7F}}},
        {"program change with running status", {0xC0, 0x05, 0x06}, {{0xC0, 0x05, 0x00}, {0xC0, 0x06, 0x00}}},
        {"pitch bend centre", {0xE0, 0x00, 0x40}, {{0xE0, 0x00, 0x40}}},
        {"CC 7/10/64 on channel 3", {0xB2, 0x07, 0x64, 0x0A, 0x20, 0x40, 0x7F},
         {{0xB2, 0x07, 0x64}, {0xB2, 0x0A, 0x20}, {0xB2, 0x40, 0x7F}}},
        {"channel pressure", {0xD0, 0x50}, {{0xD0, 0x50, 0x00}}},
        {"SysEx cancels running status", {0x90, 0x3C, 0x40, 0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7, 0x3C, 0x40},
         {{0x90, 0x3C, 0x40}}},
        {"MTC quarter frame swallows its data byte", {0xF1, 0x3C, 0x3D, 0x40}, {}},
        {"song position, tune request, stray EOX", {0xF2, 0x01, 0x02, 0xF6, 0x10, 0xF7, 0x3C}, {}},
        {"status interrupts a partial message", {0x90, 0x3C, 0xB0, 0x07, 0x10}, {{0xB0, 0x07, 0x10}}},
        {"explicit note off keeps velocity", {0x8F, 0x3C, 0x40}, {{0x8F, 0x3C, 0x40}}},
        {"poly aftertouch", {0xA1, 0x3C, 0x22}, {{0xA1, 0x3C, 0x22}}},
    };
}

static void test_serial(double baud, const char* label, std::mt19937& rng) {
    Bench b;
    std::vector<std::uint8_t> all;
    std::vector<Ev> want;
    for (const Case& c : directed_cases()) {
        for (std::uint8_t x : c.bytes) {
            b.send_serial(x, baud);
            b.idle(static_cast<int>(rng() % 3) * 785);
            all.push_back(x);
        }
        want.insert(want.end(), c.want.begin(), c.want.end());
    }
    b.idle(2000);
    char name[96];
    std::snprintf(name, sizeof name, "serial bytes @ %s", label);
    check(b.uart_bytes == all, name);
    check(b.ferr_count == 0, "unexpected framing error");
    std::snprintf(name, sizeof name, "serial events @ %s", label);
    same_events(b.ser_evs, want, name);
}

static void test_directed_direct() {
    for (const Case& c : directed_cases()) {
        Bench b;
        for (std::uint8_t x : c.bytes) {
            b.send_direct(x);
            b.tick();
        }
        b.tick();
        same_events(b.dir_evs, c.want, c.name);
    }
}

static void test_uart_faults() {
    Bench b;
    b.send_serial(0x55, BAUD, false);  // stop bit low
    b.idle(3 * 785);
    check(b.ferr_count == 1, "framing error flagged once");
    check(b.uart_bytes.empty(), "no byte from a bad frame");

    b.dut->rx = 0;                      // 100-clock glitch, well under half a bit
    for (int i = 0; i < 100; ++i) b.tick();
    b.idle(3 * 785);
    check(b.uart_bytes.empty(), "glitch ignored");

    b.dut->rx = 0;                      // line held low (break), then released
    for (int i = 0; i < 20 * 785; ++i) b.tick();
    b.idle(2 * 785);
    const std::size_t after_break = b.uart_bytes.size();
    check(after_break <= 1, "break yields at most one byte");

    b.send_serial(0x90, BAUD);
    b.send_serial(0x3C, BAUD);
    b.send_serial(0x64, BAUD);
    b.idle(2000);
    check(b.uart_bytes.size() == after_break + 3 && b.uart_bytes.back() == 0x64, "recovery after faults");
    check(!b.ser_evs.empty() && b.ser_evs.back() == Ev{0x90, 0x3C, 0x64}, "note decoded after faults");
}

static void test_random(std::mt19937& rng) {
    Bench b;
    RefParser ref;
    std::vector<Ev> want;
    for (int i = 0; i < 300000; ++i) {
        const unsigned r = rng() % 100;
        std::uint8_t x;
        if (r < 30) x = static_cast<std::uint8_t>(0x80 + rng() % 0x70);
        else if (r < 35) x = static_cast<std::uint8_t>(0xF0 + rng() % 8);
        else if (r < 45) x = static_cast<std::uint8_t>(0xF8 + rng() % 8);
        else if (r < 50) x = 0x00;
        else x = static_cast<std::uint8_t>(rng() % 0x80);
        ref.feed(x, want);
        b.send_direct(x);
        for (unsigned k = rng() % 3; k > 0; --k) b.tick();
    }
    b.tick();
    b.tick();
    if (same_events(b.dir_evs, want, "random stream vs reference model"))
        std::printf("random: 300000 bytes -> %zu events match the reference model\n", want.size());
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    std::mt19937 rng(0x31250);

    test_directed_direct();
    test_serial(BAUD, "31250 baud", rng);
    test_serial(BAUD * 1.02, "+2% baud", rng);
    test_serial(BAUD * 0.98, "-2% baud", rng);
    test_uart_faults();
    test_random(rng);

    std::printf("%s (%zu directed cases, serial at 31250 and +/-2%%, UART fault checks)\n",
                errors ? "FAILED" : "PASSED", directed_cases().size());
    return errors ? 1 : 0;
}
