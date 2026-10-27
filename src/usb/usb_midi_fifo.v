`timescale 1ns/1ps

// Dual-clock byte FIFO with commit/rollback on the write side.
//
// The USB side writes the MIDI bytes of a bulk packet as they arrive, but
// the packet's CRC and data toggle are only known at its end: `commit`
// makes the bytes written since the last commit visible to the reader,
// `rollback` discards them. The published write pointer then walks to the
// committed one a step per clock, so its Gray code changes one bit at a
// time and crosses to the read clock through a plain 2-FF synchronizer.
//
// Bytes that do not fit are dropped (`overflow` sticks until reset).
module usb_midi_fifo #(
    parameter integer AW = 8                    // 2**AW bytes
) (
    // write side (USB clock)
    input  wire       wclk,
    input  wire       wrst,
    input  wire       we,
    input  wire [7:0] wdata,
    input  wire       commit,
    input  wire       rollback,
    output reg        overflow,

    // read side
    input  wire       rclk,
    input  wire       rresetn,
    input  wire       ready,                   // reader can take a byte
    output reg        valid,                   // one-cycle strobe with data
    output reg  [7:0] data
);
    reg [7:0] mem [0:(1 << AW) - 1];
    reg [AW:0] rptr, rgray_q;           // read pointer (binary, Gray)
    reg [AW:0] wgray_s1, wgray_s2;      // published write pointer, synchronized

    function [AW:0] bin2gray(input [AW:0] b);
        bin2gray = b ^ (b >> 1);
    endfunction

    function [AW:0] gray2bin(input [AW:0] g);
        integer i;
        begin
            gray2bin[AW] = g[AW];
            for (i = AW - 1; i >= 0; i = i - 1)
                gray2bin[i] = gray2bin[i + 1] ^ g[i];
        end
    endfunction

    // ---------------- write side ----------------
    reg  [AW:0] wptr_tent, wptr_commit, wptr_pub;
    reg  [AW:0] wgray;
    reg  [AW:0] rgray_s1, rgray_s2;
    wire [AW:0] rptr_w = gray2bin(rgray_s2);
    wire        full = (wptr_tent - rptr_w) == (1 << AW);

    always @(posedge wclk) begin
        rgray_s1 <= rgray_q;
        rgray_s2 <= rgray_s1;
        if (wrst) begin
            wptr_tent   <= {(AW + 1){1'b0}};
            wptr_commit <= {(AW + 1){1'b0}};
            wptr_pub    <= {(AW + 1){1'b0}};
            wgray       <= {(AW + 1){1'b0}};
            overflow    <= 1'b0;
        end else begin
            if (rollback) begin
                wptr_tent <= wptr_commit;
            end else begin
                if (we) begin
                    if (full)
                        overflow <= 1'b1;
                    else
                        wptr_tent <= wptr_tent + 1'b1;
                end
                if (commit)
                    wptr_commit <= we && !full ? wptr_tent + 1'b1 : wptr_tent;
            end
            if (wptr_pub != wptr_commit)
                wptr_pub <= wptr_pub + 1'b1;
            wgray <= bin2gray(wptr_pub);
        end
    end

    always @(posedge wclk)
        if (we && !full && !rollback)
            mem[wptr_tent[AW-1:0]] <= wdata;

    // ---------------- read side ----------------
    reg         rd_pending;
    reg  [7:0]  q;
    wire [AW:0] wptr_r = gray2bin(wgray_s2);
    wire        empty = rptr == wptr_r;

    always @(posedge rclk)
        q <= mem[rptr[AW-1:0]];

    always @(posedge rclk) begin
        wgray_s1 <= wgray;
        wgray_s2 <= wgray_s1;
        valid    <= 1'b0;
        if (!rresetn) begin
            rptr       <= {(AW + 1){1'b0}};
            rgray_q    <= {(AW + 1){1'b0}};
            rd_pending <= 1'b0;
        end else if (rd_pending) begin
            // q now holds mem[rptr] (read issued last cycle)
            valid      <= 1'b1;
            data       <= q;
            rptr       <= rptr + 1'b1;
            rgray_q    <= bin2gray(rptr + 1'b1);
            rd_pending <= 1'b0;
        end else if (!empty && ready) begin
            rd_pending <= 1'b1;
        end
    end
endmodule
