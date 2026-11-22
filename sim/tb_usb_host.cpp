// USB host bring-up test: usb_host_probe + usb_fs_phy against the
// behavioural device in usb_device_model.h, at 48 MHz.
//
//   1. attach (8-byte EP0, first IN NAKed, second DATA packet corrupted)
//      -> bus reset, SOFs, GET_DESCRIPTOR(DEVICE) read in 3 packets
//   2. unplug -> the host notices and stops; replug with a 64-byte EP0
//      -> second enumeration in one packet
//   3. MIDI over the bulk IN endpoint found in the configuration descriptor
//      (the CTK-3500's, 0x82): every Code Index Number, a reserved one, a
//      multi-packet burst, a corrupted packet and a lost ACK (duplicate)
//   4. replug with a 32-byte EP0 and a 128-byte configuration
//   5. SOF timing and frame numbering while idle
#include <cstdint>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

#include "Vtb_usb_host_top.h"
#include "usb_device_model.h"
#include "verilated.h"

static constexpr std::uint64_t CLK_HZ = 48000000;
static constexpr std::uint64_t MS = CLK_HZ / 1000;
enum { S_DETACHED = 0, S_RESET = 1, S_DONE = 6, S_POLL = 10 };

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
    Vtb_usb_host_top top;
    UsbDeviceModel dev;
    std::uint64_t t = 0;

    std::vector<std::uint8_t> midi;      // bytes out of the FIFO
    auto step = [&]() {
        bool dp, dn;
        if (top.midi_valid) midi.push_back(top.midi_data);
        dev.tick(top.tx_oen, top.tx_dp, top.tx_dn, dp, dn);
        top.bus_dp = dp;
        top.bus_dn = dn;
        top.clk = 0; top.eval();
        top.clk = 1; top.eval();
        ++t;
    };
    auto run_until = [&](const std::function<bool()>& cond, std::uint64_t limit) {
        std::uint64_t end = t + limit;
        while (t < end && !cond()) step();
        return cond();
    };
    auto desc_byte = [&](int i) { return static_cast<int>((top.desc[i / 4] >> (8 * (i % 4))) & 0xff); };
    auto desc_ok = [&]() {
        for (int i = 0; i < 18; ++i) if (desc_byte(i) != dev.descriptor[static_cast<std::size_t>(i)]) return false;
        return top.desc_len == 18;
    };
    auto cfg_ok = [&]() {
        if (top.cfg_len != dev.config.size()) return false;
        for (std::size_t i = 0; i < dev.config.size(); ++i) {
            top.cfg_raddr = static_cast<std::uint8_t>(i);
            top.eval();
            if (top.cfg_rdata != dev.config[i]) return false;
        }
        return true;
    };
    auto enum_ok = [&](int n, const char* what) {
        run_until([] { return false; }, 1 * MS);      // let the device see the final ACK
        check(desc_ok() && dev.descriptors_sent == n, (std::string(what) + ": device descriptor").c_str(),
              "%ld bytes", top.desc_len);
        check(cfg_ok() && dev.configs_sent == n, (std::string(what) + ": configuration descriptor").c_str(),
              "%ld bytes", top.cfg_len);
        check(dev.address == 1 && top.dev_addr == 1, (std::string(what) + ": SET_ADDRESS(1)").c_str(),
              "device %ld", dev.address);
        check(dev.configuration == 1 && top.config_value == 1, (std::string(what) + ": SET_CONFIGURATION(1)").c_str(),
              "device %ld", dev.configuration);
        check(dev.tokens_to_old_address == 0, (std::string(what) + ": nothing sent to address 0 afterwards").c_str(),
              "%ld tokens", dev.tokens_to_old_address);
        check(top.midi_ep == 0x82 && top.midi_ep_size == 64, (std::string(what) + ": MIDI IN endpoint 0x82, 64 bytes").c_str(),
              "0x%02lx, %ld bytes", top.midi_ep, top.midi_ep_size);
    };

    top.rst = 1;
    for (int i = 0; i < 16; ++i) step();
    top.rst = 0;

    std::printf("enumeration 1 (8-byte EP0, NAK + CRC error injected)\n");
    std::uint64_t t0 = t;
    bool done = run_until([&] { return top.state == S_POLL; }, 400 * MS);
    check(done, "probe reached DONE", "%ld ms", static_cast<long>((t - t0) / MS));
    check(dev.resets == 1, "one bus reset", "%ld resets", dev.resets);
    check(dev.last_reset_len >= 10 * MS && dev.last_reset_len <= 30 * MS, "reset length 10..30 ms",
          "%ld us", static_cast<long>(dev.last_reset_len * 1000 / MS));
    check(dev.token_crc_errors == 0 && dev.data_crc_errors == 0 && dev.sync_errors == 0 && dev.protocol_errors == 0,
          "device saw no CRC/framing/protocol errors from the host", "%ld errors",
          dev.token_crc_errors + dev.data_crc_errors + dev.sync_errors + dev.protocol_errors);
    enum_ok(1, "8-byte EP0");
    std::printf("  VID:PID %02x%02x:%02x%02x, EP0 %d bytes, configuration %d bytes in %d packets\n", desc_byte(9),
                desc_byte(8), desc_byte(11), desc_byte(10), desc_byte(7), static_cast<int>(top.cfg_len),
                static_cast<int>((dev.config.size() + 7) / 8));
    check(top.nak_count == 1, "NAK retried", "%ld NAKs", top.nak_count);
    check(top.crc_count == 1, "corrupted packet rejected and re-read", "%ld CRC errors", top.crc_count);
    check(top.timeout_count == 0, "no timeouts", "%ld", top.timeout_count);
    check(top.attach_count == 1, "attach count 1", "%ld", top.attach_count);
    check(dev.sof_frame_errors == 0 && dev.sof_interval_errors == 0, "SOF every 1 ms with consecutive frame numbers",
          "%ld SOFs", dev.sofs);

    std::printf("MIDI over bulk IN (endpoint from the descriptor)\n");
    // USB-MIDI CIN -> number of MIDI bytes (USB-MIDI 1.0, table 4-1)
    auto cin_bytes = [](int cin) {
        static const int n[16] = {0, 0, 2, 3, 3, 1, 2, 3, 3, 3, 3, 3, 2, 2, 3, 1};
        return n[cin & 15];
    };
    std::vector<std::uint8_t> expect;
    auto ev = [&](int cin, int b1, int b2 = 0, int b3 = 0) {
        dev.queue_event(static_cast<std::uint8_t>(cin), static_cast<std::uint8_t>(b1),
                        static_cast<std::uint8_t>(b2), static_cast<std::uint8_t>(b3));
        const int b[3] = {b1, b2, b3};
        for (int i = 0; i < cin_bytes(cin); ++i) expect.push_back(static_cast<std::uint8_t>(b[i]));
    };
    run_until([] { return false; }, 5 * MS);
    check(dev.bulk_ins > 0 && dev.bulk_naks == dev.bulk_ins, "endpoint polled, NAKed while idle", "%ld polls",
          dev.bulk_ins);
    check(dev.bulk_ins >= 4 && dev.bulk_ins <= 7, "one poll per frame while idle", "%ld in 5 ms", dev.bulk_ins);
    midi.clear();
    // every CIN: note on/off, poly pressure, CC, program, channel pressure,
    // pitch bend, SysEx start/end (1, 2, 3 bytes), system common, a single
    // byte (clock), and a reserved CIN that carries nothing
    ev(0x09, 0x90, 60, 100); ev(0x08, 0x80, 60, 0); ev(0x0A, 0xA0, 60, 30);
    ev(0x0B, 0xB0, 7, 100);  ev(0x0C, 0xC0, 5);     ev(0x0D, 0xD0, 40);
    ev(0x0E, 0xE0, 0, 64);   ev(0x04, 0xF0, 0x7E, 0x7F); ev(0x07, 0x09, 0x01, 0xF7);
    ev(0x04, 0xF0, 0x43, 0x10); ev(0x05, 0xF7);     ev(0x04, 0xF0, 0x01, 0x02); ev(0x06, 0x03, 0xF7);
    ev(0x02, 0xF3, 0x01);    ev(0x03, 0xF2, 0x10, 0x20); ev(0x0F, 0xF8);
    ev(0x00, 0x12, 0x34, 0x56); ev(0x01, 0x99, 0x98, 0x97);
    bool sent = run_until([&] { return dev.bulk_empty(); }, 20 * MS);
    run_until([] { return false; }, 1 * MS);
    check(sent && midi == expect, "all CIN types unpacked, reserved CINs skipped", "%ld bytes",
          static_cast<long>(midi.size()));

    // 40 note events = 160 bytes: three packets (64, 64, 32) in one frame,
    // the second corrupted, the third's ACK lost (the device repeats it)
    midi.clear();
    expect.clear();
    dev.corrupt_bulk_packet = dev.bulk_packets + 1;
    dev.ignore_bulk_ack = dev.bulk_packets + 2;
    int crc0 = top.crc_count;
    for (int i = 0; i < 40; ++i) ev(0x09, 0x90, 36 + i, 1 + 3 * i);
    std::uint64_t t_burst = t;
    sent = run_until([&] { return dev.bulk_empty(); }, 20 * MS);
    run_until([] { return false; }, 1 * MS);
    check(sent && midi == expect, "160-byte (USB) burst received intact", "%ld bytes", static_cast<long>(midi.size()));
    check(top.crc_count == crc0 + 1, "corrupted packet rejected and re-read", "%ld CRC errors", top.crc_count - crc0);
    check(dev.ignore_bulk_ack == -2, "repeated packet (lost ACK) discarded");
    check((t - t_burst) < 5 * MS, "burst delivered within a few frames", "%ld us",
          static_cast<long>((t - t_burst) * 1000 / MS));
    check(top.midi_bytes == 40 + 120 && !top.midi_overflow, "byte counter (40 + 120), no FIFO overflow",
          "%ld bytes", top.midi_bytes);
    int crc_after_midi = top.crc_count;
    check(dev.protocol_errors == 0, "no protocol errors", "%ld", dev.protocol_errors);

    std::printf("unplug / replug (64-byte EP0)\n");
    run_until([] { return false; }, 5 * MS);
    dev.attached = false;
    std::uint64_t t_unplug = t;
    bool gone = run_until([&] { return top.state == S_DETACHED; }, 2 * MS);
    check(gone, "unplug detected within 2 ms", "%ld us", static_cast<long>((t - t_unplug) * 1000 / MS));
    int sofs = dev.sofs;
    run_until([] { return false; }, 20 * MS);
    check(dev.sofs == sofs, "no SOFs while detached", "%ld", dev.sofs - sofs);
    dev.attached = true;
    dev.max_packet = 64;
    dev.corrupt_data_packet = -1;
    t0 = t;
    done = run_until([&] { return top.state == S_POLL; }, 400 * MS);
    check(done, "second enumeration reached DONE", "%ld ms", static_cast<long>((t - t0) / MS));
    check(dev.resets == 2 && top.attach_count == 2, "second reset and attach", "%ld resets", dev.resets);
    enum_ok(2, "64-byte EP0");
    check(top.crc_count == crc_after_midi && top.timeout_count == 0, "no new errors", "%ld CRC", top.crc_count);

    // 128-byte configuration with a 32-byte EP0: the data stage ends on a
    // packet boundary, so the device closes it with a zero-length packet.
    std::printf("replug (32-byte EP0, 128-byte configuration: ends with a zero-length packet;\n"
                "        MIDI endpoint declared as interrupt)\n");
    dev.attached = false;
    run_until([&] { return top.state == S_DETACHED; }, 2 * MS);
    run_until([] { return false; }, 20 * MS);
    dev.max_packet = 32;
    dev.descriptor[7] = 32;
    std::vector<std::uint8_t> pad(27, 0);
    pad[0] = 27;
    pad[1] = 0xff;                      // vendor-specific filler descriptor
    dev.config.insert(dev.config.end(), pad.begin(), pad.end());
    dev.config[2] = 128;
    dev.config[90] = 3;                 // MIDI IN endpoint declared as interrupt (cheap keyboards)
    dev.attached = true;
    done = run_until([&] { return top.state == S_POLL; }, 400 * MS);
    check(done, "third enumeration reached DONE");
    enum_ok(3, "32-byte EP0");

    std::printf("idle frames\n");
    sofs = dev.sofs;
    run_until([] { return false; }, 50 * MS);
    check(dev.sofs - sofs >= 49 && dev.sofs - sofs <= 51, "50 SOFs in 50 ms", "%ld", dev.sofs - sofs);
    check(dev.sof_frame_errors == 0 && dev.sof_interval_errors == 0, "SOF timing still exact", "%ld interval errors",
          dev.sof_interval_errors);
    check(dev.token_crc_errors == 0 && dev.sync_errors == 0 && dev.protocol_errors == 0, "no host-side errors overall",
          "%ld", dev.token_crc_errors + dev.sync_errors + dev.protocol_errors);

    std::printf("\n%d/%d checks passed (%.1f ms simulated)\n", total - failed, total, static_cast<double>(t) / MS);
    return failed ? 1 : 0;
}
