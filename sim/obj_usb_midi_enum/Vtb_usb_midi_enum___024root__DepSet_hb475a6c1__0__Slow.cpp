// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_usb_midi_enum.h for the primary calling header

#include "Vtb_usb_midi_enum__pch.h"
#include "Vtb_usb_midi_enum___024root.h"

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_static(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_static\n"); );
}

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_initial__TOP(Vtb_usb_midi_enum___024root* vlSelf);

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_initial(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_initial\n"); );
    // Body
    Vtb_usb_midi_enum___024root___eval_initial__TOP(vlSelf);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = vlSelf->clk;
}

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_initial__TOP(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_initial__TOP\n"); );
    // Body
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__REQ_SET_ADDRESS = 0x10500ULL;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__REQ_SET_CONFIG = 0x10900ULL;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_start = 8U;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_start = 8U;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__force_se0 = 0U;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_is_token = 0U;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr = 0U;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp = 0U;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload = 0ULL;
}

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_final(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_final\n"); );
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__stl(Vtb_usb_midi_enum___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtb_usb_midi_enum___024root___eval_phase__stl(Vtb_usb_midi_enum___024root* vlSelf);

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_settle(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_settle\n"); );
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelf->__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY((0x64U < __VstlIterCount))) {
#ifdef VL_DEBUG
            Vtb_usb_midi_enum___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("tb_usb_midi_enum.v", 28, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vtb_usb_midi_enum___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelf->__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__stl(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VstlTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___stl_sequent__TOP__0(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___stl_sequent__TOP__0\n"); );
    // Body
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rst 
        = (1U & (~ (IData)(vlSelf->resetn)));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__force_se0 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__force_se0;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_start 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_is_token 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_addr_r;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_endp_r;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_payload;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_enable 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_enable;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_active 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_pid 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_pid_valid 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid_valid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_byte_valid 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte_valid;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_byte 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ok 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rst 
        = (1U & (~ (IData)(vlSelf->resetn)));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_start 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_start;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_pid;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_len;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_done;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_active 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_active;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_pid 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_pid_valid 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid_valid;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_done 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_done;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_ok 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_ok;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p1 
        = (0x3fU & ((IData)(1U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx)));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p2 
        = (0x3fU & ((IData)(2U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx)));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p3 
        = (0x3fU & ((IData)(3U) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx)));
    vlSelf->tb_usb_midi_enum__DOT__resetn = vlSelf->resetn;
    vlSelf->tb_usb_midi_enum__DOT__dev_configured = vlSelf->dev_configured;
    vlSelf->tb_usb_midi_enum__DOT__dev_error = vlSelf->dev_error;
    vlSelf->tb_usb_midi_enum__DOT__poll_count = vlSelf->poll_count;
    vlSelf->tb_usb_midi_enum__DOT__nak_count = vlSelf->nak_count;
    vlSelf->pkt_count_dbg = vlSelf->tb_usb_midi_enum__DOT__pkt_count;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_busy 
        = (0U != (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate));
    vlSelf->tb_usb_midi_enum__DOT__clk = vlSelf->clk;
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_busy = (0U 
                                                  != (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT____VdfgTmp_h5d27979b__0 
        = ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos) 
           >= (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_start));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT____VdfgTmp_h5d27979b__0 
        = ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos) 
           >= (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_start));
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
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_tok 
        = (0x7fU & ((IData)(0xaU) + (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_start)));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_tok 
        = (0x7fU & ((IData)(0xaU) + (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_start)));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_data 
        = (0x7fU & (((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_start) 
                     + VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat), 3U)) 
                    - (IData)(1U)));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_data 
        = (0x7fU & (((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_start) 
                     + VL_SHIFTL_III(7,7,32, (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat), 3U)) 
                    - (IData)(1U)));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s 
        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_sync) 
                 >> 1U));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s 
        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_sync) 
                 >> 1U));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s 
        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_sync) 
                 >> 1U));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s 
        = (1U & ((IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_sync) 
                 >> 1U));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__resetn 
        = vlSelf->tb_usb_midi_enum__DOT__resetn;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__dev_configured 
        = vlSelf->tb_usb_midi_enum__DOT__dev_configured;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__dev_error 
        = vlSelf->tb_usb_midi_enum__DOT__dev_error;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__poll_count 
        = vlSelf->tb_usb_midi_enum__DOT__poll_count;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__nak_count 
        = vlSelf->tb_usb_midi_enum__DOT__nak_count;
    vlSelf->tb_usb_midi_enum__DOT__pkt_count_dbg = vlSelf->pkt_count_dbg;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_busy 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_busy;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__clk 
        = vlSelf->tb_usb_midi_enum__DOT__clk;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__clk = vlSelf->tb_usb_midi_enum__DOT__clk;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_busy 
        = vlSelf->tb_usb_midi_enum__DOT__dev_tx_busy;
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_enable = 
        (1U & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dev_tx_busy)));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__usb_dp 
        = vlSelf->tb_usb_midi_enum__DOT__usb_dp;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dp 
        = vlSelf->tb_usb_midi_enum__DOT__usb_dp;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__usb_dm 
        = vlSelf->tb_usb_midi_enum__DOT__usb_dm;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dm 
        = vlSelf->tb_usb_midi_enum__DOT__usb_dm;
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
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_se0 
        = (1U & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s)) 
                 & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s))));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k 
        = ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s)) 
           & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j 
        = ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s)) 
           & (IData)(vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_se0 
        = (1U & ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s)) 
                 & (~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s))));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k 
        = ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s)) 
           & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s));
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j 
        = ((~ (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s)) 
           & (IData)(vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s));
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__clk 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__clk;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_enable 
        = vlSelf->tb_usb_midi_enum__DOT__dev_rx_enable;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__usb_dp 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dp;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__usb_dm 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dm;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cur_sym_k 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k;
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cur_sym_j 
        = vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cur_sym_k 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cur_sym_j 
        = vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j;
}

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_stl(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_stl\n"); );
    // Body
    if ((1ULL & vlSelf->__VstlTriggered.word(0U))) {
        Vtb_usb_midi_enum___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_triggers__stl(Vtb_usb_midi_enum___024root* vlSelf);

VL_ATTR_COLD bool Vtb_usb_midi_enum___024root___eval_phase__stl(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_phase__stl\n"); );
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtb_usb_midi_enum___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelf->__VstlTriggered.any();
    if (__VstlExecute) {
        Vtb_usb_midi_enum___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__ico(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___dump_triggers__ico\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VicoTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VicoTriggered.word(0U))) {
        VL_DBG_MSGF("         'ico' region trigger index 0 is active: Internal 'ico' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__act(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VactTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__nba(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___dump_triggers__nba\n"); );
    // Body
    if ((1U & (~ (IData)(vlSelf->__VnbaTriggered.any())))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelf->__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___ctor_var_reset(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->resetn = VL_RAND_RESET_I(1);
    vlSelf->dev_configured = VL_RAND_RESET_I(1);
    vlSelf->dev_error = VL_RAND_RESET_I(1);
    vlSelf->poll_count = VL_RAND_RESET_I(16);
    vlSelf->nak_count = VL_RAND_RESET_I(16);
    vlSelf->pkt_count_dbg = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__resetn = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_configured = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_error = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__poll_count = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__nak_count = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__pkt_count_dbg = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__usb_dp = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__usb_dm = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_start = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_pid = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_len = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_tx_busy = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_active = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_ok = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_pid_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dev_rx_enable = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__pkt_count = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__resetn = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dp = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__usb_dm = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_cin_cable = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_b0 = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_b1 = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__pkt_b2 = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__dev_configured = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__dev_error = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__poll_count = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__nak_count = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_start = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_pid = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_is_token = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_addr_r = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_endp_r = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_payload = VL_RAND_RESET_Q(64);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_len = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tx_busy = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_enable = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_active = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_pid_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_byte = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rx_ok = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__force_se0 = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__REQ_SET_ADDRESS = VL_RAND_RESET_Q(64);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__REQ_SET_CONFIG = VL_RAND_RESET_Q(64);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__state = VL_RAND_RESET_I(5);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__wait_cnt = VL_RAND_RESET_I(32);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__retry_cnt = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__ctl_which = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__tok_addr = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__data_toggle = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 64; ++__Vi0) {
        vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf[__Vi0] = VL_RAND_RESET_I(8);
    }
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rxbuf_cnt = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p1 = VL_RAND_RESET_I(6);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p2 = VL_RAND_RESET_I(6);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__proc_idx_p3 = VL_RAND_RESET_I(6);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__rb = VL_RAND_RESET_I(32);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rst = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__usb_dp = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__usb_dm = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_k = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__sym_se0 = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__force_se0 = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_start = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_is_token = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload = VL_RAND_RESET_Q(64);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_busy = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_enable = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_active = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_pid = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_pid_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_byte_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_byte = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ok = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_sync = VL_RAND_RESET_I(2);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_sync = VL_RAND_RESET_I(2);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dp_s = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__dm_s = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__oe = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__drive_se0 = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc5_upd__Vstatic__t = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc16_upd__Vstatic__t = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__txstate = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_bitclk = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pos = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_total = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_tok_lat = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_addr_lat = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_endp_lat = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_pid_lat = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_payload_lat = VL_RAND_RESET_Q(64);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_len_lat = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_run = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__crc_final = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__ones_run = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_bitclk_cnt = VL_RAND_RESET_I(2);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_start = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_tok = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cov_end_data = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__p = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__get_bit__Vstatic__crc_idx = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_crc_covered = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__tx_crc_last_bit = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rxstate = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__eop_cnt = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_bitclk = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__prev_sym_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__line_sym_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_ones_run = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__skip_cnt = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_cnt = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pid_shift = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_cnt = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__byte_shift = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__is_data_pid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rx_crc = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b0 = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_b1 = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__pipe_n = VL_RAND_RESET_I(2);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__stuff_err = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__rcvd_bytes = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__bit_dec = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cur_sym_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT__cur_sym_k = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__dut__DOT__u_sie__DOT____VdfgTmp_h5d27979b__0 = 0;
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__clk = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rst = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__usb_dp = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__usb_dm = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_k = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__sym_se0 = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__force_se0 = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_start = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_is_token = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload = VL_RAND_RESET_Q(64);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_busy = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_enable = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_active = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_pid = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_pid_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_byte_valid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_byte = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_done = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_ok = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_sync = VL_RAND_RESET_I(2);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_sync = VL_RAND_RESET_I(2);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dp_s = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__dm_s = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__oe = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__drive_se0 = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc5_upd__Vstatic__t = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc16_upd__Vstatic__t = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__txstate = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_bitclk = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pos = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_total = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_tok_lat = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_addr_lat = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_endp_lat = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_pid_lat = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_payload_lat = VL_RAND_RESET_Q(64);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_len_lat = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_run = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__crc_final = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__ones_run = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_bitclk_cnt = VL_RAND_RESET_I(2);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_start = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_tok = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cov_end_data = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__p = VL_RAND_RESET_I(7);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__get_bit__Vstatic__crc_idx = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_crc_covered = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__tx_crc_last_bit = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rxstate = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__eop_cnt = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_bitclk = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__prev_sym_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__line_sym_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_ones_run = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__skip_cnt = VL_RAND_RESET_I(4);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_cnt = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pid_shift = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_cnt = VL_RAND_RESET_I(3);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__byte_shift = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__is_data_pid = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rx_crc = VL_RAND_RESET_I(16);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b0 = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_b1 = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__pipe_n = VL_RAND_RESET_I(2);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__stuff_err = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__rcvd_bytes = VL_RAND_RESET_I(8);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__bit_dec = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cur_sym_j = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT__cur_sym_k = VL_RAND_RESET_I(1);
    vlSelf->tb_usb_midi_enum__DOT__device_sie__DOT____VdfgTmp_h5d27979b__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
}
