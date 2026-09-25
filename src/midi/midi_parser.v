`timescale 1ns/1ps

// MIDI 1.0 byte-stream parser producing complete channel-voice messages.
//
// - Running status is supported (keyboards such as the CTK-3500 use it).
// - Real-time bytes (F8..FF) may appear anywhere, even inside a message, and
//   are ignored without disturbing the message being assembled.
// - SysEx and system-common messages cancel running status; their data bytes
//   are discarded, as are data bytes that arrive with no running status.
// - Note On with velocity 0 is reported as Note Off (velocity 0).
//
// ev_status holds the full status byte (message type in [7:4], channel in
// [3:0]). For one-data-byte messages (Cn, Dn) ev_data2 is 0.
module midi_parser (
    input  wire       clk,
    input  wire       resetn,
    input  wire       in_valid,
    input  wire [7:0] in_byte,
    output reg        ev_valid,
    output reg  [7:0] ev_status,
    output reg  [6:0] ev_data1,
    output reg  [6:0] ev_data2
);

    reg [7:0] running;   // 0: no running status
    reg [6:0] d1;
    reg       have_d1;
    reg [1:0] skip;      // system-common data bytes still to discard

    wire one_data = (running[7:4] == 4'hC) || (running[7:4] == 4'hD);
    wire note_off = (running[7:4] == 4'h9) && (in_byte == 8'h00);

    always @(posedge clk) begin
        ev_valid <= 1'b0;

        if (!resetn) begin
            running   <= 8'h00;
            d1        <= 7'd0;
            have_d1   <= 1'b0;
            skip      <= 2'd0;
            ev_status <= 8'h00;
            ev_data1  <= 7'd0;
            ev_data2  <= 7'd0;
        end else if (in_valid && in_byte < 8'hF8) begin
            if (in_byte[7]) begin
                have_d1 <= 1'b0;
                if (in_byte < 8'hF0) begin
                    running <= in_byte;
                    skip    <= 2'd0;
                end else begin
                    running <= 8'h00;
                    case (in_byte)
                        8'hF1, 8'hF3: skip <= 2'd1;
                        8'hF2:        skip <= 2'd2;
                        default:      skip <= 2'd0;
                    endcase
                end
            end else if (skip != 2'd0) begin
                skip <= skip - 2'd1;
            end else if (running != 8'h00) begin
                if (one_data) begin
                    ev_valid  <= 1'b1;
                    ev_status <= running;
                    ev_data1  <= in_byte[6:0];
                    ev_data2  <= 7'd0;
                end else if (!have_d1) begin
                    d1      <= in_byte[6:0];
                    have_d1 <= 1'b1;
                end else begin
                    have_d1   <= 1'b0;
                    ev_valid  <= 1'b1;
                    ev_status <= note_off ? {4'h8, running[3:0]} : running;
                    ev_data1  <= d1;
                    ev_data2  <= in_byte[6:0];
                end
            end
        end
    end

endmodule
