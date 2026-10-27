`ifndef USB_MIDI_DEFS_VH
`define USB_MIDI_DEFS_VH

`define USB_PID_OUT    8'hE1
`define USB_PID_IN     8'h69
`define USB_PID_SETUP  8'h2D
`define USB_PID_DATA0  8'hC3
`define USB_PID_DATA1  8'h4B
`define USB_PID_ACK    8'hD2
`define USB_PID_NAK    8'h5A
`define USB_PID_STALL  8'h1E

`define CRC5_POLY_REFL   5'h14
`define CRC5_INIT        5'h1F
`define CRC5_XOROUT      5'h1F
`define CRC5_GOOD_RESID  5'h06

`define CRC16_POLY_REFL  16'hA001
`define CRC16_INIT       16'hFFFF
`define CRC16_XOROUT     16'hFFFF
`define CRC16_GOOD_RESID 16'hB001

`endif
