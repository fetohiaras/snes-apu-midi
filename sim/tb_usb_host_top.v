`timescale 1ns/1ps

// Test wrapper for the USB host simulation: usb_host_probe + usb_fs_phy with
// the transceiver pins exposed, so the C++ device model resolves the bus,
// plus the configuration descriptor RAM with a read port for the checks.
module tb_usb_host_top (
    input  wire        clk,
    input  wire        rst,
    input  wire        bus_dp,
    input  wire        bus_dn,
    output wire        tx_dp,
    output wire        tx_dn,
    output wire        tx_oen,
    output wire [3:0]  state,
    output wire [1:0]  req,
    output wire [6:0]  dev_addr,
    output wire [7:0]  config_value,
    output wire [10:0] frame,
    output wire [7:0]  attach_count,
    output wire [7:0]  last_pid,
    output wire [7:0]  nak_count,
    output wire [7:0]  timeout_count,
    output wire [7:0]  crc_count,
    output wire [4:0]  desc_len,
    output wire [8*18-1:0] desc,
    output wire [8:0]  cfg_len,
    input  wire [7:0]  cfg_raddr,
    output wire [7:0]  cfg_rdata,
    output wire [7:0]  midi_ep,
    output wire [6:0]  midi_ep_size,
    output wire [15:0] midi_bytes,
    output wire [15:0] bulk_packets,
    output wire        midi_valid,
    output wire [7:0]  midi_data,
    output wire        midi_overflow
);
    wire [7:0] tx_data, rx_data;
    wire       tx_valid, txready, rxvalid, rxactive;
    wire [1:0] linestate;
    wire       usb_reset;
    wire       cfg_we;
    wire [7:0] cfg_waddr, cfg_wdata;
    reg  [7:0] cfg_mem [0:255];
    wire       m_we, m_commit, m_rollback;
    wire [7:0] m_wdata;

    usb_midi_fifo fifo (
        .wclk(clk), .wrst(rst), .we(m_we), .wdata(m_wdata),
        .commit(m_commit), .rollback(m_rollback), .overflow(midi_overflow),
        .rclk(clk), .rresetn(~rst), .ready(1'b1), .valid(midi_valid), .data(midi_data)
    );

    always @(posedge clk)
        if (cfg_we) cfg_mem[cfg_waddr] <= cfg_wdata;
    assign cfg_rdata = cfg_mem[cfg_raddr];

    usb_fs_phy phy (
        .clk_i(clk), .rst_i(rst),
        .utmi_data_out_i(tx_data), .utmi_txvalid_i(tx_valid),
        .utmi_op_mode_i(2'b00), .utmi_xcvrselect_i(2'b01), .utmi_termselect_i(1'b0),
        .utmi_dppulldown_i(1'b1), .utmi_dmpulldown_i(1'b1),
        .usb_rx_rcv_i(bus_dp & ~bus_dn), .usb_rx_dp_i(bus_dp), .usb_rx_dn_i(bus_dn),
        .usb_reset_assert_i(usb_reset),
        .utmi_data_in_o(rx_data), .utmi_txready_o(txready), .utmi_rxvalid_o(rxvalid),
        .utmi_rxactive_o(rxactive), .utmi_rxerror_o(), .utmi_linestate_o(linestate),
        .usb_tx_dp_o(tx_dp), .usb_tx_dn_o(tx_dn), .usb_tx_oen_o(tx_oen),
        .usb_reset_detect_o(), .usb_en_o()
    );

    usb_host_probe #(.USB_CLK_HZ(48000000)) probe (
        .clk(clk), .rst(rst),
        .utmi_data_o(tx_data), .utmi_txvalid_o(tx_valid), .utmi_txready_i(txready),
        .utmi_data_i(rx_data), .utmi_rxvalid_i(rxvalid), .utmi_rxactive_i(rxactive),
        .utmi_linestate_i(linestate), .usb_reset_o(usb_reset),
        .cfg_we(cfg_we), .cfg_waddr(cfg_waddr), .cfg_wdata(cfg_wdata),
        .midi_we(m_we), .midi_wdata(m_wdata), .midi_commit(m_commit), .midi_rollback(m_rollback),
        .state(state), .req(req), .dev_addr(dev_addr), .config_value(config_value),
        .frame(frame), .attach_count(attach_count), .last_pid(last_pid),
        .nak_count(nak_count), .timeout_count(timeout_count), .crc_count(crc_count),
        .desc_len(desc_len), .desc(desc), .cfg_len(cfg_len),
        .midi_ep(midi_ep), .midi_ep_size(midi_ep_size), .midi_bytes(midi_bytes),
        .bulk_packets(bulk_packets)
    );
endmodule
