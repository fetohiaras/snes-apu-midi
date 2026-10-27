// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VTB_USB_MIDI_ENUM__SYMS_H_
#define VERILATED_VTB_USB_MIDI_ENUM__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vtb_usb_midi_enum.h"

// INCLUDE MODULE CLASSES
#include "Vtb_usb_midi_enum___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)Vtb_usb_midi_enum__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vtb_usb_midi_enum* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vtb_usb_midi_enum___024root    TOP;

    // CONSTRUCTORS
    Vtb_usb_midi_enum__Syms(VerilatedContext* contextp, const char* namep, Vtb_usb_midi_enum* modelp);
    ~Vtb_usb_midi_enum__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
