`timescale 1ns/1ps
`include "usb_midi_defs.vh"

module usb_midi_sie (
    input  wire clk,
    input  wire rst,

    inout  wire usb_dp,
    inout  wire usb_dm,

    output wire sym_j,
    output wire sym_k,
    output wire sym_se0,

    input  wire force_se0,

    input  wire        tx_start,
    input  wire [7:0]  tx_pid,
    input  wire        tx_is_token,
    input  wire [6:0]  tx_addr,
    input  wire [3:0]  tx_endp,
    input  wire [63:0] tx_payload,
    input  wire [3:0]  tx_len,
    output reg         tx_done,
    output wire        tx_busy,

    input  wire        rx_enable,
    output reg         rx_active,
    output reg  [7:0]  rx_pid,
    output reg         rx_pid_valid,
    output reg         rx_byte_valid,
    output reg  [7:0]  rx_byte,
    output reg         rx_done,
    output reg         rx_ok
);

    reg [1:0] dp_sync, dm_sync;
    always @(posedge clk) begin
        dp_sync <= {dp_sync[0], usb_dp};
        dm_sync <= {dm_sync[0], usb_dm};
    end
    wire dp_s = dp_sync[1];
    wire dm_s = dm_sync[1];

    assign sym_j   = dp_s && !dm_s;
    assign sym_k   = !dp_s && dm_s;
    assign sym_se0 = !dp_s && !dm_s;

    reg       oe;
    reg       drive_j;
    reg       drive_se0;

    assign usb_dp = oe ? (drive_se0 ? 1'b0 : (drive_j ? 1'b1 : 1'b0)) : 1'bz;
    assign usb_dm = oe ? (drive_se0 ? 1'b0 : (drive_j ? 1'b0 : 1'b1)) : 1'bz;

    /* verilator lint_off BLKSEQ */
    function [4:0] crc5_upd(input [4:0] c, input b);
        reg t;
        begin
            t = c[0] ^ b;
            crc5_upd = c >> 1;
            if (t) crc5_upd = crc5_upd ^ `CRC5_POLY_REFL;
        end
    endfunction

    function [15:0] crc16_upd(input [15:0] c, input b);
        reg t;
        begin
            t = c[0] ^ b;
            crc16_upd = c >> 1;
            if (t) crc16_upd = crc16_upd ^ `CRC16_POLY_REFL;
        end
    endfunction
    /* verilator lint_on BLKSEQ */

    localparam [2:0] TXS_IDLE = 3'd0, TXS_BITS = 3'd1, TXS_EOP_SE0 = 3'd2,
                     TXS_EOP_J = 3'd3;

    reg [2:0]  txstate;
    reg [2:0]  tx_bitclk;
    reg [6:0]  tx_pos;
    reg [6:0]  tx_total;
    reg        tx_tok_lat;
    reg [6:0]  tx_addr_lat;
    reg [3:0]  tx_endp_lat;
    reg [7:0]  tx_pid_lat;
    reg [63:0] tx_payload_lat;
    reg [3:0]  tx_len_lat;
    reg [15:0] crc_run;
    reg [15:0] crc_final;
    reg [2:0]  ones_run;
    reg [1:0]  eop_bitclk_cnt;

    wire [6:0] cov_start = 7'd8;
    wire [6:0] cov_end_tok  = cov_start + 7'd10;
    wire [6:0] cov_end_data = cov_start + {3'b0,tx_len_lat}*7'd8 - 7'd1;

    /* verilator lint_off BLKSEQ */
    /* verilator lint_off WIDTHTRUNC */
    function get_bit(input [6:0] pos);
        reg [6:0] p;
        reg [3:0] crc_idx;
        begin
            if (pos < 7'd8) begin
                get_bit = (pos == 7'd7) ? 1'b1 : 1'b0;
            end else if (pos < 7'd16) begin
                p = pos - 7'd8;
                get_bit = tx_pid_lat[p[2:0]];
            end else if (tx_tok_lat) begin
                p = pos - 7'd16;
                if (p < 7'd7)         get_bit = tx_addr_lat[p[2:0]];
                else if (p < 7'd11)   get_bit = tx_endp_lat[p[2:0]-3'd7];
                else begin
                    crc_idx = (p-7'd11);
                    get_bit = crc_final[crc_idx];
                end
            end else begin
                p = pos - 7'd16;
                if (tx_len_lat != 4'd0 && p < {3'b0,tx_len_lat}*7'd8) begin
                    get_bit = tx_payload_lat[p[5:0]];
                end else begin
                    crc_idx = p - {3'b0,tx_len_lat}*7'd8;
                    get_bit = crc_final[crc_idx];
                end
            end
        end
    endfunction
    /* verilator lint_on WIDTHTRUNC */
    /* verilator lint_on BLKSEQ */

    wire tx_crc_covered = tx_tok_lat ? ((tx_pos >= cov_start) && (tx_pos <= cov_end_tok))
                                      : ((tx_len_lat != 4'd0) && (tx_pos >= cov_start) && (tx_pos <= cov_end_data));
    wire tx_crc_last_bit = tx_tok_lat ? (tx_pos == cov_end_tok) : (tx_pos == cov_end_data);

    assign tx_busy = (txstate != TXS_IDLE);

    always @(posedge clk) begin
        tx_done <= 1'b0;
        if (rst) begin
            txstate   <= TXS_IDLE;
            oe        <= 1'b0;
            drive_j   <= 1'b1;
            drive_se0 <= 1'b0;
        end else if (force_se0) begin
            txstate   <= TXS_IDLE;
            oe        <= 1'b1;
            drive_se0 <= 1'b1;
        end else begin
            case (txstate)
                TXS_IDLE: begin
                    oe        <= 1'b0;
                    drive_se0 <= 1'b0;
                    if (tx_start) begin
                        tx_pid_lat     <= tx_pid;
                        tx_tok_lat     <= tx_is_token;
                        tx_addr_lat    <= tx_addr;
                        tx_endp_lat    <= tx_endp;
                        tx_payload_lat <= tx_payload;
                        tx_len_lat     <= tx_len;
                        tx_total       <= tx_is_token ? 7'd40 :
                                          (7'd16 + {3'b0,tx_len}*7'd8 + 7'd16);
                        tx_pos      <= 7'd0;
                        ones_run    <= 3'd0;
                        crc_run     <= tx_is_token ? {11'b0,`CRC5_INIT} : {{0{1'b0}},`CRC16_INIT};
                        if (!tx_is_token && tx_len == 4'd0)
                            crc_final <= 16'h0000;
                        tx_bitclk   <= 3'd0;
                        oe          <= 1'b1;
                        drive_j     <= 1'b1;
                        drive_se0   <= 1'b0;
                        txstate     <= TXS_BITS;
                    end
                end

                TXS_BITS: begin
                    if (tx_bitclk == 3'd4) begin
                        tx_bitclk <= 3'd0;
                        if (tx_pos == tx_total) begin
                            txstate        <= TXS_EOP_SE0;
                            eop_bitclk_cnt <= 2'd0;
                            drive_se0      <= 1'b1;
                        end else if (ones_run == 3'd6) begin
                            drive_j  <= ~drive_j;
                            ones_run <= 3'd0;
                        end else begin
                            if (get_bit(tx_pos)) begin
                                ones_run <= ones_run + 3'd1;
                            end else begin
                                drive_j  <= ~drive_j;
                                ones_run <= 3'd0;
                            end
                            if (tx_crc_covered) begin
                                crc_run <= tx_tok_lat ? {11'b0, crc5_upd(crc_run[4:0], get_bit(tx_pos))}
                                                       : crc16_upd(crc_run, get_bit(tx_pos));
                                if (tx_crc_last_bit)
                                    crc_final <= tx_tok_lat
                                        ? {11'b0, (crc5_upd(crc_run[4:0], get_bit(tx_pos)) ^ `CRC5_XOROUT)}
                                        : (crc16_upd(crc_run, get_bit(tx_pos)) ^ `CRC16_XOROUT);
                            end
                            tx_pos <= tx_pos + 7'd1;
                        end
                    end else begin
                        tx_bitclk <= tx_bitclk + 3'd1;
                    end
                end

                TXS_EOP_SE0: begin
                    if (tx_bitclk == 3'd4) begin
                        tx_bitclk <= 3'd0;
                        if (eop_bitclk_cnt == 2'd1) begin
                            drive_se0 <= 1'b0;
                            drive_j   <= 1'b1;
                            txstate   <= TXS_EOP_J;
                        end else begin
                            eop_bitclk_cnt <= eop_bitclk_cnt + 2'd1;
                        end
                    end else begin
                        tx_bitclk <= tx_bitclk + 3'd1;
                    end
                end

                TXS_EOP_J: begin
                    if (tx_bitclk == 3'd4) begin
                        oe      <= 1'b0;
                        tx_done <= 1'b1;
                        txstate <= TXS_IDLE;
                    end else begin
                        tx_bitclk <= tx_bitclk + 3'd1;
                    end
                end

                default: txstate <= TXS_IDLE;
            endcase
        end
    end

    localparam [2:0] RXS_IDLE = 3'd0, RXS_SYNC = 3'd1, RXS_DATA = 3'd3,
                      RXS_EOP_SE0 = 3'd4, RXS_EOP_J = 3'd5;
    localparam EOP_SE0_HOLD = 4'd8;
    localparam EOP_J_HOLD   = 4'd4;

    reg [2:0] rxstate;
    reg [3:0] eop_cnt;
    reg [2:0] rx_bitclk;
    reg       prev_sym_j;
    reg       line_sym_j;
    reg [2:0] rx_ones_run;
    reg [3:0] skip_cnt;
    reg [2:0] pid_cnt;
    /* verilator lint_off UNUSEDSIGNAL */
    reg [7:0] pid_shift;
    reg [2:0] byte_cnt;
    reg [7:0] byte_shift;
    /* verilator lint_on UNUSEDSIGNAL */
    reg       is_data_pid;
    reg [15:0] rx_crc;
    reg [7:0] pipe_b0, pipe_b1;
    reg [1:0] pipe_n;
    reg       stuff_err;
    reg [7:0] rcvd_bytes;
    reg       bit_dec;

    wire cur_sym_j = sym_j;
    wire cur_sym_k = sym_k;

    always @(posedge clk) begin
        rx_pid_valid  <= 1'b0;
        rx_byte_valid <= 1'b0;
        rx_done       <= 1'b0;

        if (rst) begin
            rxstate   <= RXS_IDLE;
            rx_active <= 1'b0;
        end else if (!rx_enable) begin
            rxstate   <= RXS_IDLE;
            rx_active <= 1'b0;
        end else begin
            case (rxstate)
                RXS_IDLE: begin
                    rx_active <= 1'b0;
                    if (cur_sym_k) begin
                        rxstate     <= RXS_SYNC;
                        rx_active   <= 1'b1;
                        rx_bitclk   <= 3'd0;
                        prev_sym_j  <= 1'b0;
                        line_sym_j  <= 1'b0;
                        skip_cnt    <= 4'd0;
                        pid_cnt     <= 3'd0;
                        rx_ones_run <= 3'd0;
                        stuff_err   <= 1'b0;
                        pipe_n      <= 2'd0;
                        rcvd_bytes  <= 8'd0;
                        rx_crc      <= `CRC16_INIT;
                    end
                end

                default: begin
                    if (rxstate == RXS_EOP_SE0) begin
                        if (cur_sym_j == 1'b0 && cur_sym_k == 1'b0) begin
                            if (eop_cnt != EOP_SE0_HOLD) eop_cnt <= eop_cnt + 4'd1;
                        end else if (eop_cnt >= EOP_SE0_HOLD) begin
                            eop_cnt <= 4'd1;
                            rxstate <= RXS_EOP_J;
                        end else begin
                            rx_done   <= 1'b1;
                            rx_ok     <= 1'b0;
                            rxstate   <= RXS_IDLE;
                            rx_active <= 1'b0;
                            eop_cnt   <= 4'd0;
                        end
                    end else if (rxstate == RXS_EOP_J) begin
                        if (cur_sym_j == 1'b1 && cur_sym_k == 1'b0) begin
                            if (eop_cnt >= EOP_J_HOLD) begin
                                rx_done   <= 1'b1;
                                rx_ok     <= !stuff_err &&
                                             (byte_cnt == 3'd0) &&
                                             (!is_data_pid || (rx_crc == `CRC16_GOOD_RESID && rcvd_bytes >= 8'd2));
                                rxstate   <= RXS_IDLE;
                                rx_active <= 1'b0;
                                eop_cnt   <= 4'd0;
                            end else begin
                                eop_cnt <= eop_cnt + 4'd1;
                            end
                        end else begin
                            rx_done   <= 1'b1;
                            rx_ok     <= !stuff_err &&
                                         (byte_cnt == 3'd0) &&
                                         (!is_data_pid || (rx_crc == `CRC16_GOOD_RESID && rcvd_bytes >= 8'd2));
                            rxstate   <= RXS_IDLE;
                            rx_active <= 1'b0;
                            eop_cnt   <= 4'd0;
                        end
                    end else if (cur_sym_j == 1'b0 && cur_sym_k == 1'b0) begin
                        rxstate <= RXS_EOP_SE0;
                        eop_cnt <= 4'd1;
                    end else begin
                        if (cur_sym_j != line_sym_j && rx_bitclk != 3'd0 && rx_bitclk != 3'd1 && rx_bitclk != 3'd4) begin
                            rx_bitclk <= 3'd1;
                        end else if (rx_bitclk == 3'd4) begin
                            rx_bitclk <= 3'd0;
                        end else begin
                            rx_bitclk <= rx_bitclk + 3'd1;
                        end
                        line_sym_j <= cur_sym_j;

                        if (rx_bitclk == 3'd3) begin
                            /* verilator lint_off BLKSEQ */
                            bit_dec    = (cur_sym_j == prev_sym_j);
                            /* verilator lint_on BLKSEQ */
                            prev_sym_j <= cur_sym_j;

                            if (skip_cnt != 4'd8) begin
                                skip_cnt <= skip_cnt + 4'd1;
                            end else if (rx_ones_run == 3'd6) begin
                                if (bit_dec) stuff_err <= 1'b1;
                                rx_ones_run <= 3'd0;
                            end else begin
                                if (bit_dec) rx_ones_run <= rx_ones_run + 3'd1;
                                else         rx_ones_run <= 3'd0;

                                if (rxstate == RXS_SYNC) begin
                                    pid_shift <= {bit_dec, pid_shift[7:1]};
                                    pid_cnt   <= pid_cnt + 3'd1;
                                    if (pid_cnt == 3'd7) begin
                                        rx_pid       <= {bit_dec, pid_shift[7:1]};
                                        rx_pid_valid <= 1'b1;
                                        is_data_pid  <= ({bit_dec, pid_shift[7:1]} == `USB_PID_DATA0) ||
                                                        ({bit_dec, pid_shift[7:1]} == `USB_PID_DATA1);
                                        byte_cnt <= 3'd0;
                                        rxstate  <= RXS_DATA;
                                    end
                                end else begin
                                    byte_shift <= {bit_dec, byte_shift[7:1]};
                                    byte_cnt   <= byte_cnt + 3'd1;
                                    rx_crc     <= crc16_upd(rx_crc, bit_dec);
                                    if (byte_cnt == 3'd7) begin
                                        rcvd_bytes <= rcvd_bytes + 8'd1;
                                        if (pipe_n == 2'd2) begin
                                            rx_byte_valid <= 1'b1;
                                            rx_byte       <= pipe_b1;
                                            pipe_b1       <= pipe_b0;
                                            pipe_b0       <= {bit_dec, byte_shift[7:1]};
                                        end else begin
                                            pipe_b1  <= pipe_b0;
                                            pipe_b0  <= {bit_dec, byte_shift[7:1]};
                                            pipe_n   <= pipe_n + 2'd1;
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            endcase
        end
    end

endmodule
