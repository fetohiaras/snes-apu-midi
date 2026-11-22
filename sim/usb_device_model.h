// Behavioural full-speed USB device for the host tests.
//
// Runs at the host's 48 MHz clock (4 clocks per bit). It decodes what the
// host drives (NRZI, bit stuffing, SYNC/EOP), checks every token CRC5 and
// data CRC16, and implements endpoint 0 of a USB-MIDI keyboard:
//   GET_DESCRIPTOR(DEVICE / CONFIGURATION), SET_ADDRESS, SET_CONFIGURATION,
// with control reads split into max_packet-sized DATA1/DATA0 packets, and
// the bulk IN endpoint of the MIDI Streaming interface (queue_event()).
// Fault injection: NAK the first IN, and/or corrupt the CRC of one data
// packet (the host must not ACK it and must retry).
#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <vector>

class UsbDeviceModel {
public:
    enum Line { SE0 = 0, J = 1, K = 2, SE1 = 3 };

    // configuration
    bool attached = true;
    int max_packet = 8;
    int sof_period = 48000;        // expected clocks between SOFs (+-10)
    bool nak_first_in = true;
    int corrupt_data_packet = 1;   // index (0-based) of the DATA packet sent with a bad CRC; -1 = none
    int bulk_ep = 2;               // bulk IN endpoint number (CTK-3500: 0x82)
    int bulk_size = 64;
    int corrupt_bulk_packet = -1;  // index of the bulk packet sent with a bad CRC
    int ignore_bulk_ack = -1;      // index of the bulk packet whose ACK is "lost" (resent, same toggle)
    std::vector<std::uint8_t> descriptor, config;

    // statistics
    std::uint64_t now = 0;
    int resets = 0;
    std::uint64_t reset_start = 0, last_reset_len = 0;
    int sofs = 0, sof_frame_errors = 0, sof_interval_errors = 0;
    std::uint64_t last_sof_time = 0;
    int last_sof_frame = -1;
    int setups = 0, ins = 0, outs = 0, host_acks = 0;
    int token_crc_errors = 0, data_crc_errors = 0, sync_errors = 0, protocol_errors = 0;
    int descriptors_sent = 0;      // completed GET_DESCRIPTOR(DEVICE) control transfers
    int configs_sent = 0;          // completed GET_DESCRIPTOR(CONFIGURATION)
    int data_packets = 0;
    int address = 0;               // current device address
    int configuration = 0;         // set by SET_CONFIGURATION
    int tokens_to_old_address = 0; // tokens for address 0 after SET_ADDRESS took effect
    int bulk_ins = 0, bulk_naks = 0, bulk_packets = 0, bulk_delivered = 0;

    // Queue one 4-byte USB-MIDI event for the bulk IN endpoint.
    void queue_event(std::uint8_t cin, std::uint8_t b1, std::uint8_t b2 = 0, std::uint8_t b3 = 0) {
        bulk_queue_.insert(bulk_queue_.end(), {cin, b1, b2, b3});
    }
    bool bulk_empty() const { return bulk_queue_.empty() && !bulk_await_; }

    UsbDeviceModel() {
        descriptor = {18, 1, 0x10, 0x01, 0, 0, 0, 8,       // bLength..bMaxPacketSize0
                      0x99, 0x07, 0x34, 0x12,              // idVendor 0x0799, idProduct 0x1234
                      0x00, 0x01, 1, 2, 0, 1};
        // Casio CTK-3500 configuration, as read by the FPGA: the USB-MIDI 1.0
        // example layout (AudioControl + MIDIStreaming, embedded/external
        // jack pairs) with bulk OUT 0x01 and bulk IN 0x82, 64 bytes; 101 bytes.
        config = {
            0x09, 0x02, 0x65, 0x00, 0x02, 0x01, 0x00, 0xc0, 0x00, 0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01,
            0x00, 0x00, 0x09, 0x24, 0x01, 0x00, 0x01, 0x09, 0x00, 0x01, 0x01, 0x09, 0x04, 0x01, 0x00, 0x02,
            0x01, 0x03, 0x00, 0x00, 0x07, 0x24, 0x01, 0x00, 0x01, 0x41, 0x00, 0x06, 0x24, 0x02, 0x01, 0x01,
            0x00, 0x06, 0x24, 0x02, 0x02, 0x02, 0x00, 0x09, 0x24, 0x03, 0x01, 0x03, 0x01, 0x02, 0x01, 0x00,
            0x09, 0x24, 0x03, 0x02, 0x04, 0x01, 0x01, 0x01, 0x00, 0x09, 0x05, 0x01, 0x02, 0x40, 0x00, 0x00,
            0x00, 0x00, 0x05, 0x25, 0x01, 0x01, 0x01, 0x09, 0x05, 0x82, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00,
            0x05, 0x25, 0x01, 0x01, 0x03,
        };
    }

    // One 48 MHz clock. host_oen: PHY output enable (active low).
    // Returns the resolved bus level seen by the host's receiver.
    void tick(bool host_oen, bool host_dp, bool host_dn, bool& bus_dp, bool& bus_dn) {
        ++now;
        Line line;
        bool host_drives = !host_oen;
        if (host_drives) {
            line = level(host_dp, host_dn);
        } else if (!tx_.empty()) {
            line = tx_.front();
            tx_.pop_front();
        } else {
            line = attached ? J : SE0;          // FS pull-up on D+, else the host pulldowns
        }
        bus_dp = line == J || line == SE1;
        bus_dn = line == K || line == SE1;
        if (host_drives) receive(line);
        else rx_reset_to_idle(line);
        track_reset(host_drives, line);
    }

private:
    std::deque<Line> tx_;
    bool in_reset_ = false;

    // receiver
    enum RxState { RX_IDLE, RX_ACTIVE, RX_WAIT_IDLE } rx_ = RX_IDLE;
    Line rx_prev_ = J, rx_last_level_ = J;
    int rx_phase_ = 0, rx_ones_ = 0;
    std::vector<int> rx_bits_;

    // protocol
    enum { NONE, AFTER_SETUP, AFTER_OUT } pending_ = NONE;
    enum Stage { IDLE, DATA_IN, STATUS_IN } stage_ = IDLE;
    enum Kind { K_OTHER, K_DEVICE, K_CONFIG, K_ADDRESS, K_CONFIGURE } kind_ = K_OTHER;
    bool await_ack_ = false, nak_done_ = false, in_data1_ = true;
    std::vector<std::uint8_t> response_;
    std::size_t resp_pos_ = 0, last_chunk_ = 0;
    int new_value_ = 0;
    std::vector<std::uint8_t> bulk_queue_;
    bool bulk_data1_ = false, bulk_await_ = false;
    std::size_t bulk_chunk_ = 0;

    static Line level(bool dp, bool dn) {
        return dp ? (dn ? SE1 : J) : (dn ? K : SE0);
    }

    void track_reset(bool host_drives, Line line) {
        bool se0 = host_drives && line == SE0;
        if (se0 && !in_reset_) { reset_start = now; in_reset_ = true; }
        if (!se0 && in_reset_) {
            in_reset_ = false;
            std::uint64_t len = now - reset_start;
            if (len > 120) {            // > 2.5 us: bus reset, not an EOP
                ++resets;
                last_reset_len = len;
                pending_ = NONE; stage_ = IDLE; await_ack_ = false;
                address = 0; configuration = 0;
                bulk_await_ = false; bulk_data1_ = false;
                last_sof_frame = -1;
            }
        }
    }

    void rx_reset_to_idle(Line line) {
        if (rx_ == RX_ACTIVE) { ++sync_errors; }
        rx_ = RX_IDLE;
        rx_prev_ = line;
    }

    void receive(Line line) {
        switch (rx_) {
        case RX_IDLE:
            if (line == K) {
                rx_ = RX_ACTIVE;
                rx_phase_ = 0;
                rx_last_level_ = J;
                rx_ones_ = 0;
                rx_bits_.clear();
            }
            break;
        case RX_ACTIVE:
            rx_phase_ = line != rx_prev_ ? 0 : (rx_phase_ + 1) % 4;
            if (rx_phase_ == 2) {
                if (line == SE0) {
                    rx_ = RX_WAIT_IDLE;
                    packet_done();
                } else {
                    int bit = line == rx_last_level_ ? 1 : 0;
                    rx_last_level_ = line;
                    if (rx_ones_ == 6) {
                        if (bit) ++protocol_errors;     // stuffing violation
                        rx_ones_ = 0;
                    } else {
                        rx_bits_.push_back(bit);
                        rx_ones_ = bit ? rx_ones_ + 1 : 0;
                    }
                }
            }
            break;
        case RX_WAIT_IDLE:
            if (line == J) rx_ = RX_IDLE;
            break;
        }
        rx_prev_ = line;
    }

    static std::uint8_t crc5(std::uint32_t v, int n) {
        std::uint8_t c = 0x1f;
        for (int i = 0; i < n; ++i) {
            int fb = ((c >> 4) & 1) ^ ((v >> i) & 1);
            c = static_cast<std::uint8_t>(((c << 1) & 0x1f) ^ (fb ? 0x05 : 0));
        }
        return static_cast<std::uint8_t>(~c & 0x1f);
    }

    static std::uint16_t crc16(const std::uint8_t* d, std::size_t n) {
        std::uint16_t c = 0xffff;
        for (std::size_t i = 0; i < n; ++i) {
            c ^= d[i];
            for (int b = 0; b < 8; ++b) c = (c & 1) ? static_cast<std::uint16_t>((c >> 1) ^ 0xa001) : static_cast<std::uint16_t>(c >> 1);
        }
        return static_cast<std::uint16_t>(~c);
    }

    void packet_done() {
        if (rx_bits_.size() % 8 != 0 || rx_bits_.size() < 16) { ++sync_errors; return; }
        std::vector<std::uint8_t> b;
        for (std::size_t i = 0; i < rx_bits_.size(); i += 8) {
            std::uint8_t v = 0;
            for (int k = 0; k < 8; ++k) v |= static_cast<std::uint8_t>(rx_bits_[i + k] << k);
            b.push_back(v);
        }
        if (b[0] != 0x80) { ++sync_errors; return; }
        b.erase(b.begin());
        std::uint8_t pid = b[0];
        if (((pid >> 4) ^ 0xf) != (pid & 0xf)) { ++protocol_errors; return; }

        if (pid == 0xa5 || pid == 0x2d || pid == 0x69 || pid == 0xe1) {
            if (b.size() != 3) { ++protocol_errors; return; }
            std::uint32_t v = b[1] | (b[2] << 8);
            std::uint32_t field = v & 0x7ff;
            std::uint8_t rx = 0;
            for (int i = 0; i < 5; ++i) rx |= static_cast<std::uint8_t>(((v >> (15 - i)) & 1) << i);
            if (rx != crc5(field, 11)) { ++token_crc_errors; return; }
            token(pid, field);
        } else if (pid == 0xc3 || pid == 0x4b) {
            if (b.size() < 3) { ++protocol_errors; return; }
            std::size_t n = b.size() - 3;
            std::uint16_t c = crc16(&b[1], n);
            if ((b[1 + n] | (b[2 + n] << 8)) != c) { ++data_crc_errors; return; }
            data(pid == 0x4b, std::vector<std::uint8_t>(b.begin() + 1, b.begin() + 1 + static_cast<long>(n)));
        } else if (pid == 0xd2) {
            ++host_acks;
            if (bulk_await_) {
                bulk_await_ = false;
                if (bulk_packets - 1 == ignore_bulk_ack) { ignore_bulk_ack = -2; return; }   // "lost"
                bulk_queue_.erase(bulk_queue_.begin(), bulk_queue_.begin() + static_cast<long>(bulk_chunk_));
                bulk_data1_ = !bulk_data1_;
                ++bulk_delivered;
                return;
            }
            if (!await_ack_) return;
            await_ack_ = false;
            if (stage_ == DATA_IN) {
                resp_pos_ += last_chunk_;
                in_data1_ = !in_data1_;
            } else if (stage_ == STATUS_IN) {
                stage_ = IDLE;                       // control write complete
                if (kind_ == K_ADDRESS) address = new_value_;
                if (kind_ == K_CONFIGURE) configuration = new_value_;
            }
        } else {
            ++protocol_errors;
        }
    }

    void send_data(bool data1, const std::uint8_t* d, std::size_t n) {
        std::vector<std::uint8_t> pkt = {static_cast<std::uint8_t>(data1 ? 0x4b : 0xc3)};
        pkt.insert(pkt.end(), d, d + n);
        std::uint16_t c = crc16(d, n);
        if (data_packets++ == corrupt_data_packet) c ^= 0x0100;
        pkt.push_back(static_cast<std::uint8_t>(c));
        pkt.push_back(static_cast<std::uint8_t>(c >> 8));
        send(pkt);
        await_ack_ = true;
    }

    void token(std::uint8_t pid, std::uint32_t field) {
        if (pid == 0xa5) {
            ++sofs;
            int fr = static_cast<int>(field);
            if (last_sof_frame >= 0) {
                if (fr != ((last_sof_frame + 1) & 0x7ff)) ++sof_frame_errors;
                std::uint64_t dt = now - last_sof_time;
                if (dt + 10 < static_cast<std::uint64_t>(sof_period) || dt > static_cast<std::uint64_t>(sof_period) + 10) ++sof_interval_errors;
            }
            last_sof_frame = fr;
            last_sof_time = now;
            return;
        }
        int addr = field & 0x7f, ep = (field >> 7) & 0xf;
        if (addr == 0 && address != 0) ++tokens_to_old_address;
        if (addr != address || !attached) return;                // not for us: no response
        if (ep != 0) {
            if (pid != 0x69 || ep != bulk_ep || configuration == 0) { ++protocol_errors; return; }
            ++bulk_ins;
            bulk_await_ = false;
            if (bulk_queue_.empty()) { ++bulk_naks; send({0x5a}); return; }
            bulk_chunk_ = std::min<std::size_t>(static_cast<std::size_t>(bulk_size), bulk_queue_.size());
            std::vector<std::uint8_t> pkt = {static_cast<std::uint8_t>(bulk_data1_ ? 0x4b : 0xc3)};
            pkt.insert(pkt.end(), bulk_queue_.begin(), bulk_queue_.begin() + static_cast<long>(bulk_chunk_));
            std::uint16_t c = crc16(&pkt[1], bulk_chunk_);
            if (bulk_packets++ == corrupt_bulk_packet) c ^= 0x0100;
            pkt.push_back(static_cast<std::uint8_t>(c));
            pkt.push_back(static_cast<std::uint8_t>(c >> 8));
            send(pkt);
            bulk_await_ = true;
            return;
        }
        await_ack_ = false;
        if (pid == 0x2d) { ++setups; pending_ = AFTER_SETUP; }
        else if (pid == 0xe1) { ++outs; pending_ = AFTER_OUT; }
        else {
            ++ins;
            if (nak_first_in && !nak_done_) { nak_done_ = true; send({0x5a}); return; }
            if (stage_ == STATUS_IN) { send_data(true, nullptr, 0); return; }
            if (stage_ != DATA_IN) { send({0x1e}); return; }             // STALL
            last_chunk_ = std::min<std::size_t>(static_cast<std::size_t>(max_packet), response_.size() - resp_pos_);
            send_data(in_data1_, response_.data() + resp_pos_, last_chunk_);
        }
    }

    void data(bool data1, const std::vector<std::uint8_t>& d) {
        if (pending_ == AFTER_SETUP) {
            pending_ = NONE;
            if (data1 || d.size() != 8) { ++protocol_errors; return; }
            send({0xd2});
            std::size_t len = d[6] | (d[7] << 8);
            resp_pos_ = 0;
            in_data1_ = true;
            if (d[0] == 0x80 && d[1] == 0x06 && (d[3] == 0x01 || d[3] == 0x02)) {
                const std::vector<std::uint8_t>& src = d[3] == 0x01 ? descriptor : config;
                response_.assign(src.begin(), src.begin() + static_cast<long>(std::min(len, src.size())));
                kind_ = d[3] == 0x01 ? K_DEVICE : K_CONFIG;
                stage_ = DATA_IN;
            } else if (d[0] == 0x00 && (d[1] == 0x05 || d[1] == 0x09)) {
                kind_ = d[1] == 0x05 ? K_ADDRESS : K_CONFIGURE;
                new_value_ = d[2];
                stage_ = STATUS_IN;
            } else {
                ++protocol_errors;
                stage_ = IDLE;
            }
        } else if (pending_ == AFTER_OUT) {
            pending_ = NONE;
            send({0xd2});
            if (!data1 || !d.empty() || stage_ != DATA_IN) { ++protocol_errors; return; }
            if (resp_pos_ >= response_.size()) {
                if (kind_ == K_DEVICE) ++descriptors_sent;
                if (kind_ == K_CONFIG) ++configs_sent;
            }
            stage_ = IDLE;
        } else {
            ++protocol_errors;
        }
    }

    // Queue a packet: turnaround, SYNC, NRZI with stuffing, EOP.
    void send(const std::vector<std::uint8_t>& bytes) {
        for (int i = 0; i < 4 * 3; ++i) tx_.push_back(attached ? J : SE0);  // ~3 bit times turnaround
        std::vector<int> bits;
        auto put = [&](std::uint8_t v) { for (int k = 0; k < 8; ++k) bits.push_back((v >> k) & 1); };
        put(0x80);
        for (std::uint8_t v : bytes) put(v);
        Line lv = J;
        int ones = 0;
        auto emit = [&](Line l) { for (int i = 0; i < 4; ++i) tx_.push_back(l); };
        for (int bit : bits) {
            if (!bit) lv = lv == J ? K : J;
            emit(lv);
            ones = bit ? ones + 1 : 0;
            if (ones == 6) { lv = lv == J ? K : J; emit(lv); ones = 0; }
        }
        emit(SE0); emit(SE0); emit(J);
    }
};
