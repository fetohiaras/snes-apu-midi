`timescale 1ns/1ps

// Test wrapper: parsed-MIDI events -> midi_mailbox -> apu_core running the
// SPC700 MIDI driver image built by tools/build_midi_image.py.
module tb_apu_midi_top #(
    parameter MEM_INIT_FILE = "build_midi/midi_boot.spc.hex"
) (
    input  wire        clk,
    input  wire        resetn,
    input  wire        ev_valid,
    input  wire [7:0]  ev_status,
    input  wire [6:0]  ev_data1,
    input  wire [6:0]  ev_data2,

    output wire        running,
    output wire        snd_rdy,
    output wire [15:0] audio_l,
    output wire [15:0] audio_r,
    output wire [15:0] sent_count,
    output wire        dropped,
    output wire        ack_timeout,
    output wire        busy
);

    wire [1:0] port_a;
    wire       port_wr_n;
    wire       port_cs;
    wire [7:0] port_din;
    wire [7:0] port_dout;

    apu_core #(.MEM_INIT_FILE(MEM_INIT_FILE)) apu (
        .clk       (clk),
        .resetn    (resetn),
        .running   (running),
        .port_a    (port_a),
        .port_wr_n (port_wr_n),
        .port_cs   (port_cs),
        .port_din  (port_din),
        .port_dout (port_dout),
        .snd_rdy   (snd_rdy),
        .audio_l   (audio_l),
        .audio_r   (audio_r)
    );

    midi_mailbox u_mailbox (
        .clk         (clk),
        .resetn      (resetn),
        .apu_running (running),
        .ev_valid    (ev_valid),
        .ev_status   (ev_status),
        .ev_data1    (ev_data1),
        .ev_data2    (ev_data2),
        .port_a      (port_a),
        .port_wr_n   (port_wr_n),
        .port_cs     (port_cs),
        .port_din    (port_din),
        .port_dout   (port_dout),
        .sent_count  (sent_count),
        .dropped     (dropped),
        .ack_timeout (ack_timeout),
        .busy        (busy)
    );

endmodule
