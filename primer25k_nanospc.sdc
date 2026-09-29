create_clock -name clk_50m -period 20.000 -waveform {0 10.000} [get_ports {clk_50m}]

# GW5A PLL output: 24.528 MHz. Period = 1000 / 24.528 ns.
create_clock -name dclk -period 40.769 -waveform {0 20.3845} [get_nets {dclk}]

# Host USB HID: PLL de terceiros dedicado gera 60 MHz a partir de clk_50m.
create_clock -name clk_usb -period 16.667 -waveform {0 8.333} [get_nets {clk_usb}]
set_clock_groups -asynchronous -group [get_clocks {clk_usb}] -group [get_clocks {clk_50m}] -group [get_clocks {dclk}]
