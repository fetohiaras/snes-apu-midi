`timescale 1ns/1ps

// Tang Primer 25K diagnostic: SPC700 + APU ports + midi_mailbox, without UART
// and without an external DAC.
//
// A tiny SPC image runs an SPC700 loop that mirrors port $F7 back to $F7.
// The FPGA sends synthetic NOTE_ON/OFF events through midi_mailbox.  Every
// acknowledged command increments mb_sent, so the READY LED changes state.
// If the LED repeatedly changes after startup, the parser, SPC700 execution,
// APU communication ports and mailbox handshake are all alive.
module primer25k_core_smoketest_top #(
    parameter BOOT_IMAGE = "src/data/core_smoke.spc.hex"
) (
    input  wire clk_50m,
    output wire led_ready
);

    localparam integer DCLK_HZ = 24528000;

    wire dclk;
    Gowin_PLL pll_dsp (
        .clkin   (clk_50m),
        .clkout0 (dclk),
        .mdclk   (clk_50m)
    );

    // Same conservative startup delay used by the main design.
    reg [18:0] por_count = 19'd0;
    wire resetn = &por_count;
    always @(posedge clk_50m) begin
        if (!resetn)
            por_count <= por_count + 19'd1;
    end

    wire        apu_running;
    wire        ev_valid;
    wire [7:0]  ev_status;
    wire [6:0]  ev_data1;
    wire [6:0]  ev_data2;
    wire        mb_busy;

    apu_selftest_events #(
        .CLK_HZ(DCLK_HZ)
    ) stimulus (
        .clk          (dclk),
        .resetn       (resetn),
        .apu_running  (apu_running),
        .mailbox_busy (mb_busy),
        .ev_valid     (ev_valid),
        .ev_status    (ev_status),
        .ev_data1     (ev_data1),
        .ev_data2     (ev_data2)
    );

    wire [1:0]  port_a;
    wire        port_wr_n;
    wire        port_cs;
    wire [7:0]  port_din;
    wire [7:0]  port_dout;
    wire [15:0] mb_sent;
    wire        mb_dropped;
    wire        mb_timeout;

    midi_mailbox mb (
        .clk         (dclk),
        .resetn      (resetn),
        .apu_running (apu_running),
        .ev_valid    (ev_valid),
        .ev_status   (ev_status),
        .ev_data1    (ev_data1),
        .ev_data2    (ev_data2),
        .port_a      (port_a),
        .port_wr_n   (port_wr_n),
        .port_cs     (port_cs),
        .port_din    (port_din),
        .port_dout   (port_dout),
        .sent_count  (mb_sent),
        .dropped     (mb_dropped),
        .ack_timeout (mb_timeout),
        .busy        (mb_busy)
    );

    wire        snd_rdy;
    wire [15:0] audio_l;
    wire [15:0] audio_r;

    apu_core #(
        .MEM_INIT_FILE(BOOT_IMAGE)
    ) apu (
        .clk       (dclk),
        .resetn    (resetn),
        .running   (apu_running),
        .port_a    (port_a),
        .port_wr_n (port_wr_n),
        .port_cs   (port_cs),
        .port_din  (port_din),
        .port_dout (port_dout),
        .snd_rdy   (snd_rdy),
        .audio_l   (audio_l),
        .audio_r   (audio_r)
    );

    // One LED, three useful states:
    //   before APU starts / error: fixed state
    //   APU started, no ACK yet: opposite fixed state
    //   commands acknowledged: toggles on every successful command
    // This deliberately avoids depending on an audio DAC.
    assign led_ready = !resetn                    ? 1'b0 :
                       !apu_running               ? 1'b0 :
                       (mb_timeout | mb_dropped)  ? 1'b0 :
                       (mb_sent == 16'd0)         ? 1'b1 :
                                                    mb_sent[0];

endmodule
