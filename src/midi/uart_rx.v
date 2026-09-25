`timescale 1ns/1ps

// 8N1 UART receiver. Default baud is DIN-MIDI's 31250; any rate works as long
// as CLK_HZ/BAUD is large (it is ~785 at the APU clock).
module uart_rx #(
    parameter integer CLK_HZ = 24528000,
    parameter integer BAUD   = 31250
) (
    input  wire       clk,
    input  wire       resetn,
    input  wire       rx,
    output reg  [7:0] data,
    output reg        valid,
    output reg        frame_err
);

    localparam integer CPB     = (CLK_HZ + BAUD / 2) / BAUD;
    localparam integer W       = $clog2(CPB);
    localparam integer HALF_I  = CPB / 2 - 1;
    localparam integer FULL_I  = CPB - 1;
    localparam [W-1:0] HALF    = HALF_I[W-1:0];
    localparam [W-1:0] FULL    = FULL_I[W-1:0];

    localparam [1:0] S_IDLE = 2'd0, S_START = 2'd1, S_DATA = 2'd2, S_STOP = 2'd3;

    reg [2:0]   sync = 3'b111;
    wire        rx_s    = sync[1];
    wire        rx_fall = sync[2] & ~sync[1];

    reg [1:0]   state;
    reg [W-1:0] cnt;
    reg [2:0]   bit_idx;
    reg [7:0]   shift;

    always @(posedge clk) begin
        sync <= {sync[1:0], rx};

        valid     <= 1'b0;
        frame_err <= 1'b0;

        if (!resetn) begin
            state   <= S_IDLE;
            cnt     <= {W{1'b0}};
            bit_idx <= 3'd0;
            shift   <= 8'h00;
            data    <= 8'h00;
        end else begin
            if (cnt != {W{1'b0}})
                cnt <= cnt - 1'b1;

            case (state)
                S_IDLE:
                    // A falling edge, not a low level, starts a frame, so a
                    // held-low line (break/unplugged) is not read as 0x00s.
                    if (rx_fall) begin
                        cnt   <= HALF;
                        state <= S_START;
                    end

                S_START:
                    if (cnt == {W{1'b0}}) begin
                        if (!rx_s) begin
                            cnt     <= FULL;
                            bit_idx <= 3'd0;
                            state   <= S_DATA;
                        end else begin
                            state <= S_IDLE;
                        end
                    end

                S_DATA:
                    if (cnt == {W{1'b0}}) begin
                        shift <= {rx_s, shift[7:1]};
                        cnt   <= FULL;
                        if (bit_idx == 3'd7)
                            state <= S_STOP;
                        bit_idx <= bit_idx + 3'd1;
                    end

                default:
                    if (cnt == {W{1'b0}}) begin
                        if (rx_s) begin
                            data  <= shift;
                            valid <= 1'b1;
                        end else begin
                            frame_err <= 1'b1;
                        end
                        state <= S_IDLE;
                    end
            endcase
        end
    end

endmodule
