// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_usb_midi_enum.h for the primary calling header

#include "Vtb_usb_midi_enum__pch.h"
#include "Vtb_usb_midi_enum__Syms.h"
#include "Vtb_usb_midi_enum___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_usb_midi_enum___024root___dump_triggers__stl(Vtb_usb_midi_enum___024root* vlSelf);
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_usb_midi_enum___024root___eval_triggers__stl(Vtb_usb_midi_enum___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtb_usb_midi_enum__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_usb_midi_enum___024root___eval_triggers__stl\n"); );
    // Body
    vlSelf->__VstlTriggered.set(0U, (IData)(vlSelf->__VstlFirstIteration));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_usb_midi_enum___024root___dump_triggers__stl(vlSelf);
    }
#endif
}
