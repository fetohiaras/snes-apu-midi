`timescale 1ns/1ps

// Full-speed USB host bring-up probe (no CPU).
//
// Drives usbh_sie (ultraembedded core_usb_host) over a UTMI PHY and runs
// enumeration on whatever is plugged into the host port:
//
//   DETACHED  wait for J (device pull-up on D+) for T_ATTACH
//   RESET     bus reset (SE0) for 20 ms
//   RECOVER   SOFs for 20 ms (reset recovery)
//   then four control transfers on endpoint 0, each SETUP -> data -> status:
//     REQ_DEV   GET_DESCRIPTOR(DEVICE, 18) at address 0   -> desc
//     REQ_ADDR  SET_ADDRESS(1), then WAIT 5 ms (set-address recovery)
//     REQ_CFG   GET_DESCRIPTOR(CONFIGURATION, 256) at 1   -> cfg_w* port
//     SCAN      walk the stored configuration descriptor for the first
//               bulk (or interrupt, used by some cheap keyboards; polled
//               the same way) IN endpoint of a MIDI Streaming interface (01/03)
//     REQ_SETCF SET_CONFIGURATION(bConfigurationValue of that descriptor)
//   POLL      (MIDI endpoint found) one bulk IN per frame, more at once
//             while full packets keep coming; each 4-byte USB-MIDI event
//             is unpacked to 1-3 MIDI bytes on the midi_* port, committed
//             when the packet's CRC and data toggle check out
//   DONE      (no MIDI endpoint) keep sending SOFs, hold the results
//
// Control reads: IN DATA1, DATA0, ... until wLength bytes or a short packet
// (shorter than the EP0 size from the device descriptor), then an OUT DATA1
// zero-length status. Control writes: an IN status stage expecting a
// zero-length DATA1.
//
// NAKs are retried in the next frame; a STALL restarts the current request;
// after MAX_ERRORS timeouts/CRC errors/STALLs the probe resets the bus and
// starts over. SE0 for more than 2.5 us while the bus is idle means the
// device was unplugged.
//
// The status outputs change at most once per transfer and are stable in
// DONE, so a slow clock domain can snapshot them with a handshake and read
// the configuration descriptor from a dual-clock RAM (see the top level).
module usb_host_probe #(
    parameter integer USB_CLK_HZ = 48000000,
    parameter integer ATTACH_MS  = 100,
    parameter integer RESET_MS   = 20,
    parameter integer RECOVER_MS = 20,
    parameter integer ADDR_MS    = 5
) (
    input  wire         clk,
    input  wire         rst,

    // UTMI (to usb_fs_phy)
    output wire [7:0]   utmi_data_o,
    output wire         utmi_txvalid_o,
    input  wire         utmi_txready_i,
    input  wire [7:0]   utmi_data_i,
    input  wire         utmi_rxvalid_i,
    input  wire         utmi_rxactive_i,
    input  wire [1:0]   utmi_linestate_i,
    output wire         usb_reset_o,

    // Configuration descriptor write port (to a 256-byte RAM)
    output reg          cfg_we,
    output reg  [7:0]   cfg_waddr,
    output reg  [7:0]   cfg_wdata,

    // MIDI byte stream (to usb_midi_fifo)
    output reg          midi_we,
    output reg  [7:0]   midi_wdata,
    output reg          midi_commit,
    output reg          midi_rollback,

    // Status
    output reg  [3:0]   state,
    output reg  [1:0]   req,            // control request in progress (REQ_*)
    output reg  [6:0]   dev_addr,
    output reg  [7:0]   config_value,   // set by SET_CONFIGURATION (0 = not yet)
    output reg  [10:0]  frame,
    output reg  [7:0]   attach_count,
    output reg  [7:0]   last_pid,
    output reg  [7:0]   nak_count,
    output reg  [7:0]   timeout_count,
    output reg  [7:0]   crc_count,
    output reg  [4:0]   desc_len,
    output wire [8*18-1:0] desc,
    output reg  [8:0]   cfg_len,        // configuration bytes received (<= 256)
    output reg  [7:0]   midi_ep,        // bEndpointAddress of the MIDI IN endpoint (0 = none)
    output reg  [6:0]   midi_ep_size,
    output reg  [15:0]  midi_bytes,     // MIDI bytes committed
    output reg  [15:0]  bulk_packets    // bulk IN data packets accepted
);
    localparam integer CLKS_MS   = USB_CLK_HZ / 1000;
    localparam integer CLKS_BIT  = (USB_CLK_HZ + 6000000) / 12000000;   // 4
    localparam integer SOF_LAST  = CLKS_MS - 1;
    // No transfer may start right after a SOF or so late that it runs into
    // the next one (64-byte packet + margin), as in usbh_host.
    localparam integer GUARD_LOW  = 20 * CLKS_BIT;
    localparam integer GUARD_HIGH = SOF_LAST - 50 * CLKS_BIT - (64 + 6) * 10 * CLKS_BIT;
    localparam integer T_SE0      = USB_CLK_HZ / 400000;    // 2.5 us
    localparam [7:0]  MAX_ERRORS = 8'd32;
    localparam [22:0] T_ATTACH   = 23'(ATTACH_MS * CLKS_MS);
    localparam [22:0] T_RESET    = 23'(RESET_MS * CLKS_MS);
    localparam [22:0] T_RECOVER  = 23'(RECOVER_MS * CLKS_MS);
    localparam [22:0] T_ADDR     = 23'(ADDR_MS * CLKS_MS);

    localparam [3:0] S_DETACHED = 4'd0, S_RESET = 4'd1, S_RECOVER = 4'd2,
                     S_SETUP = 4'd3, S_IN = 4'd4, S_STATUS_OUT = 4'd5, S_DONE = 4'd6,
                     S_STATUS_IN = 4'd7, S_WAIT = 4'd8, S_SCAN = 4'd9, S_POLL = 4'd10;

    localparam [1:0] REQ_DEV = 2'd0, REQ_ADDR = 2'd1, REQ_CFG = 2'd2, REQ_SETCF = 2'd3;

    localparam [7:0] PID_OUT = 8'hE1, PID_IN = 8'h69, PID_SETUP = 8'h2D,
                     PID_DATA0 = 8'hC3, PID_DATA1 = 8'h4B,
                     PID_ACK = 8'hD2, PID_NAK = 8'h5A;

    localparam [6:0] NEW_ADDR = 7'd1;

    // SETUP packets: {bmRequestType, bRequest, wValue, wIndex, wLength}
    reg [7:0] setup_byte;
    reg [2:0] tx_idx;
    always @* begin
        setup_byte = 8'h00;
        case (req)
        REQ_DEV:   case (tx_idx) 3'd0: setup_byte = 8'h80; 3'd1: setup_byte = 8'h06;
                                 3'd3: setup_byte = 8'h01; 3'd6: setup_byte = 8'd18;
                                 default: ; endcase
        REQ_ADDR:  case (tx_idx) 3'd1: setup_byte = 8'h05; 3'd2: setup_byte = {1'b0, NEW_ADDR};
                                 default: ; endcase
        REQ_CFG:   case (tx_idx) 3'd0: setup_byte = 8'h80; 3'd1: setup_byte = 8'h06;
                                 3'd3: setup_byte = 8'h02; 3'd7: setup_byte = 8'h01;   // wLength 256
                                 default: ; endcase
        default:   case (tx_idx) 3'd1: setup_byte = 8'h09; 3'd2: setup_byte = cfg_value_q;
                                 default: ; endcase
        endcase
    end

    // ------------------------------------------------------------------
    // SIE
    // ------------------------------------------------------------------
    reg         start_q, sof_xfer_q, in_xfer_q, resp_q;
    reg  [7:0]  pid_q;
    reg  [15:0] len_q;
    reg         data1_q;
    reg  [7:0]  cfg_value_q;    // bConfigurationValue (config byte 5)
    wire        sie_ack, sie_idle, sie_tx_pop, sie_rx_push, sie_crc_err, sie_timeout;
    wire [7:0]  sie_rx_data, sie_response;

    // usbh_sie wants the token fields bit-reversed (see usbh_host.v); for a
    // SOF the 11-bit frame number takes the address and endpoint fields.
    reg  [3:0]  ep_q;           // endpoint of the current transfer
    wire [10:0] tok = sof_xfer_q ? frame : {ep_q, dev_addr};
    wire [6:0]  tok_dev = {tok[0], tok[1], tok[2], tok[3], tok[4], tok[5], tok[6]};
    wire [3:0]  tok_ep  = {tok[7], tok[8], tok[9], tok[10]};

    usbh_sie #(.USB_CLK_FREQ(USB_CLK_HZ)) sie (
        .clk_i(clk), .rst_i(rst),
        .start_i(start_q), .in_transfer_i(in_xfer_q), .sof_transfer_i(sof_xfer_q),
        .resp_expected_i(resp_q),
        .token_pid_i(sof_xfer_q ? 8'hA5 : pid_q), .token_dev_i(tok_dev), .token_ep_i(tok_ep),
        .data_len_i(len_q), .data_idx_i(data1_q),
        .tx_data_i(setup_byte),
        .utmi_txready_i(utmi_txready_i), .utmi_data_i(utmi_data_i),
        .utmi_rxvalid_i(utmi_rxvalid_i), .utmi_rxactive_i(utmi_rxactive_i),
        .utmi_linestate_i(utmi_linestate_i),
        .ack_o(sie_ack), .tx_pop_o(sie_tx_pop), .rx_data_o(sie_rx_data), .rx_push_o(sie_rx_push),
        .tx_done_o(), .rx_done_o(), .crc_err_o(sie_crc_err), .timeout_o(sie_timeout),
        .response_o(sie_response), .rx_count_o(), .idle_o(sie_idle),
        .utmi_data_o(utmi_data_o), .utmi_txvalid_o(utmi_txvalid_o)
    );

    // ------------------------------------------------------------------
    // Frame timer and transfer arbiter
    // ------------------------------------------------------------------
    reg  [15:0] sof_time;
    reg         sof_en;
    reg         xreq;           // controller wants a transfer
    reg         xreq_in;        // ... and it is an IN
    reg         busy;           // controller transfer in flight
    reg         wait_frame;     // retry only after the next SOF
    wire        send_sof   = sof_en && sof_time == SOF_LAST[15:0] && sie_idle;
    wire        clear_send = sie_idle && (!sof_en ||
                             (sof_time > GUARD_LOW[15:0] && sof_time < GUARD_HIGH[15:0]));
    wire        xfer_done  = busy && sie_idle && !start_q;

    // Data stage bookkeeping
    reg  [7:0]  desc_mem [0:17];
    reg  [6:0]  pkt_len;        // bytes of the packet being received (<= 64)
    reg  [8:0]  got;            // bytes of the current request's data stage
    reg         expect_data1;
    reg  [22:0] timer;
    reg  [7:0]  se0_cnt;
    reg  [7:0]  errors;

    wire [6:0]  ep0_size  = desc_mem[7][6:0] == 7'd0 ? 7'd8 : desc_mem[7][6:0];
    wire [8:0]  want      = req == REQ_DEV ? 9'd18 : 9'd256;

    // Local copy of the configuration descriptor for the endpoint scan
    reg  [7:0]  scan_mem [0:255];
    reg  [7:0]  scan_q;
    reg  [8:0]  sidx;
    reg         sphase;         // 0: read issued, 1: scan_q valid
    reg  [7:0]  s_off, s_len, s_type, s_class, s_ep, s_attr;
    reg         s_in_ms;

    // Bulk IN: data toggle, USB-MIDI unpacking
    reg         bulk_data1;
    reg  [1:0]  ev_need;        // MIDI bytes left in the current 4-byte event
    reg  [6:0]  pkt_midi;       // MIDI bytes of the packet in flight

    // USB-MIDI Code Index Number -> MIDI bytes in the event
    function [1:0] cin_bytes(input [3:0] cin);
        case (cin)
            4'h2, 4'h6, 4'hC, 4'hD: cin_bytes = 2'd2;
            4'h3, 4'h4, 4'h7, 4'h8, 4'h9, 4'hA, 4'hB, 4'hE: cin_bytes = 2'd3;
            4'h5, 4'hF: cin_bytes = 2'd1;
            default: cin_bytes = 2'd0;          // 0, 1: reserved
        endcase
    endfunction
    wire [9:0]  got_next  = {1'b0, got} + {3'd0, pkt_len};

    genvar gi;
    generate
        for (gi = 0; gi < 18; gi = gi + 1) begin : g_desc
            assign desc[8*gi +: 8] = desc_mem[gi];
        end
    endgenerate

    assign usb_reset_o = state == S_RESET;

    // Start (or restart) control request r.
    task automatic begin_req(input [1:0] r);
        begin
            req          <= r;
            state        <= S_SETUP;
            got          <= 9'd0;
            expect_data1 <= 1'b1;
        end
    endtask

    always @(posedge clk) begin
        if (busy && sie_rx_push && state == S_IN && req == REQ_CFG && got_next < 10'd256)
            scan_mem[got_next[7:0]] <= sie_rx_data;
        scan_q <= scan_mem[sidx[7:0]];
    end

    integer k;
    always @(posedge clk) begin
        cfg_we        <= 1'b0;
        midi_we       <= 1'b0;
        midi_commit   <= 1'b0;
        midi_rollback <= 1'b0;
        if (rst) begin
            start_q       <= 1'b0;
            sof_xfer_q    <= 1'b0;
            in_xfer_q     <= 1'b0;
            resp_q        <= 1'b0;
            pid_q         <= 8'h00;
            len_q         <= 16'd0;
            data1_q       <= 1'b0;
            tx_idx        <= 3'd0;
            sof_time      <= 16'd0;
            sof_en        <= 1'b0;
            xreq          <= 1'b0;
            xreq_in       <= 1'b0;
            busy          <= 1'b0;
            wait_frame    <= 1'b0;
            state         <= S_DETACHED;
            req           <= REQ_DEV;
            dev_addr      <= 7'd0;
            config_value  <= 8'd0;
            cfg_value_q   <= 8'd0;
            frame         <= 11'd0;
            attach_count  <= 8'd0;
            last_pid      <= 8'h00;
            nak_count     <= 8'd0;
            timeout_count <= 8'd0;
            crc_count     <= 8'd0;
            desc_len      <= 5'd0;
            cfg_len       <= 9'd0;
            cfg_waddr     <= 8'd0;
            cfg_wdata     <= 8'd0;
            pkt_len       <= 7'd0;
            got           <= 9'd0;
            expect_data1  <= 1'b1;
            timer         <= 23'd0;
            se0_cnt       <= 8'd0;
            errors        <= 8'd0;
            ep_q          <= 4'd0;
            midi_ep       <= 8'd0;
            midi_ep_size  <= 7'd0;
            midi_bytes    <= 16'd0;
            bulk_packets  <= 16'd0;
            midi_wdata    <= 8'd0;
            bulk_data1    <= 1'b0;
            ev_need       <= 2'd0;
            pkt_midi      <= 7'd0;
            sidx          <= 9'd0;
            sphase        <= 1'b0;
            s_off         <= 8'd0;
            s_len         <= 8'd0;
            s_type        <= 8'd0;
            s_class       <= 8'd0;
            s_ep          <= 8'd0;
            s_attr        <= 8'd0;
            s_in_ms       <= 1'b0;
            for (k = 0; k < 18; k = k + 1)
                desc_mem[k] <= 8'h00;
        end else begin
            // ---------------- frame timer / SOF ----------------
            if (send_sof && !start_q) begin
                sof_time   <= 16'd0;
                frame      <= frame + 11'd1;
                wait_frame <= 1'b0;
            end else if (sof_time != SOF_LAST[15:0]) begin
                sof_time <= sof_time + 16'd1;
            end

            // ---------------- arbiter ----------------
            if (start_q) begin
                if (sie_ack)
                    start_q <= 1'b0;
            end else if (send_sof) begin
                start_q    <= 1'b1;
                sof_xfer_q <= 1'b1;
                in_xfer_q  <= 1'b0;
                resp_q     <= 1'b0;
            end else if (clear_send && xreq && !busy) begin
                start_q    <= 1'b1;
                sof_xfer_q <= 1'b0;
                in_xfer_q  <= xreq_in;
                resp_q     <= 1'b1;
                busy       <= 1'b1;
                xreq       <= 1'b0;
                tx_idx     <= 3'd0;
                pkt_len    <= 7'd0;
                ev_need    <= 2'd0;
                pkt_midi   <= 7'd0;
            end

            if (sie_tx_pop)
                tx_idx <= tx_idx + 3'd1;

            // Received data bytes go straight to their destination; a packet
            // that later fails its CRC is simply overwritten by the retry.
            if (busy && sie_rx_push && state == S_IN) begin
                if (pkt_len != 7'd127)
                    pkt_len <= pkt_len + 7'd1;
                if (req == REQ_DEV && got_next < 10'd18)
                    desc_mem[got_next[4:0]] <= sie_rx_data;
                if (req == REQ_CFG && got_next < 10'd256) begin
                    cfg_we    <= 1'b1;
                    cfg_waddr <= got_next[7:0];
                    cfg_wdata <= sie_rx_data;
                    if (got_next == 10'd5)
                        cfg_value_q <= sie_rx_data;
                end
            end

            // USB-MIDI events: header byte (cable, CIN), then 3 bytes of
            // which the first cin_bytes(CIN) are MIDI.
            if (busy && sie_rx_push && state == S_POLL) begin
                if (pkt_len != 7'd127)
                    pkt_len <= pkt_len + 7'd1;
                if (pkt_len[1:0] == 2'd0) begin
                    ev_need <= cin_bytes(sie_rx_data[3:0]);
                end else if (ev_need != 2'd0) begin
                    ev_need    <= ev_need - 2'd1;
                    midi_we    <= 1'b1;
                    midi_wdata <= sie_rx_data;
                    pkt_midi   <= pkt_midi + 7'd1;
                end
            end

            // ---------------- detach detection ----------------
            se0_cnt <= utmi_linestate_i != 2'b00 || !sie_idle || state == S_RESET ? 8'd0 :
                       se0_cnt == 8'hFF ? se0_cnt : se0_cnt + 8'd1;

            // ---------------- controller ----------------
            if (xfer_done) begin
                busy     <= 1'b0;
                last_pid <= sie_timeout ? 8'h00 : sie_response;
            end

            case (state)
            S_DETACHED: begin
                sof_en   <= 1'b0;
                dev_addr <= 7'd0;
                timer    <= utmi_linestate_i == 2'b01 ? timer + 23'd1 : 23'd0;
                if (timer == T_ATTACH) begin
                    state        <= S_RESET;
                    timer        <= 23'd0;
                    attach_count <= attach_count + 8'd1;
                end
            end
            S_RESET: begin
                timer <= timer + 23'd1;
                if (timer == T_RESET) begin
                    state    <= S_RECOVER;
                    timer    <= 23'd0;
                    sof_en   <= 1'b1;
                    sof_time <= 16'd0;
                end
            end
            S_RECOVER: begin
                timer <= timer + 23'd1;
                if (timer == T_RECOVER) begin
                    desc_len     <= 5'd0;
                    cfg_len      <= 9'd0;
                    config_value <= 8'd0;
                    errors       <= 8'd0;
                    begin_req(REQ_DEV);
                end
            end
            S_WAIT: begin
                timer <= timer + 23'd1;
                if (timer == T_ADDR)
                    begin_req(REQ_CFG);
            end
            S_SCAN: begin
                // Two clocks per byte: address, then data.
                sphase <= ~sphase;
                if (sphase) begin
                    s_off <= s_off == 8'd0 ? 8'd1 : s_off + 8'd1 == s_len ? 8'd0 : s_off + 8'd1;
                    case (s_off)
                        8'd0: s_len  <= scan_q;
                        8'd1: s_type <= scan_q;
                        default: ;
                    endcase
                    if (s_type == 8'd4 && s_off == 8'd5) s_class <= scan_q;
                    if (s_type == 8'd4 && s_off == 8'd6) s_in_ms <= s_class == 8'd1 && scan_q == 8'd3;
                    if (s_type == 8'd5 && s_in_ms && midi_ep == 8'd0) begin
                        if (s_off == 8'd2) s_ep   <= scan_q;
                        if (s_off == 8'd3) s_attr <= scan_q;
                        if (s_off == 8'd4 && s_ep[7] && s_attr[1]) begin      // 2 bulk, 3 interrupt
                            midi_ep      <= s_ep;
                            midi_ep_size <= scan_q > 8'd64 || scan_q == 8'd0 ? 7'd64 : scan_q[6:0];
                        end
                    end
                    sidx <= sidx + 9'd1;
                    if (sidx + 9'd1 >= cfg_len || (s_off == 8'd0 && scan_q < 8'd2))
                        begin_req(REQ_SETCF);       // end of descriptor (or malformed)
                end
            end
            S_SETUP, S_IN, S_STATUS_OUT, S_STATUS_IN, S_POLL: begin
                if (!busy && !xreq && !wait_frame) begin
                    ep_q    <= state == S_POLL ? midi_ep[3:0] : 4'd0;
                    xreq    <= 1'b1;
                    pid_q   <= state == S_SETUP ? PID_SETUP :
                               state == S_STATUS_OUT ? PID_OUT : PID_IN;
                    xreq_in <= state == S_IN || state == S_STATUS_IN || state == S_POLL;
                    len_q   <= state == S_SETUP ? 16'd8 : 16'd0;
                    data1_q <= state != S_SETUP;
                end
                if (xfer_done && state == S_POLL) begin
                    if (!sie_timeout && !sie_crc_err && sie_response == PID_NAK) begin
                        wait_frame <= 1'b1;                     // no data this frame
                    end else if (!sie_timeout && !sie_crc_err &&
                                 (sie_response == PID_DATA0 || sie_response == PID_DATA1)) begin
                        errors <= 8'd0;
                        if ((sie_response == PID_DATA1) == bulk_data1) begin
                            bulk_data1   <= ~bulk_data1;
                            midi_commit  <= 1'b1;
                            midi_bytes   <= midi_bytes + {9'd0, pkt_midi};
                            bulk_packets <= bulk_packets + 16'd1;
                        end else begin
                            midi_rollback <= 1'b1;              // repeat of a packet we have
                        end
                        // A full packet may be followed by more: ask again now.
                        wait_frame <= pkt_len < midi_ep_size;
                    end else begin
                        midi_rollback <= 1'b1;
                        if (sie_timeout)      timeout_count <= timeout_count + {7'd0, timeout_count != 8'hFF};
                        else if (sie_crc_err) crc_count     <= crc_count + {7'd0, crc_count != 8'hFF};
                        errors     <= errors + 8'd1;
                        wait_frame <= 1'b1;
                        if (errors == MAX_ERRORS - 8'd1) begin
                            state <= S_DETACHED;
                            timer <= 23'd0;
                        end
                    end
                end else if (xfer_done) begin
                    if (sie_timeout || sie_crc_err) begin
                        if (sie_timeout) timeout_count <= timeout_count + {7'd0, timeout_count != 8'hFF};
                        else             crc_count     <= crc_count + {7'd0, crc_count != 8'hFF};
                        errors     <= errors + 8'd1;
                        wait_frame <= 1'b1;
                        if (errors == MAX_ERRORS - 8'd1) begin
                            state <= S_DETACHED;
                            timer <= 23'd0;
                        end
                    end else if (sie_response == PID_NAK) begin
                        nak_count  <= nak_count + {7'd0, nak_count != 8'hFF};
                        wait_frame <= 1'b1;
                    end else if (state == S_IN &&
                                 (sie_response == PID_DATA0 || sie_response == PID_DATA1)) begin
                        if ((sie_response == PID_DATA1) == expect_data1) begin
                            expect_data1 <= ~expect_data1;
                            got <= got_next >= {1'b0, want} ? want : got_next[8:0];
                            if (req == REQ_DEV)
                                desc_len <= got_next >= 10'd18 ? 5'd18 : got_next[4:0];
                            else
                                cfg_len  <= got_next >= 10'd256 ? 9'd256 : got_next[8:0];
                            // The device descriptor's EP0 size is only known
                            // after its first packet; 8 is the minimum.
                            if (got_next >= {1'b0, want} ||
                                pkt_len < (req == REQ_DEV ? 7'd8 : ep0_size))
                                state <= S_STATUS_OUT;
                        end
                        // Wrong toggle: a retransmission we already ACKed; ignore it.
                    end else if (state == S_STATUS_IN &&
                                 (sie_response == PID_DATA0 || sie_response == PID_DATA1)) begin
                        // Zero-length status from the device: the request is done.
                        if (req == REQ_ADDR) begin
                            dev_addr <= NEW_ADDR;
                            state    <= S_WAIT;
                            timer    <= 23'd0;
                        end else begin
                            config_value <= cfg_value_q;
                            bulk_data1   <= 1'b0;   // bulk toggles start at DATA0
                            state        <= midi_ep != 8'd0 ? S_POLL : S_DONE;
                        end
                    end else if (state != S_IN && state != S_STATUS_IN && sie_response == PID_ACK) begin
                        if (state == S_SETUP)
                            state <= req == REQ_ADDR || req == REQ_SETCF ? S_STATUS_IN : S_IN;
                        else if (req == REQ_DEV)
                            begin_req(REQ_ADDR);
                        else begin
                            state   <= S_SCAN;
                            sidx    <= 9'd0;
                            sphase  <= 1'b0;
                            s_off   <= 8'd0;
                            s_len   <= 8'd0;
                            s_type  <= 8'd0;
                            s_in_ms <= 1'b0;
                            midi_ep <= 8'd0;
                        end
                    end else begin
                        // STALL or anything unexpected: restart this request.
                        errors     <= errors + 8'd1;
                        wait_frame <= 1'b1;
                        begin_req(req);
                        if (errors == MAX_ERRORS - 8'd1) begin
                            state <= S_DETACHED;
                            timer <= 23'd0;
                        end
                    end
                end
            end
            default: ;  // S_DONE: SOFs only
            endcase

            if (state >= S_RECOVER && se0_cnt > T_SE0[7:0]) begin
                state  <= S_DETACHED;
                timer  <= 23'd0;
                sof_en <= 1'b0;
                xreq   <= 1'b0;
            end
        end
    end
endmodule
