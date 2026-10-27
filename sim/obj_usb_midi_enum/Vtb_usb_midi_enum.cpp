// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_usb_midi_enum__pch.h"

//============================================================
// Constructors

Vtb_usb_midi_enum::Vtb_usb_midi_enum(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_usb_midi_enum__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , resetn{vlSymsp->TOP.resetn}
    , dev_configured{vlSymsp->TOP.dev_configured}
    , dev_error{vlSymsp->TOP.dev_error}
    , pkt_count_dbg{vlSymsp->TOP.pkt_count_dbg}
    , poll_count{vlSymsp->TOP.poll_count}
    , nak_count{vlSymsp->TOP.nak_count}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_usb_midi_enum::Vtb_usb_midi_enum(const char* _vcname__)
    : Vtb_usb_midi_enum(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_usb_midi_enum::~Vtb_usb_midi_enum() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_usb_midi_enum___024root___eval_debug_assertions(Vtb_usb_midi_enum___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_usb_midi_enum___024root___eval_static(Vtb_usb_midi_enum___024root* vlSelf);
void Vtb_usb_midi_enum___024root___eval_initial(Vtb_usb_midi_enum___024root* vlSelf);
void Vtb_usb_midi_enum___024root___eval_settle(Vtb_usb_midi_enum___024root* vlSelf);
void Vtb_usb_midi_enum___024root___eval(Vtb_usb_midi_enum___024root* vlSelf);

void Vtb_usb_midi_enum::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_usb_midi_enum::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_usb_midi_enum___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_usb_midi_enum___024root___eval_static(&(vlSymsp->TOP));
        Vtb_usb_midi_enum___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_usb_midi_enum___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_usb_midi_enum___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_usb_midi_enum::eventsPending() { return false; }

uint64_t Vtb_usb_midi_enum::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "%Error: No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* Vtb_usb_midi_enum::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_usb_midi_enum___024root___eval_final(Vtb_usb_midi_enum___024root* vlSelf);

VL_ATTR_COLD void Vtb_usb_midi_enum::final() {
    Vtb_usb_midi_enum___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_usb_midi_enum::hierName() const { return vlSymsp->name(); }
const char* Vtb_usb_midi_enum::modelName() const { return "Vtb_usb_midi_enum"; }
unsigned Vtb_usb_midi_enum::threads() const { return 1; }
void Vtb_usb_midi_enum::prepareClone() const { contextp()->prepareClone(); }
void Vtb_usb_midi_enum::atClone() const {
    contextp()->threadPoolpOnClone();
}

//============================================================
// Trace configuration

VL_ATTR_COLD void Vtb_usb_midi_enum::trace(VerilatedVcdC* tfp, int levels, int options) {
    vl_fatal(__FILE__, __LINE__, __FILE__,"'Vtb_usb_midi_enum::trace()' called on model that was Verilated without --trace option");
}
