`timescale 1ns/1ps

// Translates parsed MIDI events into SPC700 driver commands and delivers them
// through the APU I/O ports:
//
//   port 0  opcode  {command[3:0], midi_channel[3:0]}
//   port 1  arg0
//   port 2  arg1
//   port 3  sequence number, written last (commit)
//
// The driver copies ports 0-2 when port 3 changes and echoes the sequence
// number on its port 3; the next command is only sent after that echo.
//
//   command      MIDI source                 arg0        arg1
//   1 NOTE_ON    9n note vel (vel > 0)       note        velocity
//   2 NOTE_OFF   8n note vel / 9n note 0     note        velocity
//   3 PROGRAM    Cn prog                     program     0
//   4 VOLUME     Bn 07 val                   value       0
//   5 PAN        Bn 0A val                   value       0
//   6 ALL_OFF    Bn 78 xx / Bn 7B xx         0           0
//
// Other messages are ignored. Events arriving while a command is in flight
// wait in a FIFO; if it overflows the event is dropped and `dropped` is set.
module midi_mailbox #(
    parameter integer FIFO_LOG2   = 4,
    parameter integer ACK_TIMEOUT = 2_500_000   // ~100 ms at 24.5 MHz
) (
    input  wire       clk,
    input  wire       resetn,
    input  wire       apu_running,

    input  wire       ev_valid,
    input  wire [7:0] ev_status,
    input  wire [6:0] ev_data1,
    input  wire [6:0] ev_data2,

    output reg  [1:0] port_a,
    output reg        port_wr_n,
    output reg        port_cs,
    output reg  [7:0] port_din,
    input  wire [7:0] port_dout,

    output reg [15:0] sent_count,
    output reg        dropped,
    output reg        ack_timeout,
    output wire       busy
);

    localparam [3:0] CMD_NOTE_ON  = 4'd1;
    localparam [3:0] CMD_NOTE_OFF = 4'd2;
    localparam [3:0] CMD_PROGRAM  = 4'd3;
    localparam [3:0] CMD_VOLUME   = 4'd4;
    localparam [3:0] CMD_PAN      = 4'd5;
    localparam [3:0] CMD_ALL_OFF  = 4'd6;

    // ------------------------------------------------------------------
    // MIDI -> command translation
    // ------------------------------------------------------------------
    reg       map_ok;
    reg [3:0] map_cmd;
    reg [6:0] map_a0, map_a1;

    always @* begin
        map_ok  = 1'b1;
        map_cmd = 4'd0;
        map_a0  = ev_data1;
        map_a1  = 7'd0;
        case (ev_status[7:4])
            4'h9: begin map_cmd = CMD_NOTE_ON;  map_a1 = ev_data2; end
            4'h8: begin map_cmd = CMD_NOTE_OFF; map_a1 = ev_data2; end
            4'hC: map_cmd = CMD_PROGRAM;
            4'hB: begin
                map_a0 = ev_data2;
                case (ev_data1)
                    7'd7:           map_cmd = CMD_VOLUME;
                    7'd10:          map_cmd = CMD_PAN;
                    7'd120, 7'd123: begin map_cmd = CMD_ALL_OFF; map_a0 = 7'd0; end
                    default:        map_ok = 1'b0;
                endcase
            end
            default: map_ok = 1'b0;
        endcase
    end

    // ------------------------------------------------------------------
    // Command FIFO: {opcode, arg0, arg1}
    // ------------------------------------------------------------------
    localparam integer DEPTH = 1 << FIFO_LOG2;

    reg [23:0]        fifo [0:DEPTH-1];
    reg [FIFO_LOG2:0] wr_ptr, rd_ptr;

    wire fifo_empty = (wr_ptr == rd_ptr);
    wire fifo_full  = (wr_ptr[FIFO_LOG2] != rd_ptr[FIFO_LOG2]) &&
                      (wr_ptr[FIFO_LOG2-1:0] == rd_ptr[FIFO_LOG2-1:0]);

    always @(posedge clk) begin
        if (!resetn) begin
            wr_ptr  <= {(FIFO_LOG2+1){1'b0}};
            dropped <= 1'b0;
        end else if (ev_valid && map_ok) begin
            if (fifo_full) begin
                dropped <= 1'b1;
            end else begin
                fifo[wr_ptr[FIFO_LOG2-1:0]] <= {map_cmd, ev_status[3:0], 1'b0, map_a0, 1'b0, map_a1};
                wr_ptr <= wr_ptr + 1'b1;
            end
        end
    end

    // ------------------------------------------------------------------
    // Port write / acknowledge sequencer
    // ------------------------------------------------------------------
    localparam [2:0] S_IDLE = 3'd0, S_SETUP = 3'd1, S_STROBE = 3'd2,
                     S_RECOVER = 3'd3, S_WAIT_ACK = 3'd4;

    localparam integer TO_W = $clog2(ACK_TIMEOUT + 1);
    localparam integer TO_I = ACK_TIMEOUT;
    localparam [TO_W-1:0] TO_MAX = TO_I[TO_W-1:0];

    reg [2:0]      state;
    reg [23:0]     cur;
    reg [7:0]      seq;
    reg [1:0]      idx;
    reg [2:0]      cnt;
    reg [TO_W-1:0] timeout;

    wire [7:0] cur_byte = (idx == 2'd0) ? cur[23:16] :
                          (idx == 2'd1) ? cur[15:8]  :
                          (idx == 2'd2) ? cur[7:0]   : seq;

    assign busy = (state != S_IDLE) || !fifo_empty;

    always @(posedge clk) begin
        if (!resetn) begin
            state       <= S_IDLE;
            rd_ptr      <= {(FIFO_LOG2+1){1'b0}};
            cur         <= 24'd0;
            seq         <= 8'd0;
            idx         <= 2'd0;
            cnt         <= 3'd0;
            timeout     <= {TO_W{1'b0}};
            port_a      <= 2'd3;
            port_wr_n   <= 1'b1;
            port_cs     <= 1'b0;
            port_din    <= 8'h00;
            sent_count  <= 16'd0;
            ack_timeout <= 1'b0;
        end else begin
            case (state)
                S_IDLE:
                    if (apu_running && !fifo_empty) begin
                        cur    <= fifo[rd_ptr[FIFO_LOG2-1:0]];
                        rd_ptr <= rd_ptr + 1'b1;
                        seq    <= seq + 8'd1;
                        idx    <= 2'd0;
                        cnt    <= 3'd1;
                        state  <= S_SETUP;
                    end

                S_SETUP: begin
                    port_a   <= idx;
                    port_din <= cur_byte;
                    port_cs  <= 1'b1;
                    if (cnt == 3'd0) begin
                        port_wr_n <= 1'b0;
                        cnt       <= 3'd3;
                        state     <= S_STROBE;
                    end else begin
                        cnt <= cnt - 3'd1;
                    end
                end

                S_STROBE:
                    if (cnt == 3'd0) begin
                        port_wr_n <= 1'b1;
                        cnt       <= 3'd2;
                        state     <= S_RECOVER;
                    end else begin
                        cnt <= cnt - 3'd1;
                    end

                S_RECOVER:
                    if (cnt == 3'd0) begin
                        if (idx == 2'd3) begin
                            port_cs <= 1'b0;
                            port_a  <= 2'd3;
                            timeout <= TO_MAX;
                            state   <= S_WAIT_ACK;
                        end else begin
                            idx   <= idx + 2'd1;
                            cnt   <= 3'd1;
                            state <= S_SETUP;
                        end
                    end else begin
                        cnt <= cnt - 3'd1;
                    end

                default:
                    if (port_dout == seq) begin
                        sent_count <= sent_count + 16'd1;
                        state      <= S_IDLE;
                    end else if (timeout == {TO_W{1'b0}}) begin
                        ack_timeout <= 1'b1;
                        state       <= S_IDLE;
                    end else begin
                        timeout <= timeout - 1'b1;
                    end
            endcase
        end
    end

endmodule
