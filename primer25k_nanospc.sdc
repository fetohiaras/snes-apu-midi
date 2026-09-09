create_clock -name clk_50m -period 20.000 -waveform {0 10.000} [get_ports {clk_50m}]

# GW5A PLL output: 24.528 MHz. Period = 1000 / 24.528 ns.
create_clock -name dclk -period 40.769 -waveform {0 20.3845} [get_nets {dclk}]
