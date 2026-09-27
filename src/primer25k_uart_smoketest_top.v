`timescale 1ns/1ps

// Tang Primer 25K diagnostic for the onboard BL616 USB-UART path only.
// Every correctly received byte toggles READY, so no MIDI parser/APU/DAC is
// involved.  Use tools/uart_smoke.py to send one byte at a time.
module primer25k_uart_smoketest_top #(
    parameter integer UART_BAUD = 115200
) (
    input  wire clk_50m,
    input  wire midi_rx,
    output wire led_ready
);

    reg [17:0] por_count = 18'd0;
    wire resetn = &por_count;
    always @(posedge clk_50m) begin
        if (!resetn)
            por_count <= por_count + 18'd1;
    end

    wire [7:0] rx_byte;
    wire       rx_valid;
    wire       frame_err;

    uart_rx #(
        .CLK_HZ(50000000),
        .BAUD(UART_BAUD)
    ) uart (
        .clk       (clk_50m),
        .resetn    (resetn),
        .rx        (midi_rx),
        .data      (rx_byte),
        .valid     (rx_valid),
        .frame_err (frame_err)
    );

    reg led_state = 1'b0;
    always @(posedge clk_50m) begin
        if (!resetn)
            led_state <= 1'b0;
        else if (rx_valid)
            led_state <= ~led_state;
    end

    assign led_ready = led_state;

endmodule
