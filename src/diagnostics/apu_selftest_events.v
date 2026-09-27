`timescale 1ns/1ps

// Generates a very small MIDI-like event sequence inside the FPGA so the
// SPC700/APU path can be tested without USB-MIDI, a PC bridge, or a DAC.
//
// The events are deliberately slow: NOTE_ON / NOTE_OFF for C4, E4 and G4.
// `ev_valid` is a one-clock pulse and a new event is only emitted while the
// mailbox is idle.
module apu_selftest_events #(
    parameter integer CLK_HZ         = 24528000,
    parameter integer START_DELAY_MS = 250,
    parameter integer NOTE_MS        = 400,
    parameter integer GAP_MS         = 150
) (
    input  wire       clk,
    input  wire       resetn,
    input  wire       apu_running,
    input  wire       mailbox_busy,

    output reg        ev_valid,
    output reg  [7:0] ev_status,
    output reg  [6:0] ev_data1,
    output reg  [6:0] ev_data2
);

    localparam integer TICKS_PER_MS = CLK_HZ / 1000;
    localparam integer START_TICKS  = TICKS_PER_MS * START_DELAY_MS;
    localparam integer NOTE_TICKS   = TICKS_PER_MS * NOTE_MS;
    localparam integer GAP_TICKS    = TICKS_PER_MS * GAP_MS;

    localparam [2:0] S_WAIT_APU = 3'd0,
                     S_START    = 3'd1,
                     S_NOTE_ON  = 3'd2,
                     S_HOLD     = 3'd3,
                     S_NOTE_OFF = 3'd4,
                     S_GAP      = 3'd5;

    reg [2:0]  state;
    reg [31:0] timer;
    reg [1:0]  note_index;

    function [6:0] note_value;
        input [1:0] idx;
        begin
            case (idx)
                2'd0: note_value = 7'd60; // C4
                2'd1: note_value = 7'd64; // E4
                default: note_value = 7'd67; // G4
            endcase
        end
    endfunction

    always @(posedge clk) begin
        ev_valid <= 1'b0;

        if (!resetn) begin
            state      <= S_WAIT_APU;
            timer      <= 32'd0;
            note_index <= 2'd0;
            ev_status  <= 8'h90;
            ev_data1   <= 7'd60;
            ev_data2   <= 7'd100;
        end else begin
            case (state)
                S_WAIT_APU: begin
                    timer <= 32'd0;
                    if (apu_running)
                        state <= S_START;
                end

                S_START: begin
                    if (!apu_running) begin
                        state <= S_WAIT_APU;
                        timer <= 32'd0;
                    end else if (timer >= START_TICKS - 1) begin
                        timer <= 32'd0;
                        state <= S_NOTE_ON;
                    end else begin
                        timer <= timer + 32'd1;
                    end
                end

                S_NOTE_ON: begin
                    if (!mailbox_busy) begin
                        ev_status <= 8'h90;
                        ev_data1  <= note_value(note_index);
                        ev_data2  <= 7'd100;
                        ev_valid  <= 1'b1;
                        timer     <= 32'd0;
                        state     <= S_HOLD;
                    end
                end

                S_HOLD: begin
                    if (timer >= NOTE_TICKS - 1) begin
                        timer <= 32'd0;
                        state <= S_NOTE_OFF;
                    end else begin
                        timer <= timer + 32'd1;
                    end
                end

                S_NOTE_OFF: begin
                    if (!mailbox_busy) begin
                        ev_status <= 8'h80;
                        ev_data1  <= note_value(note_index);
                        ev_data2  <= 7'd0;
                        ev_valid  <= 1'b1;
                        timer     <= 32'd0;
                        state     <= S_GAP;
                    end
                end

                default: begin // S_GAP
                    if (timer >= GAP_TICKS - 1) begin
                        timer <= 32'd0;
                        note_index <= (note_index == 2'd2) ? 2'd0 : note_index + 2'd1;
                        state <= S_NOTE_ON;
                    end else begin
                        timer <= timer + 32'd1;
                    end
                end
            endcase
        end
    end

endmodule
