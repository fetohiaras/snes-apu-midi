`timescale 1ns/1ps

// Test wrapper: a serial chain (uart_rx -> midi_parser) and a second
// parser fed directly with bytes for fast randomized checking.
module tb_midi_top (
    input  wire       clk,
    input  wire       resetn,
    input  wire       rx,
    input  wire       byte_valid,
    input  wire [7:0] byte_in,

    output wire       uart_valid,
    output wire [7:0] uart_data,
    output wire       uart_ferr,
    output wire       ser_ev_valid,
    output wire [7:0] ser_ev_status,
    output wire [6:0] ser_ev_data1,
    output wire [6:0] ser_ev_data2,
    output wire       dir_ev_valid,
    output wire [7:0] dir_ev_status,
    output wire [6:0] dir_ev_data1,
    output wire [6:0] dir_ev_data2
);

    uart_rx #(.CLK_HZ(24528000), .BAUD(31250)) u_rx (
        .clk(clk), .resetn(resetn), .rx(rx),
        .data(uart_data), .valid(uart_valid), .frame_err(uart_ferr)
    );

    midi_parser u_ser (
        .clk(clk), .resetn(resetn),
        .in_valid(uart_valid), .in_byte(uart_data),
        .ev_valid(ser_ev_valid), .ev_status(ser_ev_status),
        .ev_data1(ser_ev_data1), .ev_data2(ser_ev_data2)
    );

    midi_parser u_dir (
        .clk(clk), .resetn(resetn),
        .in_valid(byte_valid), .in_byte(byte_in),
        .ev_valid(dir_ev_valid), .ev_status(dir_ev_status),
        .ev_data1(dir_ev_data1), .ev_data2(dir_ev_data2)
    );

endmodule
