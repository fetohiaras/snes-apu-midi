`timescale 1ns/1ps

// Streams the APU output to a PC over a UART (8N1), for listening without
// the DAC. The mono mix m = floor((L + R) / 2) is sent in one of two formats:
//
//   FORMAT = 0 (PCM14): every sample as two self-synchronising bytes with its
//     top 14 bits, byte 0 = 1 m[15:9], byte 1 = 0 m[8:2]. Needs >= ~800 kbaud.
//   FORMAT = 1 (ULAW8): the average of every DECIM samples (power of 2) as one
//     G.711 mu-law byte. DECIM = 4 gives 7984 Hz, which fits 115200 baud.
//
// Each byte is followed by one idle bit. `overrun` flags a sample that
// arrived while the previous one was still being sent (BAUD too low).
module pc_audio_tx #(
    parameter integer CLK_HZ = 24528000,
    parameter integer BAUD   = 115200,
    parameter integer FORMAT = 1,
    parameter integer DECIM  = 4
) (
    input  wire        clk,
    input  wire        resetn,
    input  wire        sample_valid,
    input  wire [15:0] sample_l,
    input  wire [15:0] sample_r,
    output reg         tx,
    output reg         overrun,
    output wire        busy
);

    // Fractional baud generator: one tick per bit, exact on average.
    localparam [31:0] STEP = BAUD;
    localparam [31:0] WRAP = CLK_HZ;
    reg  [31:0] acc;
    wire [31:0] acc_next = acc + STEP;
    wire        tick = acc_next >= WRAP;

    // floor((L + R) / 2); bit 0 of the sum is dropped on purpose.
    /* verilator lint_off UNUSEDSIGNAL */
    wire [16:0] sum  = {sample_l[15], sample_l} + {sample_r[15], sample_r};
    /* verilator lint_on UNUSEDSIGNAL */
    wire [15:0] mono = sum[16:1];

    // ------------------------------------------------------------------
    // Decimation (ULAW8): floor of the average of DECIM mono samples
    // ------------------------------------------------------------------
    localparam integer DSHIFT = (DECIM > 1) ? $clog2(DECIM) : 0;
    localparam integer ACC_W  = 16 + DSHIFT;
    localparam integer CNT_W  = (DSHIFT > 0) ? DSHIFT : 1;

    reg  [ACC_W-1:0] dec_acc;
    reg  [CNT_W-1:0] dec_cnt;
    wire [ACC_W-1:0] dec_sum = dec_acc + {{DSHIFT{mono[15]}}, mono};
    /* verilator lint_off UNUSEDSIGNAL */
    wire [ACC_W-1:0] dec_shifted = $signed(dec_sum) >>> DSHIFT;
    /* verilator lint_on UNUSEDSIGNAL */
    wire [15:0]      dec_avg = dec_shifted[15:0];
    wire             dec_last = (DSHIFT == 0) || (dec_cnt == {CNT_W{1'b1}});

    // ------------------------------------------------------------------
    // G.711 mu-law encoder (as in the reference g711.c linear2ulaw)
    // ------------------------------------------------------------------
    // x[1:0] and the bits of t above the mantissa are dropped by design.
    /* verilator lint_off UNUSEDSIGNAL */
    function [7:0] ulaw;
        input [15:0] x;
        reg        neg;
        reg [13:0] v;       // |x >> 2|, clipped to 8158, plus bias 33
        reg [13:0] t;
        reg [2:0]  seg;
        integer    i;
        begin
            neg = x[15];
            v   = neg ? (~x[15:2] + 14'd1) : x[15:2];
            if (v > 14'd8158)
                v = 14'd8158;
            v   = v + 14'd33;
            seg = 3'd0;
            for (i = 1; i < 8; i = i + 1)
                if (v[i + 5])
                    seg = i[2:0];
            t    = v >> ({1'b0, seg} + 4'd1);
            ulaw = {~neg, ~seg, ~t[3:0]};
        end
    endfunction
    /* verilator lint_on UNUSEDSIGNAL */

    wire [7:0] dec_ulaw = ulaw(dec_avg);

    // ------------------------------------------------------------------
    // Byte transmitter
    // ------------------------------------------------------------------
    reg [9:0] shift;     // {stop, data, start}, sent LSB first
    reg [3:0] nbits;
    reg [7:0] byte0, byte1;
    reg [1:0] pending;   // bytes still to start: 2 = byte0 then byte1, 1 = byte1

    assign busy = (nbits != 4'd0) || (pending != 2'd0);

    always @(posedge clk) begin
        if (!resetn) begin
            acc     <= 32'd0;
            tx      <= 1'b1;
            overrun <= 1'b0;
            shift   <= 10'h3FF;
            nbits   <= 4'd0;
            byte0   <= 8'h00;
            byte1   <= 8'h00;
            pending <= 2'd0;
            dec_acc <= {ACC_W{1'b0}};
            dec_cnt <= {CNT_W{1'b0}};
        end else begin
            acc <= tick ? acc_next - WRAP : acc_next;

            if (sample_valid) begin
                if (FORMAT == 0) begin
                    if (nbits != 4'd0 || pending != 2'd0) begin
                        overrun <= 1'b1;
                    end else begin
                        byte0   <= {1'b1, mono[15:9]};
                        byte1   <= {1'b0, mono[8:2]};
                        pending <= 2'd2;
                    end
                end else begin
                    dec_cnt <= dec_cnt + 1'b1;
                    dec_acc <= dec_last ? {ACC_W{1'b0}} : dec_sum;
                    if (dec_last) begin
                        if (nbits != 4'd0 || pending != 2'd0) begin
                            overrun <= 1'b1;
                        end else begin
                            byte1   <= dec_ulaw;
                            pending <= 2'd1;
                        end
                    end
                end
            end

            if (tick) begin
                if (nbits != 4'd0) begin
                    tx    <= shift[0];
                    shift <= {1'b1, shift[9:1]};
                    nbits <= nbits - 4'd1;
                end else if (pending != 2'd0) begin
                    // Loading takes one bit time with the line idle (high).
                    shift   <= {1'b1, (pending == 2'd2) ? byte0 : byte1, 1'b0};
                    nbits   <= 4'd10;
                    pending <= pending - 2'd1;
                end
            end
        end
    end

endmodule
