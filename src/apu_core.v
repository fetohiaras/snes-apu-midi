`timescale 1ns/1ps

// S-SMP/SPC700 + S-DSP + 64 KiB A-RAM, booted from an SPC image.
//
// After reset the SPC parser restores CPU, S-SMP and S-DSP state from the
// image while the core is disabled; `running` rises once the SPC700 starts.
//
// The port_* signals are the SNES-side view of the four APU I/O ports
// ($2140-$2143 on the S-CPU bus, $F4-$F7 inside the SPC700). A write is
// latched by the S-SMP two clocks after port_wr_n falls, so hold port_a,
// port_din and port_cs stable while port_wr_n is low for at least 3 clocks,
// and keep port_wr_n high for at least 2 clocks between writes.
// port_dout returns what the SPC700 last wrote to port port_a.
module apu_core #(
    parameter MEM_INIT_FILE = "src/data/test_spc.spc.hex"
) (
    input  wire        clk,
    input  wire        resetn,
    output reg         running,

    input  wire [1:0]  port_a,
    input  wire        port_wr_n,
    input  wire        port_cs,
    input  wire [7:0]  port_din,
    output wire [7:0]  port_dout,

    output wire        snd_rdy,
    output wire [15:0] audio_l,
    output wire [15:0] audio_r
);

    wire        smp_en;
    wire        smp_we_n;
    wire [15:0] smp_a;
    wire [7:0]  smp_do;
    wire [7:0]  smp_di;

    wire [15:0] ram_a;
    wire [7:0]  ram_din;
    wire [7:0]  ram_dout;
    wire        ram_wr;

    wire [2:0]  dsp_phase;
    wire        dsp_last_phase;

    wire        parser_done;
    wire        parser_rd;
    wire [16:0] parser_a;
    wire        cpu_dbg_wr;
    wire        smp_dbg_wr;
    wire [7:0]  smpcpu_dbg_reg;
    wire [7:0]  smpcpu_dbg_din;
    wire        dsp_dbg_wr;
    wire [7:0]  dsp_dbg_reg;
    wire [7:0]  dsp_dbg_din;

    reg         parser_start = 1'b0;
    reg         spc_reset = 1'b1;

    localparam [1:0] BOOT_RESET = 2'd0;
    localparam [1:0] BOOT_PARSE = 2'd1;
    localparam [1:0] BOOT_RUN   = 2'd2;
    reg [1:0] boot_state = BOOT_RESET;

    initial running = 1'b0;

    always @(posedge clk) begin
        if (!resetn) begin
            boot_state   <= BOOT_RESET;
            parser_start <= 1'b0;
            running      <= 1'b0;
            spc_reset    <= 1'b1;
        end else begin
            parser_start <= 1'b0;

            case (boot_state)
                BOOT_RESET: begin
                    running      <= 1'b0;
                    spc_reset    <= 1'b1;
                    parser_start <= 1'b1;
                    boot_state   <= BOOT_PARSE;
                end

                BOOT_PARSE: begin
                    // Release the APU reset. With running still low, debug
                    // writes from spc_parser initialise its state.
                    spc_reset <= 1'b0;
                    if (parser_done) begin
                        running    <= 1'b1;
                        boot_state <= BOOT_RUN;
                    end
                end

                default: begin
                    spc_reset <= 1'b0;
                    running   <= 1'b1;
                end
            endcase
        end
    end

    wire apu_resetn = resetn & ~spc_reset;

    SMP smp (
        .CLK             (clk),
        .RST_N           (apu_resetn),
        .ENABLE          (running & smp_en & dsp_last_phase),
        .A               (smp_a),
        .DI              (smp_di),
        .DO              (smp_do),
        .WE_N            (smp_we_n),
        .PA              (port_a),
        .PARD_N          (1'b1),
        .PAWR_N          (port_wr_n),
        .CPU_DI          (port_din),
        .CPU_DO          (port_dout),
        .CS              (port_cs),
        .CS_N            (~port_cs),
        .DBG_REG         (smpcpu_dbg_reg),
        .DBG_DAT_IN      (smpcpu_dbg_din),
        .DBG_SMP_DAT     (),
        .DBG_CPU_DAT     (),
        .DBG_CPU_DAT_WR  (cpu_dbg_wr),
        .DBG_SMP_DAT_WR  (smp_dbg_wr),
        .BRK_OUT         ()
    );

    DSP dsp (
        .CLK         (clk),
        .RST_N       (apu_resetn),
        .ENABLE      (running),
        .PHASE       (dsp_phase),
        .LAST_PHASE  (dsp_last_phase),
        .SMP_EN      (smp_en),
        .SMP_A       (smp_a),
        .SMP_DO      (smp_do),
        .SMP_DI      (smp_di),
        .SMP_WE_N    (smp_we_n),
        .RAM_A       (ram_a),
        .RAM_DI      (ram_dout),
        .RAM_DO      (ram_din),
        .RAM_WR      (ram_wr),
        .LRCK        (),
        .BCK         (),
        .SDAT        (),
        .SND_RDY     (snd_rdy),
        .AUDIO_L     (audio_l),
        .AUDIO_R     (audio_r),
        .DBG_REG     (dsp_dbg_reg),
        .DBG_DAT_IN  (dsp_dbg_din),
        .DBG_DAT_OUT (),
        .DBG_DAT_WR  (dsp_dbg_wr)
    );

    test_aram #(
        .MEM_INIT_FILE(MEM_INIT_FILE)
    ) aram (
        .clk    (clk),
        .din    (ram_din),
        .dout   (ram_dout),
        .wr     (ram_wr),
        .a      (ram_a),
        .spc_rd (parser_rd),
        .spc_wr (1'b0),
        .spc_a  (parser_a),
        .length (),
        .fade   ()
    );

    spc_parser parser (
        .clk             (clk),
        .resetn          (resetn),
        .start           (parser_start),
        .done            (parser_done),
        .parser_rd       (parser_rd),
        .parser_a        (parser_a),
        .aram_dout       (ram_dout),
        .cpu_dbg_wr      (cpu_dbg_wr),
        .smp_dbg_wr      (smp_dbg_wr),
        .smpcpu_dbg_reg  (smpcpu_dbg_reg),
        .smpcpu_dbg_din  (smpcpu_dbg_din),
        .dsp_dbg_wr      (dsp_dbg_wr),
        .dsp_dbg_reg     (dsp_dbg_reg),
        .dsp_dbg_din     (dsp_dbg_din)
    );

endmodule
