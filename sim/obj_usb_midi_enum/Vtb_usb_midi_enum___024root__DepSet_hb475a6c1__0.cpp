// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_usb_midi_enum.h for the primary calling header

#include "Vtb_usb_midi_enum__pch.h"
#include "Vtb_usb_midi_enum___024root.h"

VL_INLINE_OPT void Vtb_usb_midi_enum___024root___ico_sequent__TOP__0(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___ico_sequent__TOP__0\n"); );
    // Body
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rst 
        = (1U & (~ (IData)(vlSelf->resetn)));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rst 
        = (1U & (~ (IData)(vlSelf->resetn)));
    vlSelf->tb_usb_midi_enum__DOT__resetn = vlSelf->resetn;
    vlSelf->tb_usb_midi_enum__DOT__clk = vlSelf->clk;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__resetn 
        = vlSelf->tb_usb_midi_enum__DOT__resetn;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__clk 
        = vlSelf->tb_usb_midi_enum__DOT__clk;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__clk = vlSelf->tb_usb_midi_enum__DOT__clk;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__clk 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__clk;
}

void Vtb_usb_midi_enum___024root___eval_ico(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_ico\n"); );
    // Body
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        Vtb_usb_midi_enum___024root___ico_sequent__TOP__0(vlSelf);
    }
}

void Vtb_usb_midi_enum___024root___eval_triggers__ico(Vtb_usb_midi_enum___024root* vlSelf);

bool Vtb_usb_midi_enum___024root___eval_phase__ico(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_phase__ico\n"); );
    // Init
    CData/*0:0*/ __VicoExecute;
    // Body
    Vtb_usb_midi_enum___024root___eval_triggers__ico(vlSelf);
    __VicoExecute = vlSelf->__VicoTriggered.any();
    if (__VicoExecute) {
        Vtb_usb_midi_enum___024root___eval_ico(vlSelf);
    }
    return (__VicoExecute);
}

void Vtb_usb_midi_enum___024root___eval_act(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_act\n"); );
}

extern const VlUnpacked<CData/*0:0*/, 64> Vtb_usb_midi_enum__ConstPool__TABLE_h367ab1b2_0;
extern const VlUnpacked<CData/*3:0*/, 64> Vtb_usb_midi_enum__ConstPool__TABLE_h4389ffb3_0;
extern const VlUnpacked<CData/*7:0*/, 64> Vtb_usb_midi_enum__ConstPool__TABLE_hcf4b28ba_0;
extern const VlUnpacked<CData/*3:0*/, 64> Vtb_usb_midi_enum__ConstPool__TABLE_he5ce485c_0;
extern const VlUnpacked<CData/*3:0*/, 64> Vtb_usb_midi_enum__ConstPool__TABLE_h80bc0b65_0;

VL_INLINE_OPT void Vtb_usb_midi_enum___024root___nba_sequent__TOP__0(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout = 0;
    IData/*31:0*/ __Vilp;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos = 0;
    CData/*4:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__Vfuncout = 0;
    CData/*4:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__c;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__b;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__c;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__b;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos = 0;
    CData/*4:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__Vfuncout = 0;
    CData/*4:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__c;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__b;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__c;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__b;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__c;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__b;
    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos = 0;
    CData/*4:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__Vfuncout = 0;
    CData/*4:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__c;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__b;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__c;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__b;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos = 0;
    CData/*4:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__Vfuncout = 0;
    CData/*4:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__c;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__b;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__c;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__b;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__b = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout = 0;
    CData/*6:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__Vfuncout;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__Vfuncout = 0;
    SData/*15:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__c;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__c = 0;
    CData/*0:0*/ __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__b;
    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__b = 0;
    CData/*5:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*4:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0;
    IData/*31:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__data_toggle;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__data_toggle = 0;
    CData/*5:0*/ __Vdlyvdim0__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0;
    __Vdlyvdim0__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0 = 0;
    CData/*7:0*/ __Vdlyvval__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0;
    __Vdlyvval__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0 = 0;
    CData/*0:0*/ __Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0;
    __Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0 = 0;
    CData/*6:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tok_addr;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tok_addr = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__ctl_which;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__ctl_which = 0;
    CData/*3:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt = 0;
    CData/*0:0*/ __Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v1;
    __Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v1 = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tx_done;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tx_done = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run = 0;
    SData/*15:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j = 0;
    CData/*3:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err = 0;
    CData/*1:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n = 0;
    CData/*7:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes = 0;
    SData/*15:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc = 0;
    CData/*3:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt = 0;
    CData/*7:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt = 0;
    CData/*7:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift = 0;
    CData/*7:0*/ __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0 = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__ones_run;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__ones_run = 0;
    SData/*15:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__crc_run;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__crc_run = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j = 0;
    CData/*3:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run = 0;
    CData/*0:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err = 0;
    CData/*1:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n = 0;
    CData/*7:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes = 0;
    SData/*15:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc = 0;
    CData/*3:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt = 0;
    CData/*7:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift = 0;
    CData/*2:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt = 0;
    CData/*7:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift = 0;
    CData/*7:0*/ __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0 = 0;
    SData/*31:0*/ __Vtemp_8;
    SData/*31:0*/ __Vtemp_15;
    // Body
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rxstate;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__crc_run 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_run;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__ones_run 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__ones_run;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate;
    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_j;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__ctl_which 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__ctl_which;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tok_addr 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tok_addr;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__data_toggle 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__data_toggle;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state;
    __Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0 = 0U;
    __Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v1 = 0U;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_enable;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate;
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j;
    if ((1U & (~ (IData)(vlSelf->resetn)))) {
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 1U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 2U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 3U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 4U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 5U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 6U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 7U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 8U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 9U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0xaU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0xbU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0xcU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0xdU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0xeU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0xfU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x10U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x11U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x12U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x13U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x14U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x15U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x16U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x17U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x18U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x19U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x1aU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x1bU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x1cU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x1dU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x1eU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x1fU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x20U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x21U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x22U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x23U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x24U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x25U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x26U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x27U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x28U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x29U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x2aU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x2bU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x2cU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x2dU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x2eU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x2fU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x30U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x31U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x32U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x33U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x34U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x35U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x36U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x37U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x38U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x39U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x3aU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x3bU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x3cU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x3dU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x3eU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x3fU;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = 0x40U;
    }
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_sync 
        = ((2U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_sync) 
                  << 1U)) | (IData)(vlSelf->tb_usb_midi_enum__DOT__usb_dm));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_sync 
        = ((2U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_sync) 
                  << 1U)) | (IData)(vlSelf->tb_usb_midi_enum__DOT__usb_dp));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_sync 
        = ((2U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_sync) 
                  << 1U)) | (IData)(vlSelf->tb_usb_midi_enum__DOT__usb_dm));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_sync 
        = ((2U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_sync) 
                  << 1U)) | (IData)(vlSelf->tb_usb_midi_enum__DOT__usb_dp));
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_done = 0U;
    if (vlSelf->resetn) {
        if ((4U & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate))) {
            __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate = 0U;
        } else if ((2U & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate))) {
            if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate))) {
                if ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk))) {
                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__oe = 0U;
                    vlSelf->tb_usb_midi_enum__DOT__dev_tx_done = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate = 0U;
                } else {
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk 
                        = (7U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk)));
                }
            } else if ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk))) {
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk = 0U;
                if ((1U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_bitclk_cnt))) {
                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_se0 = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate = 3U;
                } else {
                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_bitclk_cnt 
                        = (3U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_bitclk_cnt)));
                }
            } else {
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk 
                    = (7U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk)));
            }
        } else if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate))) {
            if ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk))) {
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk = 0U;
                if (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos) 
                     == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_total))) {
                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_bitclk_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate = 2U;
                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_se0 = 1U;
                } else if ((6U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__ones_run))) {
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j 
                        = (1U & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_j)));
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__ones_run = 0U;
                } else {
                    if (([&]() {
                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos 
                                    = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos;
                                if ((8U > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos))) {
                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout 
                                        = (7U == (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos));
                                } else if ((0x10U > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos))) {
                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                        = (0x7fU & 
                                           ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos) 
                                            - (IData)(8U)));
                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout 
                                        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid_lat) 
                                                 >> 
                                                 (7U 
                                                  & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))));
                                } else if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat) {
                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                        = (0x7fU & 
                                           ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos) 
                                            - (IData)(0x10U)));
                                    if ((7U > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout 
                                            = ((6U 
                                                >= 
                                                (7U 
                                                 & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) 
                                               && (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr_lat) 
                                                      >> 
                                                      (7U 
                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                    } else if ((0xbU 
                                                > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout 
                                            = (1U & 
                                               ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp_lat) 
                                                >> 
                                                (3U 
                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                    - (IData)(3U)))));
                                    } else {
                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                            = (0xfU 
                                               & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                  - (IData)(0xbU)));
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout 
                                            = (1U & 
                                               ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                    }
                                } else {
                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                        = (0x7fU & 
                                           ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__pos) 
                                            - (IData)(0x10U)));
                                    if (((0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat)) 
                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                            < (0x7fU 
                                               & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout 
                                            = (1U & (IData)(
                                                            (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload_lat 
                                                             >> 
                                                             (0x3fU 
                                                              & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                    } else {
                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                            = (0xfU 
                                               & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                  - 
                                                  (0x7fU 
                                                   & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))));
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout 
                                            = (1U & 
                                               ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                    }
                                }
                            }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__10__Vfuncout))) {
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__ones_run 
                            = (7U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__ones_run)));
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j 
                            = (1U & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_j)));
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__ones_run = 0U;
                    }
                    if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_crc_covered) {
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__crc_run 
                            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat)
                                ? VL_EXTEND_II(16,5, 
                                               ([&]() {
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos 
                                            = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos;
                                        if ((8U > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout 
                                                = (7U 
                                                   == (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos));
                                        } else if (
                                                   (0x10U 
                                                    > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout 
                                                = (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid_lat) 
                                                      >> 
                                                      (7U 
                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))));
                                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                = (0x7fU 
                                                   & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos) 
                                                      - (IData)(8U)));
                                        } else if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat) {
                                            if ((7U 
                                                 > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout 
                                                    = 
                                                    ((6U 
                                                      >= 
                                                      (7U 
                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) 
                                                     && (1U 
                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr_lat) 
                                                            >> 
                                                            (7U 
                                                             & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                            } else if (
                                                       (0xbU 
                                                        > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout 
                                                    = 
                                                    (1U 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp_lat) 
                                                        >> 
                                                        (3U 
                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                            - (IData)(3U)))));
                                            } else {
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout 
                                                    = 
                                                    (1U 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                        >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                                    = 
                                                    (0xfU 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                        - (IData)(0xbU)));
                                            }
                                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                = (0x7fU 
                                                   & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos) 
                                                      - (IData)(0x10U)));
                                        } else {
                                            if (((0U 
                                                  != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat)) 
                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                    < 
                                                    (0x7fU 
                                                     & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))))) {
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout 
                                                    = 
                                                    (1U 
                                                     & (IData)(
                                                               (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload_lat 
                                                                >> 
                                                                (0x3fU 
                                                                 & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                            } else {
                                                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                                    = 
                                                    (0xfU 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                        - 
                                                        (0x7fU 
                                                         & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))));
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout 
                                                    = 
                                                    (1U 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                        >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                            }
                                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                = (0x7fU 
                                                   & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__pos) 
                                                      - (IData)(0x10U)));
                                        }
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__b 
                                            = __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__12__Vfuncout;
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__c 
                                            = (0x1fU 
                                               & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_run));
                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__Vstatic__t 
                                            = (1U & 
                                               ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__c) 
                                                ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__b)));
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__Vfuncout 
                                            = (0x1fU 
                                               & VL_SHIFTR_III(5,5,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__c), 1U));
                                        if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__Vstatic__t) {
                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__Vfuncout 
                                                = (0x14U 
                                                   ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__Vfuncout));
                                        }
                                    }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__11__Vfuncout)))
                                : ([&]() {
                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos 
                                        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos;
                                    if ((8U > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout 
                                            = (7U == (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos));
                                    } else if ((0x10U 
                                                > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout 
                                            = (1U & 
                                               ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid_lat) 
                                                >> 
                                                (7U 
                                                 & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))));
                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                            = (0x7fU 
                                               & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos) 
                                                  - (IData)(8U)));
                                    } else if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat) {
                                        if ((7U > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout 
                                                = (
                                                   (6U 
                                                    >= 
                                                    (7U 
                                                     & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) 
                                                   && (1U 
                                                       & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr_lat) 
                                                          >> 
                                                          (7U 
                                                           & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                        } else if (
                                                   (0xbU 
                                                    > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout 
                                                = (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp_lat) 
                                                      >> 
                                                      (3U 
                                                       & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                          - (IData)(3U)))));
                                        } else {
                                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                                = (0xfU 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                      - (IData)(0xbU)));
                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout 
                                                = (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                      >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                        }
                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                            = (0x7fU 
                                               & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos) 
                                                  - (IData)(0x10U)));
                                    } else {
                                        if (((0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat)) 
                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                < (0x7fU 
                                                   & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout 
                                                = (1U 
                                                   & (IData)(
                                                             (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload_lat 
                                                              >> 
                                                              (0x3fU 
                                                               & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                        } else {
                                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                                = (0xfU 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                      - 
                                                      (0x7fU 
                                                       & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))));
                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout 
                                                = (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                      >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                        }
                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                            = (0x7fU 
                                               & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__pos) 
                                                  - (IData)(0x10U)));
                                    }
                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__b 
                                        = __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__14__Vfuncout;
                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__c 
                                        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_run;
                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__Vstatic__t 
                                        = (1U & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__c) 
                                                 ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__b)));
                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__Vfuncout 
                                        = (0xffffU 
                                           & VL_SHIFTR_III(16,16,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__c), 1U));
                                    if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__Vstatic__t) {
                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__Vfuncout 
                                            = (0xa001U 
                                               ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__Vfuncout));
                                    }
                                }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__13__Vfuncout)));
                        if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_crc_last_bit) {
                            __Vtemp_8 = (0xffffU & 
                                         ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat)
                                           ? VL_EXTEND_II(16,5, 
                                                          (0x1fU 
                                                           & (~ 
                                                              ([&]() {
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos 
                                                            = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos;
                                                        if (
                                                            (8U 
                                                             > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos))) {
                                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout 
                                                                = 
                                                                (7U 
                                                                 == (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos));
                                                        } else if (
                                                                   (0x10U 
                                                                    > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos))) {
                                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout 
                                                                = 
                                                                (1U 
                                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid_lat) 
                                                                    >> 
                                                                    (7U 
                                                                     & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))));
                                                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                                = 
                                                                (0x7fU 
                                                                 & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos) 
                                                                    - (IData)(8U)));
                                                        } else if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat) {
                                                            if (
                                                                (7U 
                                                                 > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout 
                                                                    = 
                                                                    ((6U 
                                                                      >= 
                                                                      (7U 
                                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) 
                                                                     && (1U 
                                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr_lat) 
                                                                            >> 
                                                                            (7U 
                                                                             & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                                            } else if (
                                                                       (0xbU 
                                                                        > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout 
                                                                    = 
                                                                    (1U 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp_lat) 
                                                                        >> 
                                                                        (3U 
                                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                                            - (IData)(3U)))));
                                                            } else {
                                                                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                                                    = 
                                                                    (0xfU 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                                        - (IData)(0xbU)));
                                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout 
                                                                    = 
                                                                    (1U 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                                        >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                            }
                                                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                                = 
                                                                (0x7fU 
                                                                 & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos) 
                                                                    - (IData)(0x10U)));
                                                        } else {
                                                            if (
                                                                ((0U 
                                                                  != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat)) 
                                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                                    < 
                                                                    (0x7fU 
                                                                     & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))))) {
                                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout 
                                                                    = 
                                                                    (1U 
                                                                     & (IData)(
                                                                               (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload_lat 
                                                                                >> 
                                                                                (0x3fU 
                                                                                & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                                            } else {
                                                                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                                                    = 
                                                                    (0xfU 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                                        - 
                                                                        (0x7fU 
                                                                         & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))));
                                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout 
                                                                    = 
                                                                    (1U 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                                        >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                            }
                                                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                                = 
                                                                (0x7fU 
                                                                 & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__pos) 
                                                                    - (IData)(0x10U)));
                                                        }
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__b 
                                                            = __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__16__Vfuncout;
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__c 
                                                            = 
                                                            (0x1fU 
                                                             & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_run));
                                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__Vstatic__t 
                                                            = 
                                                            (1U 
                                                             & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__c) 
                                                                ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__b)));
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__Vfuncout 
                                                            = 
                                                            (0x1fU 
                                                             & VL_SHIFTR_III(5,5,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__c), 1U));
                                                        if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__Vstatic__t) {
                                                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__Vfuncout 
                                                                = 
                                                                (0x14U 
                                                                 ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__Vfuncout));
                                                        }
                                                    }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__15__Vfuncout)))))
                                           : (~ ([&]() {
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos 
                                                    = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos;
                                                if (
                                                    (8U 
                                                     > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos))) {
                                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout 
                                                        = 
                                                        (7U 
                                                         == (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos));
                                                } else if (
                                                           (0x10U 
                                                            > (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos))) {
                                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout 
                                                        = 
                                                        (1U 
                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid_lat) 
                                                            >> 
                                                            (7U 
                                                             & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))));
                                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                        = 
                                                        (0x7fU 
                                                         & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos) 
                                                            - (IData)(8U)));
                                                } else if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat) {
                                                    if (
                                                        (7U 
                                                         > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout 
                                                            = 
                                                            ((6U 
                                                              >= 
                                                              (7U 
                                                               & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) 
                                                             && (1U 
                                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr_lat) 
                                                                    >> 
                                                                    (7U 
                                                                     & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                                    } else if (
                                                               (0xbU 
                                                                > (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p))) {
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout 
                                                            = 
                                                            (1U 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp_lat) 
                                                                >> 
                                                                (3U 
                                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                                    - (IData)(3U)))));
                                                    } else {
                                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                                            = 
                                                            (0xfU 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                                - (IData)(0xbU)));
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout 
                                                            = 
                                                            (1U 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                                >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                    }
                                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                        = 
                                                        (0x7fU 
                                                         & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos) 
                                                            - (IData)(0x10U)));
                                                } else {
                                                    if (
                                                        ((0U 
                                                          != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat)) 
                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                            < 
                                                            (0x7fU 
                                                             & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))))) {
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout 
                                                            = 
                                                            (1U 
                                                             & (IData)(
                                                                       (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload_lat 
                                                                        >> 
                                                                        (0x3fU 
                                                                         & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p)))));
                                                    } else {
                                                        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx 
                                                            = 
                                                            (0xfU 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p) 
                                                                - 
                                                                (0x7fU 
                                                                 & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U))));
                                                        __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout 
                                                            = 
                                                            (1U 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final) 
                                                                >> (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                    }
                                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p 
                                                        = 
                                                        (0x7fU 
                                                         & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__pos) 
                                                            - (IData)(0x10U)));
                                                }
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__b 
                                                    = __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__18__Vfuncout;
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__c 
                                                    = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_run;
                                                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__Vstatic__t 
                                                    = 
                                                    (1U 
                                                     & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__c) 
                                                        ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__b)));
                                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__Vfuncout 
                                                    = 
                                                    (0xffffU 
                                                     & VL_SHIFTR_III(16,16,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__c), 1U));
                                                if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__Vstatic__t) {
                                                    __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__Vfuncout 
                                                        = 
                                                        (0xa001U 
                                                         ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__Vfuncout));
                                                }
                                            }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__17__Vfuncout)))));
                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final 
                                = __Vtemp_8;
                        }
                    }
                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos 
                        = (0x7fU & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos)));
                }
            } else {
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk 
                    = (7U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk)));
            }
        } else {
            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__oe = 0U;
            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_se0 = 0U;
            if (vlSelf->tb_usb_midi_enum__DOT__dev_tx_start) {
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid_lat 
                    = vlSelf->tb_usb_midi_enum__DOT__dev_tx_pid;
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat = 0U;
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr_lat = 0U;
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp_lat = 0U;
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload_lat = 0ULL;
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat 
                    = vlSelf->tb_usb_midi_enum__DOT__dev_tx_len;
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_total 
                    = (0x7fU & ((IData)(0x20U) + VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dev_tx_len), 3U)));
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos = 0U;
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__ones_run = 0U;
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__crc_run = 0xffffU;
                if ((0U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dev_tx_len))) {
                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final = 0U;
                }
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk = 0U;
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__oe = 1U;
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j = 1U;
                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_se0 = 0U;
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate = 1U;
            }
        }
    } else {
        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate = 0U;
        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__oe = 0U;
        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j = 1U;
        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_se0 = 0U;
    }
    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tx_done = 0U;
    if (vlSelf->resetn) {
        if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__force_se0) {
            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = 0U;
            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe = 1U;
            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0 = 1U;
        } else if ((4U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate))) {
            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = 0U;
        } else if ((2U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate))) {
            if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate))) {
                if ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk))) {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tx_done = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = 0U;
                } else {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk 
                        = (7U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk)));
                }
            } else if ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk))) {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk = 0U;
                if ((1U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_bitclk_cnt))) {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0 = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = 3U;
                } else {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_bitclk_cnt 
                        = (3U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_bitclk_cnt)));
                }
            } else {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk 
                    = (7U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk)));
            }
        } else if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate))) {
            if ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk))) {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk = 0U;
                if (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos) 
                     == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_total))) {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_bitclk_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = 2U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0 = 1U;
                } else if ((6U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run))) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j 
                        = (1U & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j)));
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run = 0U;
                } else {
                    if (([&]() {
                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos 
                                    = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos;
                                if ((8U > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos))) {
                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout 
                                        = (7U == (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos));
                                } else if ((0x10U > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos))) {
                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                        = (0x7fU & 
                                           ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos) 
                                            - (IData)(8U)));
                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout 
                                        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid_lat) 
                                                 >> 
                                                 (7U 
                                                  & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))));
                                } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat) {
                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                        = (0x7fU & 
                                           ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos) 
                                            - (IData)(0x10U)));
                                    if ((7U > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout 
                                            = ((6U 
                                                >= 
                                                (7U 
                                                 & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) 
                                               && (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr_lat) 
                                                      >> 
                                                      (7U 
                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                    } else if ((0xbU 
                                                > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout 
                                            = (1U & 
                                               ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp_lat) 
                                                >> 
                                                (3U 
                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                    - (IData)(3U)))));
                                    } else {
                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                            = (0xfU 
                                               & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                  - (IData)(0xbU)));
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout 
                                            = (1U & 
                                               ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                    }
                                } else {
                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                        = (0x7fU & 
                                           ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__pos) 
                                            - (IData)(0x10U)));
                                    if (((0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat)) 
                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                            < (0x7fU 
                                               & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout 
                                            = (1U & (IData)(
                                                            (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload_lat 
                                                             >> 
                                                             (0x3fU 
                                                              & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                    } else {
                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                            = (0xfU 
                                               & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                  - 
                                                  (0x7fU 
                                                   & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))));
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout 
                                            = (1U & 
                                               ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                    }
                                }
                            }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__0__Vfuncout))) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run 
                            = (7U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run)));
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j 
                            = (1U & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j)));
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run = 0U;
                    }
                    if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_crc_covered) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run 
                            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat)
                                ? VL_EXTEND_II(16,5, 
                                               ([&]() {
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos 
                                            = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos;
                                        if ((8U > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout 
                                                = (7U 
                                                   == (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos));
                                        } else if (
                                                   (0x10U 
                                                    > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout 
                                                = (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid_lat) 
                                                      >> 
                                                      (7U 
                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))));
                                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                = (0x7fU 
                                                   & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos) 
                                                      - (IData)(8U)));
                                        } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat) {
                                            if ((7U 
                                                 > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout 
                                                    = 
                                                    ((6U 
                                                      >= 
                                                      (7U 
                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) 
                                                     && (1U 
                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr_lat) 
                                                            >> 
                                                            (7U 
                                                             & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                            } else if (
                                                       (0xbU 
                                                        > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout 
                                                    = 
                                                    (1U 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp_lat) 
                                                        >> 
                                                        (3U 
                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                            - (IData)(3U)))));
                                            } else {
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout 
                                                    = 
                                                    (1U 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                        >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                                    = 
                                                    (0xfU 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                        - (IData)(0xbU)));
                                            }
                                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                = (0x7fU 
                                                   & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos) 
                                                      - (IData)(0x10U)));
                                        } else {
                                            if (((0U 
                                                  != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat)) 
                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                    < 
                                                    (0x7fU 
                                                     & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))))) {
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout 
                                                    = 
                                                    (1U 
                                                     & (IData)(
                                                               (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload_lat 
                                                                >> 
                                                                (0x3fU 
                                                                 & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                            } else {
                                                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                                    = 
                                                    (0xfU 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                        - 
                                                        (0x7fU 
                                                         & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))));
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout 
                                                    = 
                                                    (1U 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                        >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                            }
                                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                = (0x7fU 
                                                   & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__pos) 
                                                      - (IData)(0x10U)));
                                        }
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__b 
                                            = __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__2__Vfuncout;
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__c 
                                            = (0x1fU 
                                               & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run));
                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__Vstatic__t 
                                            = (1U & 
                                               ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__c) 
                                                ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__b)));
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__Vfuncout 
                                            = (0x1fU 
                                               & VL_SHIFTR_III(5,5,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__c), 1U));
                                        if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__Vstatic__t) {
                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__Vfuncout 
                                                = (0x14U 
                                                   ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__Vfuncout));
                                        }
                                    }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__1__Vfuncout)))
                                : ([&]() {
                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos 
                                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos;
                                    if ((8U > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout 
                                            = (7U == (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos));
                                    } else if ((0x10U 
                                                > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos))) {
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout 
                                            = (1U & 
                                               ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid_lat) 
                                                >> 
                                                (7U 
                                                 & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))));
                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                            = (0x7fU 
                                               & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos) 
                                                  - (IData)(8U)));
                                    } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat) {
                                        if ((7U > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout 
                                                = (
                                                   (6U 
                                                    >= 
                                                    (7U 
                                                     & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) 
                                                   && (1U 
                                                       & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr_lat) 
                                                          >> 
                                                          (7U 
                                                           & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                        } else if (
                                                   (0xbU 
                                                    > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout 
                                                = (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp_lat) 
                                                      >> 
                                                      (3U 
                                                       & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                          - (IData)(3U)))));
                                        } else {
                                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                                = (0xfU 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                      - (IData)(0xbU)));
                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout 
                                                = (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                      >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                        }
                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                            = (0x7fU 
                                               & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos) 
                                                  - (IData)(0x10U)));
                                    } else {
                                        if (((0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat)) 
                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                < (0x7fU 
                                                   & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))))) {
                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout 
                                                = (1U 
                                                   & (IData)(
                                                             (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload_lat 
                                                              >> 
                                                              (0x3fU 
                                                               & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                        } else {
                                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                                = (0xfU 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                      - 
                                                      (0x7fU 
                                                       & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))));
                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout 
                                                = (1U 
                                                   & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                      >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                        }
                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                            = (0x7fU 
                                               & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__pos) 
                                                  - (IData)(0x10U)));
                                    }
                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__b 
                                        = __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__4__Vfuncout;
                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__c 
                                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run;
                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__Vstatic__t 
                                        = (1U & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__c) 
                                                 ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__b)));
                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__Vfuncout 
                                        = (0xffffU 
                                           & VL_SHIFTR_III(16,16,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__c), 1U));
                                    if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__Vstatic__t) {
                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__Vfuncout 
                                            = (0xa001U 
                                               ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__Vfuncout));
                                    }
                                }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__3__Vfuncout)));
                        if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_crc_last_bit) {
                            __Vtemp_15 = (0xffffU & 
                                          ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat)
                                            ? VL_EXTEND_II(16,5, 
                                                           (0x1fU 
                                                            & (~ 
                                                               ([&]() {
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos 
                                                            = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos;
                                                        if (
                                                            (8U 
                                                             > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos))) {
                                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout 
                                                                = 
                                                                (7U 
                                                                 == (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos));
                                                        } else if (
                                                                   (0x10U 
                                                                    > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos))) {
                                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout 
                                                                = 
                                                                (1U 
                                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid_lat) 
                                                                    >> 
                                                                    (7U 
                                                                     & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))));
                                                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                                = 
                                                                (0x7fU 
                                                                 & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos) 
                                                                    - (IData)(8U)));
                                                        } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat) {
                                                            if (
                                                                (7U 
                                                                 > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout 
                                                                    = 
                                                                    ((6U 
                                                                      >= 
                                                                      (7U 
                                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) 
                                                                     && (1U 
                                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr_lat) 
                                                                            >> 
                                                                            (7U 
                                                                             & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                                            } else if (
                                                                       (0xbU 
                                                                        > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout 
                                                                    = 
                                                                    (1U 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp_lat) 
                                                                        >> 
                                                                        (3U 
                                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                                            - (IData)(3U)))));
                                                            } else {
                                                                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                                                    = 
                                                                    (0xfU 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                                        - (IData)(0xbU)));
                                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout 
                                                                    = 
                                                                    (1U 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                                        >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                            }
                                                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                                = 
                                                                (0x7fU 
                                                                 & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos) 
                                                                    - (IData)(0x10U)));
                                                        } else {
                                                            if (
                                                                ((0U 
                                                                  != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat)) 
                                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                                    < 
                                                                    (0x7fU 
                                                                     & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))))) {
                                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout 
                                                                    = 
                                                                    (1U 
                                                                     & (IData)(
                                                                               (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload_lat 
                                                                                >> 
                                                                                (0x3fU 
                                                                                & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                                            } else {
                                                                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                                                    = 
                                                                    (0xfU 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                                        - 
                                                                        (0x7fU 
                                                                         & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))));
                                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout 
                                                                    = 
                                                                    (1U 
                                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                                        >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                            }
                                                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                                = 
                                                                (0x7fU 
                                                                 & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__pos) 
                                                                    - (IData)(0x10U)));
                                                        }
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__b 
                                                            = __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__6__Vfuncout;
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__c 
                                                            = 
                                                            (0x1fU 
                                                             & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run));
                                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__Vstatic__t 
                                                            = 
                                                            (1U 
                                                             & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__c) 
                                                                ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__b)));
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__Vfuncout 
                                                            = 
                                                            (0x1fU 
                                                             & VL_SHIFTR_III(5,5,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__c), 1U));
                                                        if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__Vstatic__t) {
                                                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__Vfuncout 
                                                                = 
                                                                (0x14U 
                                                                 ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__Vfuncout));
                                                        }
                                                    }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__5__Vfuncout)))))
                                            : (~ ([&]() {
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos 
                                                    = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos;
                                                if (
                                                    (8U 
                                                     > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos))) {
                                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout 
                                                        = 
                                                        (7U 
                                                         == (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos));
                                                } else if (
                                                           (0x10U 
                                                            > (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos))) {
                                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout 
                                                        = 
                                                        (1U 
                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid_lat) 
                                                            >> 
                                                            (7U 
                                                             & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))));
                                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                        = 
                                                        (0x7fU 
                                                         & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos) 
                                                            - (IData)(8U)));
                                                } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat) {
                                                    if (
                                                        (7U 
                                                         > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout 
                                                            = 
                                                            ((6U 
                                                              >= 
                                                              (7U 
                                                               & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) 
                                                             && (1U 
                                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr_lat) 
                                                                    >> 
                                                                    (7U 
                                                                     & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                                    } else if (
                                                               (0xbU 
                                                                > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p))) {
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout 
                                                            = 
                                                            (1U 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp_lat) 
                                                                >> 
                                                                (3U 
                                                                 & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                                    - (IData)(3U)))));
                                                    } else {
                                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                                            = 
                                                            (0xfU 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                                - (IData)(0xbU)));
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout 
                                                            = 
                                                            (1U 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                                >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                    }
                                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                        = 
                                                        (0x7fU 
                                                         & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos) 
                                                            - (IData)(0x10U)));
                                                } else {
                                                    if (
                                                        ((0U 
                                                          != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat)) 
                                                         & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                            < 
                                                            (0x7fU 
                                                             & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))))) {
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout 
                                                            = 
                                                            (1U 
                                                             & (IData)(
                                                                       (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload_lat 
                                                                        >> 
                                                                        (0x3fU 
                                                                         & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p)))));
                                                    } else {
                                                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx 
                                                            = 
                                                            (0xfU 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p) 
                                                                - 
                                                                (0x7fU 
                                                                 & VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U))));
                                                        __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout 
                                                            = 
                                                            (1U 
                                                             & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final) 
                                                                >> (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx)));
                                                    }
                                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p 
                                                        = 
                                                        (0x7fU 
                                                         & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__pos) 
                                                            - (IData)(0x10U)));
                                                }
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__b 
                                                    = __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__8__Vfuncout;
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__c 
                                                    = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run;
                                                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__Vstatic__t 
                                                    = 
                                                    (1U 
                                                     & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__c) 
                                                        ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__b)));
                                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__Vfuncout 
                                                    = 
                                                    (0xffffU 
                                                     & VL_SHIFTR_III(16,16,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__c), 1U));
                                                if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__Vstatic__t) {
                                                    __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__Vfuncout 
                                                        = 
                                                        (0xa001U 
                                                         ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__Vfuncout));
                                                }
                                            }(), (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__7__Vfuncout)))));
                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final 
                                = __Vtemp_15;
                        }
                    }
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos 
                        = (0x7fU & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos)));
                }
            } else {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk 
                    = (7U & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk)));
            }
        } else {
            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe = 0U;
            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0 = 0U;
            if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start) {
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid_lat 
                    = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid;
                if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token) {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat = 1U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr_lat 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_addr_r;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp_lat 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_endp_r;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload_lat 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_payload;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_total = 0x28U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run = 0x1fU;
                } else {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat = 0U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr_lat 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_addr_r;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp_lat 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_endp_r;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload_lat 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_payload;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_total 
                        = (0x7fU & ((IData)(0x20U) 
                                    + VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len), 3U)));
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run = 0xffffU;
                }
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run = 0U;
                if (((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token)) 
                     & (0U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len)))) {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final = 0U;
                }
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk = 0U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe = 1U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j = 1U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0 = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = 1U;
            }
        }
    } else {
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = 0U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe = 0U;
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j = 1U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0 = 0U;
    }
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__ones_run 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__ones_run;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_run 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__crc_run;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__txstate;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_j 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__drive_j;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s 
        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_sync) 
                 >> 1U));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s 
        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_sync) 
                 >> 1U));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s 
        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_sync) 
                 >> 1U));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s 
        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_sync) 
                 >> 1U));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_done;
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_busy = (0U 
                                                  != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT____VdfgTmp_h5d27979b__0 
        = ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos) 
           >= (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_start));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_data 
        = (0x7fU & (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_start) 
                     + VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U)) 
                    - (IData)(1U)));
    __Vtableidx1 = (((IData)(vlSelf->tb_usb_midi_enum__DOT__pkt_count) 
                     << 2U) | (((IData)(vlSelf->tb_usb_midi_enum__DOT__dev_rx_done) 
                                << 1U) | (IData)(vlSelf->resetn)));
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_start = Vtb_usb_midi_enum__ConstPool__TABLE_h367ab1b2_0
        [__Vtableidx1];
    if ((2U & Vtb_usb_midi_enum__ConstPool__TABLE_h4389ffb3_0
         [__Vtableidx1])) {
        vlSelf->tb_usb_midi_enum__DOT__dev_tx_pid = 
            Vtb_usb_midi_enum__ConstPool__TABLE_hcf4b28ba_0
            [__Vtableidx1];
    }
    if ((4U & Vtb_usb_midi_enum__ConstPool__TABLE_h4389ffb3_0
         [__Vtableidx1])) {
        vlSelf->tb_usb_midi_enum__DOT__dev_tx_len = 
            Vtb_usb_midi_enum__ConstPool__TABLE_he5ce485c_0
            [__Vtableidx1];
    }
    if ((8U & Vtb_usb_midi_enum__ConstPool__TABLE_h4389ffb3_0
         [__Vtableidx1])) {
        vlSelf->tb_usb_midi_enum__DOT__pkt_count = 
            Vtb_usb_midi_enum__ConstPool__TABLE_h80bc0b65_0
            [__Vtableidx1];
    }
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_busy 
        = (0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT____VdfgTmp_h5d27979b__0 
        = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos) 
           >= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_start));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_data 
        = (0x7fU & (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_start) 
                     + VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U)) 
                    - (IData)(1U)));
    vlSelf->tb_usb_midi_enum__DOT__usb_dp = (1U & (
                                                   (~ 
                                                    ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__oe) 
                                                     | (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe))) 
                                                   | ((((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe) 
                                                        & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0))) 
                                                       & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j)) 
                                                      | (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__oe) 
                                                          & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_se0))) 
                                                         & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_j)))));
    vlSelf->tb_usb_midi_enum__DOT__usb_dm = ((((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe) 
                                               & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0))) 
                                              & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j))) 
                                             | (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__oe) 
                                                 & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_se0))) 
                                                & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_j))));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_valid = 0U;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start = 0U;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__force_se0 = 0U;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_se0 
        = (1U & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s)) 
                 & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s))));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_se0 
        = (1U & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s)) 
                 & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s))));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_busy 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_busy;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_busy 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_busy;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__usb_dp 
        = vlSelf->tb_usb_midi_enum__DOT__usb_dp;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dp 
        = vlSelf->tb_usb_midi_enum__DOT__usb_dp;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__usb_dm 
        = vlSelf->tb_usb_midi_enum__DOT__usb_dm;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dm 
        = vlSelf->tb_usb_midi_enum__DOT__usb_dm;
    if (vlSelf->resetn) {
        if ((0x10U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
            if ((8U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0U;
            } else if ((4U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                if ((2U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0U;
                } else if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
                } else if ((0x64U <= vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt)) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0xeU;
                } else {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
                        = ((IData)(1U) + vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt);
                }
            } else if ((2U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                    if (((0x7fU & ((IData)(4U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx))) 
                         <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf_cnt))) {
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_valid = 1U;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_cin_cable 
                            = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf
                            [(0x3fU & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx))];
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_b0 
                            = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf
                            [vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p1];
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_b1 
                            = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf
                            [vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p2];
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_b2 
                            = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf
                            [vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p3];
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x13U;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx 
                            = (0x7fU & ((IData)(4U) 
                                        + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx)));
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x14U;
                    }
                } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__data_toggle 
                        = (1U & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__data_toggle)));
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x13U;
                }
            } else if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start = 1U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid = 0xd2U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token = 0U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x12U;
            } else {
                if (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte_valid) 
                     & (0x40U > (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf_cnt)))) {
                    __Vdlyvval__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte;
                    __Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0 = 1U;
                    __Vdlyvdim0__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0 
                        = (0x3fU & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf_cnt));
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf_cnt 
                        = (0x7fU & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf_cnt)));
                }
                if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
                    if (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok) 
                         & (0x5aU == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid)))) {
                        vlSelf->nak_count = (0xffffU 
                                             & ((IData)(1U) 
                                                + (IData)(vlSelf->nak_count)));
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x14U;
                    } else if (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok) 
                                & (0x1eU == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid)))) {
                        vlSelf->dev_error = 1U;
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x15U;
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state 
                            = (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok) 
                                & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid) 
                                   == ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__data_toggle)
                                        ? 0x4bU : 0xc3U)))
                                ? 0x11U : 0x14U);
                    }
                } else if ((0xfa0U <= vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt)) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x14U;
                } else {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
                        = ((IData)(1U) + vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt);
                }
            }
        } else if ((8U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
            if ((4U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                if ((2U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                    if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                        if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done) {
                            vlSelf->poll_count = (0xffffU 
                                                  & ((IData)(1U) 
                                                     + (IData)(vlSelf->poll_count)));
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 1U;
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x10U;
                        }
                    } else {
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf_cnt = 0U;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start = 1U;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid = 0x69U;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token = 1U;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_addr_r 
                            = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tok_addr;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_endp_r = 1U;
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0xfU;
                    }
                } else if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                    if ((0x3e8U <= vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt)) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
                        if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__ctl_which) {
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__data_toggle = 0U;
                            vlSelf->dev_configured = 1U;
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0xeU;
                        } else {
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tok_addr = 1U;
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__ctl_which = 1U;
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt = 0U;
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 2U;
                        }
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
                            = ((IData)(1U) + vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt);
                    }
                } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0xdU;
                }
            } else if ((2U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start = 1U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid = 0xd2U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token = 0U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0xcU;
                } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
                    if (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok) 
                         & (0x4bU == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid)))) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0xbU;
                    } else if (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok) 
                                & (0x5aU == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid)))) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 8U;
                    } else if ((8U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt))) {
                        vlSelf->dev_error = 1U;
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x15U;
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt)));
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 2U;
                    }
                } else if ((0xfa0U <= vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt)) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 8U;
                } else {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
                        = ((IData)(1U) + vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt);
                }
            } else if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0xaU;
                }
            } else {
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start = 1U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid = 0x69U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token = 1U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_addr_r 
                    = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tok_addr;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_endp_r = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 9U;
            }
        } else if ((4U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
            if ((2U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                    if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
                        if (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok) 
                             & (0xd2U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid)))) {
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 8U;
                        } else if ((8U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt))) {
                            vlSelf->dev_error = 1U;
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x15U;
                        } else {
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt 
                                = (0xfU & ((IData)(1U) 
                                           + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt)));
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 2U;
                        }
                    } else if ((0xfa0U <= vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt)) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
                        if ((8U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt))) {
                            vlSelf->dev_error = 1U;
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0x15U;
                        } else {
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt 
                                = (0xfU & ((IData)(1U) 
                                           + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt)));
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 2U;
                        }
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
                            = ((IData)(1U) + vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt);
                    }
                } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 7U;
                }
            } else if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start = 1U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid = 0xc3U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token = 0U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len = 8U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 6U;
            } else if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done) {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 5U;
            }
        } else if ((2U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
            if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start = 1U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid = 0x2dU;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token = 1U;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_addr_r 
                    = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tok_addr;
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_endp_r = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 4U;
            } else {
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_payload 
                    = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__ctl_which)
                        ? vlSelf->tb_usb_midi_enum__DOT__dut__DOT__REQ_SET_CONFIG
                        : vlSelf->tb_usb_midi_enum__DOT__dut__DOT__REQ_SET_ADDRESS);
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 3U;
            }
        } else if ((1U & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state))) {
            if ((0x1f4U <= vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt)) {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__ctl_which = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tok_addr = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 2U;
            } else {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
                    = ((IData)(1U) + vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt);
            }
        } else {
            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__force_se0 = 1U;
            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
            if ((0x7d0U <= vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt)) {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 1U;
            } else {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
                    = ((IData)(1U) + vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt);
            }
        }
    } else {
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__ctl_which = 0U;
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__data_toggle = 0U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf_cnt = 0U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx = 0U;
        vlSelf->poll_count = 0U;
        vlSelf->nak_count = 0U;
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state = 0U;
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = 0U;
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt = 0U;
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tok_addr = 0U;
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable = 0U;
        vlSelf->dev_configured = 0U;
        vlSelf->dev_error = 0U;
        __Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v1 = 1U;
    }
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__state;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__wait_cnt;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__data_toggle 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__data_toggle;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tok_addr 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tok_addr;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__ctl_which 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__ctl_which;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__retry_cnt;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__tx_done;
    if (__Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0) {
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf[__Vdlyvdim0__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0] 
            = __Vdlyvval__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v0;
    }
    if (__Vdlyvset__tb_usb_midi_enum__DOT__dut__DOT__rxbuf__v1) {
        __Vilp = 0U;
        while ((__Vilp <= 0x3fU)) {
            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf[__Vilp] = 0U;
            __Vilp = ((IData)(1U) + __Vilp);
        }
    }
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_start 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_start;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_pid;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_len;
    vlSelf->pkt_count_dbg = vlSelf->tb_usb_midi_enum__DOT__pkt_count;
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid_valid = 0U;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_byte_valid = 0U;
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_done = 0U;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__usb_dp 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dp;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__usb_dm 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dm;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_start 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__force_se0 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__force_se0;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_is_token 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_addr_r;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_endp_r;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_payload;
    vlSelf->tb_usb_midi_enum__DOT__nak_count = vlSelf->nak_count;
    vlSelf->tb_usb_midi_enum__DOT__dev_error = vlSelf->dev_error;
    vlSelf->tb_usb_midi_enum__DOT__poll_count = vlSelf->poll_count;
    vlSelf->tb_usb_midi_enum__DOT__dev_configured = vlSelf->dev_configured;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p1 
        = (0x3fU & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx)));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p2 
        = (0x3fU & ((IData)(2U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx)));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p3 
        = (0x3fU & ((IData)(3U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx)));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid_valid = 0U;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte_valid = 0U;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done = 0U;
    if (vlSelf->resetn) {
        if (vlSelf->tb_usb_midi_enum__DOT__dev_rx_enable) {
            if ((0U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rxstate))) {
                vlSelf->tb_usb_midi_enum__DOT__dev_rx_active = 0U;
                if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k) {
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 1U;
                    vlSelf->tb_usb_midi_enum__DOT__dev_rx_active = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc = 0xffffU;
                }
            } else if ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rxstate))) {
                if ((1U & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j)) 
                           & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k))))) {
                    if ((8U != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt))) {
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt)));
                    }
                } else if ((8U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt))) {
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 5U;
                } else {
                    vlSelf->tb_usb_midi_enum__DOT__dev_rx_done = 1U;
                    vlSelf->tb_usb_midi_enum__DOT__dev_rx_ok = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 0U;
                    vlSelf->tb_usb_midi_enum__DOT__dev_rx_active = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt = 0U;
                }
            } else if ((5U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rxstate))) {
                if (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j) 
                     & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k)))) {
                    if ((4U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt))) {
                        vlSelf->tb_usb_midi_enum__DOT__dev_rx_done = 1U;
                        vlSelf->tb_usb_midi_enum__DOT__dev_rx_ok 
                            = (((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err)) 
                                & (0U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt))) 
                               & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__is_data_pid)) 
                                  | ((0xb001U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc)) 
                                     & (2U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes)))));
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 0U;
                        vlSelf->tb_usb_midi_enum__DOT__dev_rx_active = 0U;
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt = 0U;
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt)));
                    }
                } else {
                    vlSelf->tb_usb_midi_enum__DOT__dev_rx_done = 1U;
                    vlSelf->tb_usb_midi_enum__DOT__dev_rx_ok 
                        = (((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err)) 
                            & (0U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt))) 
                           & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__is_data_pid)) 
                              | ((0xb001U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc)) 
                                 & (2U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes)))));
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 0U;
                    vlSelf->tb_usb_midi_enum__DOT__dev_rx_active = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt = 0U;
                }
            } else if ((1U & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j)) 
                              & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k))))) {
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 4U;
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt = 1U;
            } else {
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk 
                    = ((((((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j) 
                           != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j)) 
                          & (0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk))) 
                         & (1U != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk))) 
                        & (4U != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk)))
                        ? 1U : ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk))
                                 ? 0U : (7U & ((IData)(1U) 
                                               + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk)))));
                if ((3U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk))) {
                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec 
                        = ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j) 
                           == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j));
                    if ((8U != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt))) {
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt)));
                    } else if ((6U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run))) {
                        if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec) {
                            __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err = 1U;
                        }
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run = 0U;
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run 
                            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec)
                                ? (7U & ((IData)(1U) 
                                         + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run)))
                                : 0U);
                        if ((1U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rxstate))) {
                            __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift 
                                = (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec) 
                                    << 7U) | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift) 
                                                 >> 1U)));
                            __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt 
                                = (7U & ((IData)(1U) 
                                         + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt)));
                            if ((7U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt))) {
                                vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid 
                                    = (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec) 
                                        << 7U) | (0x7fU 
                                                  & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift) 
                                                     >> 1U)));
                                vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid_valid = 1U;
                                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt = 0U;
                                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 3U;
                                vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__is_data_pid 
                                    = ((0xc3U == (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec) 
                                                   << 7U) 
                                                  | (0x7fU 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift) 
                                                        >> 1U)))) 
                                       | (0x4bU == 
                                          (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec) 
                                            << 7U) 
                                           | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift) 
                                                 >> 1U)))));
                            }
                        } else {
                            __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift 
                                = (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec) 
                                    << 7U) | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift) 
                                                 >> 1U)));
                            __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt 
                                = (7U & ((IData)(1U) 
                                         + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt)));
                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__b 
                                = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec;
                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__c 
                                = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc;
                            vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__Vstatic__t 
                                = (1U & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__c) 
                                         ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__b)));
                            __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__Vfuncout 
                                = (0xffffU & VL_SHIFTR_III(16,16,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__c), 1U));
                            if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__Vstatic__t) {
                                __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__Vfuncout 
                                    = (0xa001U ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__Vfuncout));
                            }
                            __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc 
                                = __Vfunc_tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__19__Vfuncout;
                            if ((7U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt))) {
                                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes 
                                    = (0xffU & ((IData)(1U) 
                                                + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes)));
                                if ((2U == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n))) {
                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_byte_valid = 1U;
                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_byte 
                                        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b1;
                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b1 
                                        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0;
                                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0 
                                        = (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec) 
                                            << 7U) 
                                           | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift) 
                                                 >> 1U)));
                                } else {
                                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n 
                                        = (3U & ((IData)(1U) 
                                                 + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n)));
                                    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b1 
                                        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0;
                                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0 
                                        = (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec) 
                                            << 7U) 
                                           | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift) 
                                                 >> 1U)));
                                }
                            }
                        }
                    }
                    __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j 
                        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j;
                }
                __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j 
                    = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j;
            }
        } else {
            __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 0U;
            vlSelf->tb_usb_midi_enum__DOT__dev_rx_active = 0U;
        }
        if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_enable) {
            if ((0U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate))) {
                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active = 0U;
                if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 1U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc = 0xffffU;
                }
            } else if ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate))) {
                if ((1U & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j)) 
                           & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k))))) {
                    if ((8U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt))) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt)));
                    }
                } else if ((8U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt))) {
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt = 1U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 5U;
                } else {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done = 1U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 0U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt = 0U;
                }
            } else if ((5U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate))) {
                if (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j) 
                     & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k)))) {
                    if ((4U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt))) {
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done = 1U;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok 
                            = (((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err)) 
                                & (0U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt))) 
                               & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__is_data_pid)) 
                                  | ((0xb001U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc)) 
                                     & (2U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes)))));
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 0U;
                        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active = 0U;
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt = 0U;
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt)));
                    }
                } else {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done = 1U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok 
                        = (((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err)) 
                            & (0U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt))) 
                           & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__is_data_pid)) 
                              | ((0xb001U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc)) 
                                 & (2U <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes)))));
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 0U;
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active = 0U;
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt = 0U;
                }
            } else if ((1U & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j)) 
                              & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k))))) {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 4U;
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt = 1U;
            } else {
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk 
                    = ((((((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j) 
                           != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j)) 
                          & (0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk))) 
                         & (1U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk))) 
                        & (4U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk)))
                        ? 1U : ((4U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk))
                                 ? 0U : (7U & ((IData)(1U) 
                                               + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk)))));
                if ((3U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk))) {
                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec 
                        = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j) 
                           == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j));
                    if ((8U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt))) {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt 
                            = (0xfU & ((IData)(1U) 
                                       + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt)));
                    } else if ((6U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run))) {
                        if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec) {
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err = 1U;
                        }
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run = 0U;
                    } else {
                        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run 
                            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec)
                                ? (7U & ((IData)(1U) 
                                         + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run)))
                                : 0U);
                        if ((1U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate))) {
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift 
                                = (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec) 
                                    << 7U) | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift) 
                                                 >> 1U)));
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt 
                                = (7U & ((IData)(1U) 
                                         + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt)));
                            if ((7U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt))) {
                                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid 
                                    = (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec) 
                                        << 7U) | (0x7fU 
                                                  & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift) 
                                                     >> 1U)));
                                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid_valid = 1U;
                                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt = 0U;
                                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 3U;
                                vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__is_data_pid 
                                    = ((0xc3U == (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec) 
                                                   << 7U) 
                                                  | (0x7fU 
                                                     & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift) 
                                                        >> 1U)))) 
                                       | (0x4bU == 
                                          (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec) 
                                            << 7U) 
                                           | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift) 
                                                 >> 1U)))));
                            }
                        } else {
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift 
                                = (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec) 
                                    << 7U) | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift) 
                                                 >> 1U)));
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt 
                                = (7U & ((IData)(1U) 
                                         + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt)));
                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__b 
                                = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec;
                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__c 
                                = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc;
                            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__Vstatic__t 
                                = (1U & ((IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__c) 
                                         ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__b)));
                            __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__Vfuncout 
                                = (0xffffU & VL_SHIFTR_III(16,16,32, (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__c), 1U));
                            if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__Vstatic__t) {
                                __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__Vfuncout 
                                    = (0xa001U ^ (IData)(__Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__Vfuncout));
                            }
                            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc 
                                = __Vfunc_tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__9__Vfuncout;
                            if ((7U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt))) {
                                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes 
                                    = (0xffU & ((IData)(1U) 
                                                + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes)));
                                if ((2U == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n))) {
                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte_valid = 1U;
                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte 
                                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b1;
                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b1 
                                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0;
                                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0 
                                        = (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec) 
                                            << 7U) 
                                           | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift) 
                                                 >> 1U)));
                                } else {
                                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n 
                                        = (3U & ((IData)(1U) 
                                                 + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n)));
                                    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b1 
                                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0;
                                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0 
                                        = (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec) 
                                            << 7U) 
                                           | (0x7fU 
                                              & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift) 
                                                 >> 1U)));
                                }
                            }
                        }
                    }
                    __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j 
                        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j;
                }
                __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j 
                    = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j;
            }
        } else {
            __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 0U;
            vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active = 0U;
        }
    } else {
        __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = 0U;
        vlSelf->tb_usb_midi_enum__DOT__dev_rx_active = 0U;
        __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = 0U;
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active = 0U;
    }
    vlSelf->tb_usb_midi_enum__DOT__pkt_count_dbg = vlSelf->pkt_count_dbg;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rxstate 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rxstate;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0 
        = __Vdly__tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0;
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_enable = 
        (1U & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dev_tx_busy)));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k 
        = ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s)) 
           & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j 
        = ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s)) 
           & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__nak_count 
        = vlSelf->tb_usb_midi_enum__DOT__nak_count;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__dev_error 
        = vlSelf->tb_usb_midi_enum__DOT__dev_error;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__poll_count 
        = vlSelf->tb_usb_midi_enum__DOT__poll_count;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__dev_configured 
        = vlSelf->tb_usb_midi_enum__DOT__dev_configured;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_enable 
        = __Vdly__tb_usb_midi_enum__DOT__dut__DOT__rx_enable;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k 
        = ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s)) 
           & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j 
        = ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s)) 
           & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_enable 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_enable;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_pid_valid 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid_valid;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_done;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_active 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_active;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cur_sym_k 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cur_sym_j 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_ok 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_ok;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_pid 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_enable 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_enable;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_pid_valid 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid_valid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_byte_valid 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte_valid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_active 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cur_sym_k 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cur_sym_j 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ok 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_pid 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_byte 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte;
}

VL_INLINE_OPT void Vtb_usb_midi_enum___024root___nba_sequent__TOP__1(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___nba_sequent__TOP__1\n"); );
    // Body
    if (vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat) {
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_crc_last_bit 
            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos) 
               == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_tok));
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_crc_covered 
            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT____VdfgTmp_h5d27979b__0) 
               & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos) 
                  <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_tok)));
    } else {
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_crc_last_bit 
            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos) 
               == (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_data));
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_crc_covered 
            = ((0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat)) 
               & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT____VdfgTmp_h5d27979b__0) 
                  & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos) 
                     <= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_data))));
    }
    if (vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat) {
        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_crc_last_bit 
            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos) 
               == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_tok));
        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_crc_covered 
            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT____VdfgTmp_h5d27979b__0) 
               & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos) 
                  <= (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_tok)));
    } else {
        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_crc_last_bit 
            = ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos) 
               == (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_data));
        vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_crc_covered 
            = ((0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat)) 
               & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT____VdfgTmp_h5d27979b__0) 
                  & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos) 
                     <= (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_data))));
    }
}

void Vtb_usb_midi_enum___024root___eval_nba(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_nba\n"); );
    // Body
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        Vtb_usb_midi_enum___024root___nba_sequent__TOP__0(vlSelf);
        Vtb_usb_midi_enum___024root___nba_sequent__TOP__1(vlSelf);
    }
}

void Vtb_usb_midi_enum___024root___eval_triggers__act(Vtb_usb_midi_enum___024root* vlSelf);

bool Vtb_usb_midi_enum___024root___eval_phase__act(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_phase__act\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_usb_midi_enum___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelf->__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
        vlSelf->__VnbaTriggered.thisOr(vlSelf->__VactTriggered);
        Vtb_usb_midi_enum___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vtb_usb_midi_enum___024root___eval_phase__nba(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_phase__nba\n"); );
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelf->__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vtb_usb_midi_enum___024root___eval_nba(vlSelf);
        vlSelf->__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__ico(Vtb_usb_midi_enum___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__nba(Vtb_usb_midi_enum___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__act(Vtb_usb_midi_enum___024root* vlSelf);
#endif  // VL_DEBUG

void Vtb_usb_midi_enum___024root___eval(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval\n"); );
    // Init
    IData/*31:0*/ __VicoIterCount;
    CData/*0:0*/ __VicoContinue;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VicoIterCount = 0U;
    vlSelf->__VicoFirstIteration = 1U;
    __VicoContinue = 1U;
    while (__VicoContinue) {
        if (VL_UNLIKELY((0x64U < __VicoIterCount))) {
#ifdef VL_DEBUG
            Vtb_usb_midi_enum___024root___dump_triggers__ico(vlSelf);
#endif
            VL_FATAL_MT("tb_usb_midi_enum.v", 28, "", "Input combinational region did not converge.");
        }
        __VicoIterCount = ((IData)(1U) + __VicoIterCount);
        __VicoContinue = 0U;
        if (Vtb_usb_midi_enum___024root___eval_phase__ico(vlSelf)) {
            __VicoContinue = 1U;
        }
        vlSelf->__VicoFirstIteration = 0U;
    }
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
            Vtb_usb_midi_enum___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("tb_usb_midi_enum.v", 28, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                Vtb_usb_midi_enum___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("tb_usb_midi_enum.v", 28, "", "Active region did not converge.");
            }
            vlSelf->__VactIterCount = ((IData)(1U) 
                                       + vlSelf->__VactIterCount);
            vlSelf->__VactContinue = 0U;
            if (Vtb_usb_midi_enum___024root___eval_phase__act(vlSelf)) {
                vlSelf->__VactContinue = 1U;
            }
        }
        if (Vtb_usb_midi_enum___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vtb_usb_midi_enum___024root___eval_debug_assertions(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->resetn & 0xfeU))) {
        Verilated::overWidthError("resetn");}
}
#endif  // VL_DEBUG
