`timescale 1ns/1ps

// Philips-format I2S transmitter for a PCM5102A (SCK tied low, internal PLL).
//
// Frame: 64 BCLK periods, two 32-bit slots, left while LRCLK is low. Each
// 16-bit sample is sent MSB first, one BCLK after the LRCLK edge, followed by
// zero padding. SDATA and LRCLK change on the BCLK falling edge.
//
// With CLKS_PER_HALF_BCLK = 6 a frame is 768 clocks, exactly the S-DSP output
// period, so the transmitter runs in the DSP clock domain and can never under-
// or overrun: each frame sends the most recent sample latched on sample_valid.
module i2s_tx #(
    parameter integer CLKS_PER_HALF_BCLK = 6
) (
    input  wire        clk,
    input  wire        resetn,
    input  wire        sample_valid,
    input  wire [15:0] sample_l,
    input  wire [15:0] sample_r,
    output reg         bclk,
    output reg         lrclk,
    output reg         sdata
);

    localparam integer DIV_W = (CLKS_PER_HALF_BCLK > 1) ? $clog2(CLKS_PER_HALF_BCLK) : 1;
    localparam integer     LAST_INT = CLKS_PER_HALF_BCLK - 1;
    localparam [DIV_W-1:0] DIV_LAST = LAST_INT[DIV_W-1:0];

    reg [DIV_W-1:0] div;
    reg [5:0]       bit_cnt;
    reg [15:0]      hold_l, hold_r;
    reg [15:0]      frame_l, frame_r;

    // bit_cnt is the frame position of the bit about to be driven.
    wire [15:0] slot_word = bit_cnt[5] ? frame_r : frame_l;
    wire        slot_bit  = (bit_cnt[4] == 1'b0) ? slot_word[4'd15 - bit_cnt[3:0]] : 1'b0;
    wire [5:0]  next_cnt  = bit_cnt + 6'd1;

    always @(posedge clk) begin
        if (!resetn) begin
            div     <= {DIV_W{1'b0}};
            bclk    <= 1'b0;
            lrclk   <= 1'b1;
            sdata   <= 1'b0;
            bit_cnt <= 6'd63;
            hold_l  <= 16'h0000;
            hold_r  <= 16'h0000;
            frame_l <= 16'h0000;
            frame_r <= 16'h0000;
        end else begin
            if (sample_valid) begin
                hold_l <= sample_l;
                hold_r <= sample_r;
            end

            if (div == DIV_LAST) begin
                div  <= {DIV_W{1'b0}};
                bclk <= ~bclk;

                if (bclk) begin
                    // Falling edge: position 63 is padding, so the frame
                    // words can be replaced while it is being driven.
                    bit_cnt <= next_cnt;
                    lrclk   <= next_cnt[5];
                    sdata   <= slot_bit;
                    if (bit_cnt == 6'd63) begin
                        frame_l <= hold_l;
                        frame_r <= hold_r;
                    end
                end
            end else begin
                div <= div + 1'b1;
            end
        end
    end

endmodule
