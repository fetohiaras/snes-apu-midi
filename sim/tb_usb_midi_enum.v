`timescale 1ns/1ps
`include "usb_midi_defs.vh"

module tb_usb_midi_enum (
    input  wire        clk,
    input  wire        resetn,
    output wire        dev_configured,
    output wire        dev_error,
    output wire [15:0] poll_count,
    output wire [15:0] nak_count,
    output wire [3:0]  pkt_count_dbg
);

    wire usb_dp, usb_dm;

    pullup(usb_dp);
    pulldown(usb_dm);

    /* verilator lint_off PINCONNECTEMPTY */
    usb_midi_host #(
        .RESET_HOLD_CYCLES(2000),
        .SETTLE_CYCLES(500),
        .POST_CTL_DELAY_CYCLES(1000),
        .TIMEOUT_CYCLES(4000),
        .POLL_GAP_CYCLES(100),
        .MAX_RETRIES(8),
        .NEW_ADDR(7'd1),
        .MIDI_BULK_IN_EP(4'd1)
    ) dut (
        .clk           (clk),
        .resetn        (resetn),
        .usb_dp        (usb_dp),
        .usb_dm        (usb_dm),
        .pkt_valid     (),
        .pkt_cin_cable (),
        .pkt_b0        (),
        .pkt_b1        (),
        .pkt_b2        (),
        .dev_configured(dev_configured),
        .dev_error     (dev_error),
        .poll_count    (poll_count),
        .nak_count     (nak_count)
    );
    /* verilator lint_on PINCONNECTEMPTY */

    reg        dev_tx_start;
    reg [7:0]  dev_tx_pid;
    reg [3:0]  dev_tx_len;
    /* verilator lint_off UNUSEDSIGNAL */
    wire       dev_tx_done, dev_tx_busy;
    wire       dev_rx_active, dev_rx_done, dev_rx_ok;
    wire [7:0] dev_rx_pid;
    wire       dev_rx_pid_valid;
    /* verilator lint_on UNUSEDSIGNAL */

    wire dev_rx_enable = ~dev_tx_busy;

    /* verilator lint_off PINCONNECTEMPTY */
    usb_midi_sie device_sie (
        .clk          (clk),
        .rst          (~resetn),
        .usb_dp       (usb_dp),
        .usb_dm       (usb_dm),
        .sym_j        (),
        .sym_k        (),
        .sym_se0      (),
        .force_se0    (1'b0),
        .tx_start     (dev_tx_start),
        .tx_pid       (dev_tx_pid),
        .tx_is_token  (1'b0),
        .tx_addr      (7'd0),
        .tx_endp      (4'd0),
        .tx_payload   (64'd0),
        .tx_len       (dev_tx_len),
        .tx_done      (dev_tx_done),
        .tx_busy      (dev_tx_busy),
        .rx_enable    (dev_rx_enable),
        .rx_active    (dev_rx_active),
        .rx_pid       (dev_rx_pid),
        .rx_pid_valid (dev_rx_pid_valid),
        .rx_byte_valid(),
        .rx_byte      (),
        .rx_done      (dev_rx_done),
        .rx_ok        (dev_rx_ok)
    );
    /* verilator lint_on PINCONNECTEMPTY */

    reg [3:0] pkt_count;
    assign pkt_count_dbg = pkt_count;

    always @(posedge clk) begin
        dev_tx_start <= 1'b0;
        if (~resetn) begin
            pkt_count <= 4'd0;
        end else if (dev_rx_done) begin
            case (pkt_count)
                4'd1: begin
                    dev_tx_start <= 1'b1;
                    dev_tx_pid   <= `USB_PID_ACK;
                    dev_tx_len   <= 4'd0;
                end
                4'd2: begin
                    dev_tx_start <= 1'b1;
                    dev_tx_pid   <= `USB_PID_DATA1;
                    dev_tx_len   <= 4'd0;
                end
                4'd5: begin
                    dev_tx_start <= 1'b1;
                    dev_tx_pid   <= `USB_PID_ACK;
                    dev_tx_len   <= 4'd0;
                end
                4'd6: begin
                    dev_tx_start <= 1'b1;
                    dev_tx_pid   <= `USB_PID_DATA1;
                    dev_tx_len   <= 4'd0;
                end
                4'd8: begin
                    dev_tx_start <= 1'b1;
                    dev_tx_pid   <= `USB_PID_NAK;
                    dev_tx_len   <= 4'd0;
                end
                default: ;
            endcase
            if (pkt_count != 4'hF) pkt_count <= pkt_count + 4'd1;
        end
    end

endmodule
