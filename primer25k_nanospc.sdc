create_clock -name clk_50m -period 20.000 -waveform {0 10.000} [get_ports {clk_50m}]

# GW5A PLL output: 24.528 MHz. Period = 1000 / 24.528 ns.
create_clock -name dclk -period 40.769 -waveform {0 20.3845} [get_nets {dclk}]

# USB host (USB_HOST = 1): second PLL, 48 MHz. Remove this and the clock group
# below when building with USB_HOST = 0 (the net is then a constant).
create_clock -name clk48 -period 20.833 -waveform {0 10.417} [get_nets {clk48}]

# dclk <-> clk48 only through the status snapshot handshake (2-FF toggles).
set_clock_groups -asynchronous -group [get_clocks {dclk}] -group [get_clocks {clk48}]
