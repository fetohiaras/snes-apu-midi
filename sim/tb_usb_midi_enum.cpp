#include <cstdio>

#include "Vtb_usb_midi_enum.h"
#include "Vtb_usb_midi_enum___024root.h"
#include "verilated.h"

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    auto* dut = new Vtb_usb_midi_enum;

    dut->resetn = 0;
    for (int i = 0; i < 8; ++i) {
        dut->clk = 1; dut->eval();
        dut->clk = 0; dut->eval();
    }
    dut->resetn = 1;

    const long MAX_CYCLES = 400000;
    long cyc = 0;
    int last_pkt_count = -1;
    bool configured_seen = false;
    bool error_seen = false;
    long configured_at = -1, error_at = -1, first_poll_at = -1;

    int last_state = -1, last_retry = -1, last_rx_pid = -1, last_rx_ok = -1;

    for (; cyc < MAX_CYCLES; ++cyc) {
        dut->clk = 1; dut->eval();
        dut->clk = 0; dut->eval();

        auto* rp = dut->rootp;
        int st    = rp->tb_usb_midi_enum__DOT__dut__DOT__state;
        int retry = rp->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt;
        int rxpid = rp->tb_usb_midi_enum__DOT__dut__DOT__rx_pid;
        int rxok  = rp->tb_usb_midi_enum__DOT__dut__DOT__rx_ok;
        int rxdone = rp->tb_usb_midi_enum__DOT__dut__DOT__rx_done;
        if (st != last_state || retry != last_retry) {
            std::printf("[%8ld] host: state=%d retry_cnt=%d ctl_which=%d tok_addr=%d data_toggle=%d\n",
                         cyc, st, retry,
                         (int)rp->tb_usb_midi_enum__DOT__dut__DOT__ctl_which,
                         (int)rp->tb_usb_midi_enum__DOT__dut__DOT__tok_addr,
                         (int)rp->tb_usb_midi_enum__DOT__dut__DOT__data_toggle);
            last_state = st;
            last_retry = retry;
        }
        if (rxdone) {
            std::printf("[%8ld] host rx_done: rx_ok=%d rx_pid=0x%02x\n", cyc, rxok, rxpid);
        }
        last_rx_pid = rxpid;
        last_rx_ok  = rxok;

        if (cyc >= 3330 && cyc <= 3670) {
            std::printf("[%8ld] host.sie: rxstate=%d txstate=%d oe=%d sym_j=%d sym_k=%d rx_bitclk=%d pid_cnt=%d "
                        "byte_cnt=%d rcvd_bytes=%d rx_crc=0x%04x stuff_err=%d rx_pid=0x%02x rx_active=%d | dev.sie: txstate=%d oe=%d tx_busy=%d "
                        "sym_j=%d sym_k=%d rx_enable=%d\n",
                        cyc,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_pid,
                        (int)rp->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_active,
                        (int)rp->tb_usb_midi_enum__DOT__device_sie__DOT__txstate,
                        (int)rp->tb_usb_midi_enum__DOT__device_sie__DOT__oe,
                        (int)rp->tb_usb_midi_enum__DOT__device_sie__DOT__tx_busy,
                        (int)rp->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j,
                        (int)rp->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k,
                        (int)rp->tb_usb_midi_enum__DOT__device_sie__DOT__rx_enable);
        }

        if (dut->pkt_count_dbg != last_pkt_count) {
            last_pkt_count = dut->pkt_count_dbg;
            std::printf("[%8ld] pacotes do host contados pelo dispositivo: %d\n", cyc, last_pkt_count);
        }
        if (dut->dev_configured && !configured_seen) {
            configured_seen = true;
            configured_at = cyc;
        }
        if (dut->dev_error && !error_seen) {
            error_seen = true;
            error_at = cyc;
        }
        if (dut->poll_count >= 1 && first_poll_at < 0) {
            first_poll_at = cyc;
        }
        if (configured_seen && dut->poll_count >= 1) break;
        if (error_seen) break;
    }

    std::printf("\n=== resultado da enumeracao ===\n");
    std::printf("ciclos simulados: %ld / %ld\n", cyc, MAX_CYCLES);
    std::printf("dev_configured: %s", dut->dev_configured ? "SIM" : "NAO");
    if (configured_at >= 0) std::printf(" (em %ld)", configured_at);
    std::printf("\n");
    std::printf("dev_error: %s", dut->dev_error ? "SIM" : "NAO");
    if (error_at >= 0) std::printf(" (em %ld)", error_at);
    std::printf("\n");
    std::printf("poll_count: %u", dut->poll_count);
    if (first_poll_at >= 0) std::printf(" (1o poll em %ld)", first_poll_at);
    std::printf("\n");
    std::printf("nak_count: %u\n", dut->nak_count);
    std::printf("pacotes do host contados pelo dispositivo ao final: %d\n", dut->pkt_count_dbg);

    const bool pass = dut->dev_configured && !dut->dev_error && dut->poll_count >= 1;
    std::printf("\n%s: %s\n", pass ? "PASSOU" : "FALHOU",
                pass ? "o host enumerou um dispositivo real e chegou a varredura do endpoint bulk"
                     : "o host NAO completou a enumeracao contra um dispositivo que responde de verdade");

    delete dut;
    return pass ? 0 : 1;
}
