`timescale 1ns/1ps

module usb_midi_top #(
    parameter integer RESET_HOLD_CYCLES     = 32'd900_000,
    parameter integer SETTLE_CYCLES         = 32'd60_000,
    parameter integer POST_CTL_DELAY_CYCLES = 32'd120_000,
    parameter integer TIMEOUT_CYCLES        = 32'd60_000,
    parameter integer POLL_GAP_CYCLES       = 32'd600,
    parameter integer MAX_RETRIES           = 8,
    parameter [6:0]   NEW_ADDR              = 7'd1,
    parameter [3:0]   MIDI_BULK_IN_EP       = 4'd1
) (
    input  wire clk_usb,
    input  wire resetn_usb,

    inout  wire usb_dp,
    inout  wire usb_dm,

    input  wire clk,
    input  wire resetn,

    output wire        ev_valid,
    output wire [7:0]  ev_status,
    output wire [6:0]  ev_data1,
    output wire [6:0]  ev_data2,

    output wire        dev_configured,
    output wire        dev_error,
    output wire [15:0] poll_count,
    output wire [15:0] nak_count
);

    wire       pkt_valid;
    wire [7:0] pkt_cin_cable;
    wire [7:0] pkt_b0, pkt_b1, pkt_b2;

    usb_midi_host #(
        .RESET_HOLD_CYCLES    (RESET_HOLD_CYCLES),
        .SETTLE_CYCLES        (SETTLE_CYCLES),
        .POST_CTL_DELAY_CYCLES(POST_CTL_DELAY_CYCLES),
        .TIMEOUT_CYCLES       (TIMEOUT_CYCLES),
        .POLL_GAP_CYCLES      (POLL_GAP_CYCLES),
        .MAX_RETRIES          (MAX_RETRIES),
        .NEW_ADDR             (NEW_ADDR),
        .MIDI_BULK_IN_EP      (MIDI_BULK_IN_EP)
    ) u_host (
        .clk           (clk_usb),
        .resetn        (resetn_usb),
        .usb_dp        (usb_dp),
        .usb_dm        (usb_dm),
        .pkt_valid     (pkt_valid),
        .pkt_cin_cable (pkt_cin_cable),
        .pkt_b0        (pkt_b0),
        .pkt_b1        (pkt_b1),
        .pkt_b2        (pkt_b2),
        .dev_configured(dev_configured),
        .dev_error     (dev_error),
        .poll_count    (poll_count),
        .nak_count     (nak_count)
    );

    usb_midi_note_bridge u_bridge (
        .clk_usb       (clk_usb),
        .resetn_usb    (resetn_usb),
        .pkt_valid     (pkt_valid),
        .pkt_cin_cable (pkt_cin_cable),
        .pkt_b0        (pkt_b0),
        .pkt_b1        (pkt_b1),
        .pkt_b2        (pkt_b2),
        .clk           (clk),
        .resetn        (resetn),
        .ev_valid      (ev_valid),
        .ev_status     (ev_status),
        .ev_data1      (ev_data1),
        .ev_data2      (ev_data2)
    );

endmodule
