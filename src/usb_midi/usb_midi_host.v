`timescale 1ns/1ps
`include "usb_midi_defs.vh"

module usb_midi_host #(
    parameter integer RESET_HOLD_CYCLES   = 32'd900_000,
    parameter integer SETTLE_CYCLES       = 32'd60_000,
    parameter integer POST_CTL_DELAY_CYCLES = 32'd120_000,
    parameter integer TIMEOUT_CYCLES      = 32'd60_000,
    parameter integer POLL_GAP_CYCLES     = 32'd600,
    parameter integer MAX_RETRIES         = 8,
    parameter [6:0]   NEW_ADDR            = 7'd1,
    parameter [3:0]   MIDI_BULK_IN_EP     = 4'd1
) (
    input  wire clk,
    input  wire resetn,

    inout  wire usb_dp,
    inout  wire usb_dm,

    output reg        pkt_valid,
    output reg [7:0]  pkt_cin_cable,
    output reg [7:0]  pkt_b0,
    output reg [7:0]  pkt_b1,
    output reg [7:0]  pkt_b2,

    output reg        dev_configured,
    output reg        dev_error,
    output reg [15:0] poll_count,
    output reg [15:0] nak_count
);

    reg         tx_start;
    reg  [7:0]  tx_pid;
    reg         tx_is_token;
    reg  [6:0]  tx_addr_r;
    reg  [3:0]  tx_endp_r;
    reg  [63:0] tx_payload;
    reg  [3:0]  tx_len;
    wire        tx_done;
    /* verilator lint_off UNUSEDSIGNAL */
    wire        tx_busy;

    reg         rx_enable;
    wire        rx_active;
    wire [7:0]  rx_pid;
    wire        rx_pid_valid;
    /* verilator lint_on UNUSEDSIGNAL */
    wire        rx_byte_valid;
    wire [7:0]  rx_byte;
    wire        rx_done;
    wire        rx_ok;

    reg         force_se0;

    /* verilator lint_off PINCONNECTEMPTY */
    usb_midi_sie u_sie (
        .clk          (clk),
        .rst          (~resetn),
        .usb_dp       (usb_dp),
        .usb_dm       (usb_dm),
        .sym_j        (),
        .sym_k        (),
        .sym_se0      (),
        .force_se0    (force_se0),
        .tx_start     (tx_start),
        .tx_pid       (tx_pid),
        .tx_is_token  (tx_is_token),
        .tx_addr      (tx_addr_r),
        .tx_endp      (tx_endp_r),
        .tx_payload   (tx_payload),
        .tx_len       (tx_len),
        .tx_done      (tx_done),
        .tx_busy      (tx_busy),
        .rx_enable    (rx_enable),
        .rx_active    (rx_active),
        .rx_pid       (rx_pid),
        .rx_pid_valid (rx_pid_valid),
        .rx_byte_valid(rx_byte_valid),
        .rx_byte      (rx_byte),
        .rx_done      (rx_done),
        .rx_ok        (rx_ok)
    );

    wire [63:0] REQ_SET_ADDRESS = {16'h0000, 16'h0000, 8'h00, {1'b0, NEW_ADDR}, 8'h05, 8'h00};
    wire [63:0] REQ_SET_CONFIG  = {16'h0000, 16'h0000, 8'h00, 8'h01,            8'h09, 8'h00};

    localparam [4:0]
        S_RST_ASSERT        = 5'd0,
        S_RST_RELEASE       = 5'd1,
        S_CTL_LOAD          = 5'd2,
        S_CTL_SETUP_TX      = 5'd3,
        S_CTL_SETUP_TXWAIT  = 5'd4,
        S_CTL_SETUP_DATA_TX = 5'd5,
        S_CTL_SETUP_DATAWAIT= 5'd6,
        S_CTL_SETUP_ACKWAIT = 5'd7,
        S_CTL_STATUS_TOK_TX = 5'd8,
        S_CTL_STATUS_TOKWAIT= 5'd9,
        S_CTL_STATUS_RXWAIT = 5'd10,
        S_CTL_STATUS_ACK_TX = 5'd11,
        S_CTL_STATUS_ACKWAIT= 5'd12,
        S_CTL_POST_DELAY    = 5'd13,
        S_POLL_TOK_TX       = 5'd14,
        S_POLL_TOK_WAIT     = 5'd15,
        S_POLL_RXWAIT       = 5'd16,
        S_POLL_ACK_TX       = 5'd17,
        S_POLL_ACK_WAIT     = 5'd18,
        S_POLL_PROCESS      = 5'd19,
        S_POLL_GAP          = 5'd20,
        S_ERROR_HALT        = 5'd21;

    reg [4:0]  state;
    reg [31:0] wait_cnt;
    reg [3:0]  retry_cnt;
    reg        ctl_which;
    reg [6:0]  tok_addr;
    reg        data_toggle;

    reg [7:0]  rxbuf [0:63];
    reg [6:0]  rxbuf_cnt;
    reg [6:0]  proc_idx;
    wire [5:0] proc_idx_p1 = proc_idx[5:0] + 6'd1;
    wire [5:0] proc_idx_p2 = proc_idx[5:0] + 6'd2;
    wire [5:0] proc_idx_p3 = proc_idx[5:0] + 6'd3;

    integer rb;

    always @(posedge clk) begin
        pkt_valid <= 1'b0;
        tx_start  <= 1'b0;
        force_se0 <= 1'b0;

        if (!resetn) begin
            state          <= S_RST_ASSERT;
            wait_cnt       <= 32'd0;
            retry_cnt      <= 4'd0;
            ctl_which      <= 1'b0;
            tok_addr       <= 7'd0;
            data_toggle    <= 1'b0;
            rx_enable      <= 1'b0;
            rxbuf_cnt      <= 7'd0;
            proc_idx       <= 7'd0;
            dev_configured <= 1'b0;
            dev_error      <= 1'b0;
            poll_count     <= 16'd0;
            nak_count      <= 16'd0;
            for (rb = 0; rb < 64; rb = rb + 1) rxbuf[rb] <= 8'h00;
        end else begin
            case (state)
                S_RST_ASSERT: begin
                    force_se0 <= 1'b1;
                    rx_enable <= 1'b0;
                    if (wait_cnt >= RESET_HOLD_CYCLES) begin
                        wait_cnt <= 32'd0;
                        state    <= S_RST_RELEASE;
                    end else begin
                        wait_cnt <= wait_cnt + 32'd1;
                    end
                end

                S_RST_RELEASE: begin
                    if (wait_cnt >= SETTLE_CYCLES) begin
                        wait_cnt   <= 32'd0;
                        ctl_which  <= 1'b0;
                        tok_addr   <= 7'd0;
                        retry_cnt  <= 4'd0;
                        state      <= S_CTL_LOAD;
                    end else begin
                        wait_cnt <= wait_cnt + 32'd1;
                    end
                end

                S_CTL_LOAD: begin
                    tx_payload  <= ctl_which ? REQ_SET_CONFIG : REQ_SET_ADDRESS;
                    state       <= S_CTL_SETUP_TX;
                end

                S_CTL_SETUP_TX: begin
                    tx_start    <= 1'b1;
                    tx_pid      <= `USB_PID_SETUP;
                    tx_is_token <= 1'b1;
                    tx_addr_r   <= tok_addr;
                    tx_endp_r   <= 4'd0;
                    state       <= S_CTL_SETUP_TXWAIT;
                end

                S_CTL_SETUP_TXWAIT: if (tx_done) state <= S_CTL_SETUP_DATA_TX;

                S_CTL_SETUP_DATA_TX: begin
                    tx_start    <= 1'b1;
                    tx_pid      <= `USB_PID_DATA0;
                    tx_is_token <= 1'b0;
                    tx_len      <= 4'd8;
                    state       <= S_CTL_SETUP_DATAWAIT;
                end

                S_CTL_SETUP_DATAWAIT: if (tx_done) begin
                    wait_cnt  <= 32'd0;
                    rx_enable <= 1'b1;
                    state     <= S_CTL_SETUP_ACKWAIT;
                end

                S_CTL_SETUP_ACKWAIT: begin
                    if (rx_done) begin
                        rx_enable <= 1'b0;
                        if (rx_ok && rx_pid == `USB_PID_ACK) begin
                            state <= S_CTL_STATUS_TOK_TX;
                        end else if (retry_cnt >= MAX_RETRIES[3:0]) begin
                            dev_error <= 1'b1;
                            state     <= S_ERROR_HALT;
                        end else begin
                            retry_cnt <= retry_cnt + 4'd1;
                            state     <= S_CTL_LOAD;
                        end
                    end else if (wait_cnt >= TIMEOUT_CYCLES) begin
                        rx_enable <= 1'b0;
                        if (retry_cnt >= MAX_RETRIES[3:0]) begin
                            dev_error <= 1'b1;
                            state     <= S_ERROR_HALT;
                        end else begin
                            retry_cnt <= retry_cnt + 4'd1;
                            state     <= S_CTL_LOAD;
                        end
                    end else begin
                        wait_cnt <= wait_cnt + 32'd1;
                    end
                end

                S_CTL_STATUS_TOK_TX: begin
                    tx_start    <= 1'b1;
                    tx_pid      <= `USB_PID_IN;
                    tx_is_token <= 1'b1;
                    tx_addr_r   <= tok_addr;
                    tx_endp_r   <= 4'd0;
                    state       <= S_CTL_STATUS_TOKWAIT;
                end

                S_CTL_STATUS_TOKWAIT: if (tx_done) begin
                    wait_cnt  <= 32'd0;
                    rx_enable <= 1'b1;
                    state     <= S_CTL_STATUS_RXWAIT;
                end

                S_CTL_STATUS_RXWAIT: begin
                    if (rx_done) begin
                        rx_enable <= 1'b0;
                        if (rx_ok && (rx_pid == `USB_PID_DATA1)) begin
                            state <= S_CTL_STATUS_ACK_TX;
                        end else if (rx_ok && rx_pid == `USB_PID_NAK) begin
                            state <= S_CTL_STATUS_TOK_TX;
                        end else if (retry_cnt >= MAX_RETRIES[3:0]) begin
                            dev_error <= 1'b1;
                            state     <= S_ERROR_HALT;
                        end else begin
                            retry_cnt <= retry_cnt + 4'd1;
                            state     <= S_CTL_LOAD;
                        end
                    end else if (wait_cnt >= TIMEOUT_CYCLES) begin
                        rx_enable <= 1'b0;
                        state     <= S_CTL_STATUS_TOK_TX;
                    end else begin
                        wait_cnt <= wait_cnt + 32'd1;
                    end
                end

                S_CTL_STATUS_ACK_TX: begin
                    tx_start    <= 1'b1;
                    tx_pid      <= `USB_PID_ACK;
                    tx_is_token <= 1'b0;
                    tx_len      <= 4'd0;
                    state       <= S_CTL_STATUS_ACKWAIT;
                end

                S_CTL_STATUS_ACKWAIT: if (tx_done) begin
                    wait_cnt <= 32'd0;
                    state    <= S_CTL_POST_DELAY;
                end

                S_CTL_POST_DELAY: begin
                    if (wait_cnt >= POST_CTL_DELAY_CYCLES) begin
                        wait_cnt <= 32'd0;
                        if (!ctl_which) begin
                            tok_addr  <= NEW_ADDR;
                            ctl_which <= 1'b1;
                            retry_cnt <= 4'd0;
                            state     <= S_CTL_LOAD;
                        end else begin
                            dev_configured <= 1'b1;
                            data_toggle    <= 1'b0;
                            state          <= S_POLL_TOK_TX;
                        end
                    end else begin
                        wait_cnt <= wait_cnt + 32'd1;
                    end
                end

                S_POLL_TOK_TX: begin
                    tx_start    <= 1'b1;
                    tx_pid      <= `USB_PID_IN;
                    tx_is_token <= 1'b1;
                    tx_addr_r   <= tok_addr;
                    tx_endp_r   <= MIDI_BULK_IN_EP;
                    rxbuf_cnt   <= 7'd0;
                    state       <= S_POLL_TOK_WAIT;
                end

                S_POLL_TOK_WAIT: if (tx_done) begin
                    poll_count <= poll_count + 16'd1;
                    wait_cnt   <= 32'd0;
                    rx_enable  <= 1'b1;
                    state      <= S_POLL_RXWAIT;
                end

                S_POLL_RXWAIT: begin
                    if (rx_byte_valid && rxbuf_cnt < 7'd64) begin
                        rxbuf[rxbuf_cnt[5:0]] <= rx_byte;
                        rxbuf_cnt             <= rxbuf_cnt + 7'd1;
                    end
                    if (rx_done) begin
                        rx_enable <= 1'b0;
                        if (rx_ok && rx_pid == `USB_PID_NAK) begin
                            nak_count <= nak_count + 16'd1;
                            state     <= S_POLL_GAP;
                        end else if (rx_ok && rx_pid == `USB_PID_STALL) begin
                            dev_error <= 1'b1;
                            state     <= S_ERROR_HALT;
                        end else if (rx_ok &&
                                     (rx_pid == (data_toggle ? `USB_PID_DATA1 : `USB_PID_DATA0))) begin
                            state <= S_POLL_ACK_TX;
                        end else begin
                            state <= S_POLL_GAP;
                        end
                    end else if (wait_cnt >= TIMEOUT_CYCLES) begin
                        rx_enable <= 1'b0;
                        state     <= S_POLL_GAP;
                    end else begin
                        wait_cnt <= wait_cnt + 32'd1;
                    end
                end

                S_POLL_ACK_TX: begin
                    tx_start    <= 1'b1;
                    tx_pid      <= `USB_PID_ACK;
                    tx_is_token <= 1'b0;
                    tx_len      <= 4'd0;
                    state       <= S_POLL_ACK_WAIT;
                end

                S_POLL_ACK_WAIT: if (tx_done) begin
                    data_toggle <= ~data_toggle;
                    proc_idx    <= 7'd0;
                    state       <= S_POLL_PROCESS;
                end

                S_POLL_PROCESS: begin
                    if (proc_idx + 7'd4 <= rxbuf_cnt) begin
                        pkt_valid     <= 1'b1;
                        pkt_cin_cable <= rxbuf[proc_idx[5:0]];
                        pkt_b0        <= rxbuf[proc_idx_p1];
                        pkt_b1        <= rxbuf[proc_idx_p2];
                        pkt_b2        <= rxbuf[proc_idx_p3];
                        proc_idx      <= proc_idx + 7'd4;
                        state         <= S_POLL_PROCESS;
                    end else begin
                        state <= S_POLL_GAP;
                    end
                end

                S_POLL_GAP: begin
                    if (wait_cnt >= POLL_GAP_CYCLES) begin
                        wait_cnt <= 32'd0;
                        state    <= S_POLL_TOK_TX;
                    end else begin
                        wait_cnt <= wait_cnt + 32'd1;
                    end
                end

                S_ERROR_HALT: begin
                    rx_enable <= 1'b0;
                end

                default: state <= S_RST_ASSERT;
            endcase
        end
    end

endmodule
