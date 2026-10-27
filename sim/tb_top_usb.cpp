// Full top with USB_HOST = 1: dclk (24.528 MHz) and the 48 MHz USB clock run
// together; the behavioural device sits on the USB pins. After the probe has enumerated the device, 0xF9 and 0xFD are sent
// back to back on midi_rx and both status packets are decoded from uart_tx.
//
// Usage (from the project root): Vprimer25k_nanospc_top BAUD
#include <cstdint>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <vector>

#include "Vprimer25k_nanospc_top.h"
#include "usb_device_model.h"
#include "verilated.h"

static constexpr double DCLK_HZ = 24528000.0;
static constexpr double USB_HZ = 48000000.0;

static int failed = 0, total = 0;
static void check(bool ok, const char* name, const char* fmt = "", long a = 0, long b = 0) {
    ++total;
    failed += !ok;
    std::printf("  [%s] %s", ok ? "PASS" : "FAIL", name);
    if (*fmt) { std::printf(": "); std::printf(fmt, a, b); }
    std::printf("\n");
}

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    const double baud = argc > 1 ? std::atof(argv[1]) : 115200.0;
    Vprimer25k_nanospc_top top;
    UsbDeviceModel dev;
    dev.nak_first_in = false;
    dev.corrupt_data_packet = -1;

    const double t_dclk = 1e12 / DCLK_HZ, t_usb = 1e12 / USB_HZ;   // ps
    double next_dclk = 0, next_usb = 0, now = 0;
    top.midi_rx = 1;
    top.clk_50m = 0;
    top.sim_clk48 = 0;
    top.eval();

    // UART stimulus (dclk-timed) and receiver
    const double bit_ps = 1e12 / baud;
    std::vector<int> tx_bits;           // queued line levels, one per bit
    double tx_next = 0;
    std::vector<std::uint8_t> rx;
    bool rx_busy = false;
    double rx_start = 0;
    int rx_bit = 0;
    std::uint8_t rx_byte = 0;
    bool last_tx = true;

    auto send_byte = [&](std::uint8_t b) {
        tx_bits.push_back(0);
        for (int i = 0; i < 8; ++i) tx_bits.push_back((b >> i) & 1);
        tx_bits.push_back(1);
    };

    auto run_ms = [&](double ms) {
        double end = now + ms * 1e9;
        while (now < end) {
            if (next_usb <= next_dclk) {
                now = next_usb;
                next_usb += t_usb / 2;
                if (!top.sim_clk48) {           // rising edge: device model step
                    bool dp, dn;
                    dev.tick(top.sim_usb_tx_oen, top.sim_usb_tx_dp, top.sim_usb_tx_dn, dp, dn);
                    top.sim_usb_dp = dp;
                    top.sim_usb_dn = dn;
                }
                top.sim_clk48 = !top.sim_clk48;
            } else {
                now = next_dclk;
                next_dclk += t_dclk / 2;
                top.clk_50m = !top.clk_50m;
                if (top.clk_50m) {
                    if (!tx_bits.empty() && now >= tx_next) {
                        top.midi_rx = tx_bits.front();
                        tx_bits.erase(tx_bits.begin());
                        tx_next = now + bit_ps;
                    } else if (tx_bits.empty() && now >= tx_next) {
                        top.midi_rx = 1;
                    }
                    bool line = top.uart_tx;
                    if (!rx_busy && last_tx && !line) {
                        rx_busy = true; rx_start = now; rx_bit = 0; rx_byte = 0;
                    } else if (rx_busy) {
                        double centre = rx_start + bit_ps * (rx_bit + 1.5);
                        if (now >= centre) {
                            if (rx_bit < 8) rx_byte |= static_cast<std::uint8_t>(line << rx_bit);
                            if (++rx_bit == 9) { rx.push_back(rx_byte); rx_busy = false; }
                        }
                    }
                    last_tx = line;
                }
            }
            top.eval();
        }
    };

    std::printf("enumeration through the full top\n");
    run_ms(250);
    check(top.sim_running, "APU running alongside the USB host");
    check(dev.resets == 1, "device saw one bus reset", "%ld", dev.resets);
    check(dev.descriptors_sent == 1 && dev.configs_sent == 1, "device and configuration descriptors read", "%ld", dev.configs_sent);
    check(dev.address == 1 && dev.configuration == 1, "device addressed and configured", "%ld", dev.configuration);
    check(dev.token_crc_errors + dev.data_crc_errors + dev.sync_errors + dev.protocol_errors == 0,
          "no CRC/framing/protocol errors", "%ld",
          dev.token_crc_errors + dev.data_crc_errors + dev.sync_errors + dev.protocol_errors);
    check(dev.sof_interval_errors == 0, "SOFs exactly 1 ms apart", "%ld SOFs", dev.sofs);

    check(rx.empty(), "uart_tx quiet before any request", "%ld bytes, first %02lx",
          static_cast<long>(rx.size()), rx.empty() ? 0L : static_cast<long>(rx[0]));
    rx.clear();
    std::printf("status packets (0xF9 then 0xFD back to back)\n");
    send_byte(0xF9);
    send_byte(0xFD);
    run_ms(40);                         // 22 + 40 + 101 + 1 bytes at 115200 baud
    std::size_t i9 = rx.size(), i5 = rx.size();
    for (std::size_t i = 0; i + 1 < rx.size(); ++i) {
        if (rx[i] == 0xA5 && rx[i + 1] == 0x5A && i9 == rx.size()) i9 = i;
        if (rx[i] == 0xA5 && rx[i + 1] == 0x5B && i5 == rx.size()) i5 = i;
    }
    const std::size_t L = dev.config.size(), n5 = 40 + L + 1;
    check(rx.size() == 22 + n5, "both packets received", "%ld bytes", static_cast<long>(rx.size()));
    check(i9 == 0 && i5 == 22, "main packet first, USB packet after it", "offsets %ld/%ld",
          static_cast<long>(i9), static_cast<long>(i5));
    if (i5 + n5 <= rx.size()) {
        const std::uint8_t* p = &rx[i5];
        std::uint8_t x = 0;
        for (std::size_t k = 0; k + 1 < n5; ++k) x ^= p[k];
        check(x == p[n5 - 1], "USB packet XOR");
        check(p[2] == 10, "state: receiving MIDI", "%ld", p[2]);
        check(p[34] == 0x82 && (p[35] & 0x7f) == 64 && !(p[35] & 0x80), "MIDI IN endpoint 0x82, 64 bytes, no overflow",
              "0x%02lx", p[34]);
        check((p[3] & 3) == 1, "line state J (FS device idle)", "%ld", p[3] & 3);
        int frame = p[4] | (p[5] << 8);
        check(frame > 50, "frame counter advancing", "%ld", frame);
        check(p[6] == 1, "attach count 1", "%ld", p[6]);
        check(p[8] == 0 && p[9] == 0 && p[10] == 0, "no NAK/timeout/CRC errors", "%ld", p[8] + p[9] + p[10]);
        check(p[11] == 18, "descriptor length 18", "%ld", p[11]);
        bool same = true;
        for (int k = 0; k < 18; ++k) same &= p[12 + k] == dev.descriptor[static_cast<std::size_t>(k)];
        check(same, "device descriptor bytes match");
        check(p[30] == 1 && p[31] == 1, "address 1, configuration 1", "%ld/%ld", p[30], p[31]);
        check((p[32] | (p[33] << 8)) == static_cast<int>(L), "configuration length", "%ld",
              p[32] | (p[33] << 8));
        same = true;
        for (std::size_t k = 0; k < L; ++k) same &= p[40 + k] == dev.config[k];
        check(same, "configuration descriptor bytes match");
        std::printf("  VID:PID %02x%02x:%02x%02x\n", p[12 + 9], p[12 + 8], p[12 + 11], p[12 + 10]);
    }
    std::printf("notes over USB, merged with the UART input\n");
    auto peak = [&](double ms) {
        int m = 0;
        double end = now + ms * 1e9;
        while (now < end) {
            run_ms(0.1);
            int l = static_cast<std::int16_t>(top.sim_audio_l);
            m = std::max(m, std::abs(l));
        }
        return m;
    };
    unsigned sent0 = top.sim_mb_sent;
    int quiet = peak(20);
    dev.queue_event(0x09, 0x90, 69, 100);                 // USB: Note On A4
    send_byte(0x90); send_byte(64); send_byte(100);        // UART: Note On E4, same moment
    int playing = peak(60);
    check(top.sim_mb_sent == sent0 + 2, "both Note Ons reached the driver", "%ld commands",
          static_cast<long>(top.sim_mb_sent - sent0));
    check(quiet < 50 && playing > 2000, "audio starts", "peak %ld -> %ld", quiet, playing);
    dev.queue_event(0x08, 0x80, 69, 0);                   // USB: Note Off
    dev.queue_event(0x09, 0x90, 64, 0);                   // USB: Note On velocity 0 = Note Off (E4)
    peak(60);
    int after = peak(30);
    check(top.sim_mb_sent == sent0 + 4, "Note Offs reached the driver", "%ld commands",
          static_cast<long>(top.sim_mb_sent - sent0));
    check(after < playing / 50, "audio released", "peak %ld", after);
    check(!top.sim_mb_dropped && !top.sim_mb_timeout, "mailbox: no drops or timeouts");

    if (i9 + 22 <= rx.size()) {
        std::uint8_t x = 0;
        for (int k = 0; k < 21; ++k) x ^= rx[i9 + static_cast<std::size_t>(k)];
        check(x == rx[i9 + 21], "main packet XOR (unchanged format)");
    }

    std::printf("\n%d/%d checks passed\n", total - failed, total);
    return failed ? 1 : 0;
}
