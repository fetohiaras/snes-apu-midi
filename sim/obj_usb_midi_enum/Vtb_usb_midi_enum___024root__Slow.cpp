// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_usb_midi_enum.h for the primary calling header

#include "Vtb_usb_midi_enum__pch.h"
#include "Vtb_usb_midi_enum__Syms.h"
#include "Vtb_usb_midi_enum___024root.h"

// Parameter definitions for Vtb_usb_midi_enum___024root
constexpr CData/*6:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__NEW_ADDR;
constexpr CData/*3:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__MIDI_BULK_IN_EP;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_RST_ASSERT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_RST_RELEASE;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_LOAD;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_SETUP_TX;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_SETUP_TXWAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_SETUP_DATA_TX;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_SETUP_DATAWAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_SETUP_ACKWAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_STATUS_TOK_TX;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_STATUS_TOKWAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_STATUS_RXWAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_STATUS_ACK_TX;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_STATUS_ACKWAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_CTL_POST_DELAY;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_POLL_TOK_TX;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_POLL_TOK_WAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_POLL_RXWAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_POLL_ACK_TX;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_POLL_ACK_WAIT;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_POLL_PROCESS;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_POLL_GAP;
constexpr CData/*4:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__S_ERROR_HALT;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__TXS_IDLE;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__TXS_BITS;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__TXS_EOP_SE0;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__TXS_EOP_J;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__RXS_IDLE;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__RXS_SYNC;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__RXS_DATA;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__RXS_EOP_SE0;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__RXS_EOP_J;
constexpr CData/*3:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__EOP_SE0_HOLD;
constexpr CData/*3:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__EOP_J_HOLD;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__TXS_IDLE;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__TXS_BITS;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__TXS_EOP_SE0;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__TXS_EOP_J;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__RXS_IDLE;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__RXS_SYNC;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__RXS_DATA;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__RXS_EOP_SE0;
constexpr CData/*2:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__RXS_EOP_J;
constexpr CData/*3:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__EOP_SE0_HOLD;
constexpr CData/*3:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__device_sie__DOT__EOP_J_HOLD;
constexpr IData/*31:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__RESET_HOLD_CYCLES;
constexpr IData/*31:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__SETTLE_CYCLES;
constexpr IData/*31:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__POST_CTL_DELAY_CYCLES;
constexpr IData/*31:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__TIMEOUT_CYCLES;
constexpr IData/*31:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__POLL_GAP_CYCLES;
constexpr IData/*31:0*/ Vtb_usb_midi_enum___024root::tb_usb_midi_enum__DOT__dut__DOT__MAX_RETRIES;


void Vtb_usb_midi_enum___024root___ctor_var_reset(Vtb_usb_midi_enum___024root* vlSelf);

Vtb_usb_midi_enum___024root::Vtb_usb_midi_enum___024root(Vtb_usb_midi_enum__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    Vtb_usb_midi_enum___024root___ctor_var_reset(this);
}

void Vtb_usb_midi_enum___024root::__Vconfigure(bool first) {
    if (false && first) {}  // Prevent unused
}

Vtb_usb_midi_enum___024root::~Vtb_usb_midi_enum___024root() {
}
