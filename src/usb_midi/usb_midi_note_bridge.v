`timescale 1ns/1ps

module usb_midi_note_bridge (
    input  wire       clk_usb,
    input  wire       resetn_usb,
    input  wire        pkt_valid,
    input  wire [7:0]  pkt_cin_cable,
    input  wire [7:0]  pkt_b0,
    input  wire [7:0]  pkt_b1,
    input  wire [7:0]  pkt_b2,

    input  wire        clk,
    input  wire        resetn,

    output reg         ev_valid,
    output reg  [7:0]  ev_status,
    output reg  [6:0]  ev_data1,
    output reg  [6:0]  ev_data2
);

    /* verilator lint_off UNUSEDSIGNAL */
    reg [7:0] cin_latched, b0_latched, b1_latched, b2_latched;
    /* verilator lint_on UNUSEDSIGNAL */
    reg       pkt_toggle;

    always @(posedge clk_usb) begin
        if (!resetn_usb) begin
            cin_latched <= 8'h00;
            b0_latched  <= 8'h00;
            b1_latched  <= 8'h00;
            b2_latched  <= 8'h00;
            pkt_toggle  <= 1'b0;
        end else begin
            if (pkt_valid) begin
                cin_latched <= pkt_cin_cable;
                b0_latched  <= pkt_b0;
                b1_latched  <= pkt_b1;
                b2_latched  <= pkt_b2;
            end
            pkt_toggle <= pkt_toggle ^ pkt_valid;
        end
    end

    reg [2:0] toggle_sync;
    wire      toggle_edge = toggle_sync[2] ^ toggle_sync[1];

    always @(posedge clk) begin
        ev_valid <= 1'b0;

        if (!resetn) begin
            toggle_sync <= 3'b000;
            ev_status   <= 8'h00;
            ev_data1    <= 7'd0;
            ev_data2    <= 7'd0;
        end else begin
            toggle_sync <= {toggle_sync[1:0], pkt_toggle};

            if (toggle_edge) begin
                case (cin_latched[3:0])
                    4'h2: begin
                        ev_valid  <= 1'b1;
                        ev_status <= b0_latched;
                        ev_data1  <= b1_latched[6:0];
                        ev_data2  <= 7'd0;
                    end
                    4'h3, 4'h8, 4'h9, 4'hA, 4'hB, 4'hE: begin
                        ev_valid  <= 1'b1;
                        ev_status <= b0_latched;
                        ev_data1  <= b1_latched[6:0];
                        ev_data2  <= b2_latched[6:0];
                    end
                    4'hC, 4'hD: begin
                        ev_valid  <= 1'b1;
                        ev_status <= b0_latched;
                        ev_data1  <= b1_latched[6:0];
                        ev_data2  <= 7'd0;
                    end
                    4'h5, 4'hF: begin
                        ev_valid  <= 1'b1;
                        ev_status <= b0_latched;
                        ev_data1  <= 7'd0;
                        ev_data2  <= 7'd0;
                    end
                    default: begin
                    end
                endcase
            end
        end
    end

endmodule
