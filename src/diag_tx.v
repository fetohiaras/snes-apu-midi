`timescale 1ns/1ps

// Sends an N-byte packet (byte 0 = data[7:0]) over a UART, 8N1 with one idle
// bit after each byte, when `start` pulses while idle.
module diag_tx #(
    parameter integer CLK_HZ = 24528000,
    parameter integer BAUD   = 115200,
    parameter integer N      = 22
) (
    input  wire           clk,
    input  wire           resetn,
    input  wire           start,
    input  wire [8*N-1:0] data,
    output reg            tx,
    output wire           busy
);

    localparam [31:0] STEP = BAUD;
    localparam [31:0] WRAP = CLK_HZ;
    localparam integer IW  = $clog2(N + 1);
    localparam [IW-1:0] N_BYTES = N[IW-1:0];

    reg  [31:0]    acc;
    wire [31:0]    acc_next = acc + STEP;
    wire           tick = acc_next >= WRAP;

    reg  [8*N-1:0] buffer;
    reg  [IW-1:0]  remaining;   // bytes still to load
    reg  [9:0]     shift;
    reg  [3:0]     nbits;

    assign busy = (remaining != {IW{1'b0}}) || (nbits != 4'd0);

    always @(posedge clk) begin
        if (!resetn) begin
            acc       <= 32'd0;
            tx        <= 1'b1;
            buffer    <= {8*N{1'b0}};
            remaining <= {IW{1'b0}};
            shift     <= 10'h3FF;
            nbits     <= 4'd0;
        end else begin
            acc <= tick ? acc_next - WRAP : acc_next;

            if (start && !busy) begin
                buffer    <= data;
                remaining <= N_BYTES;
            end

            if (tick) begin
                if (nbits != 4'd0) begin
                    tx    <= shift[0];
                    shift <= {1'b1, shift[9:1]};
                    nbits <= nbits - 4'd1;
                end else if (remaining != {IW{1'b0}}) begin
                    shift     <= {1'b1, buffer[7:0], 1'b0};
                    buffer    <= buffer >> 8;
                    nbits     <= 4'd10;
                    remaining <= remaining - 1'b1;
                end
            end
        end
    end

endmodule
