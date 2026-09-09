TOP       := primer25k_nanospc_top
BUILD_DIR := obj_dir

RTL := \
	../src/primer25k_nanospc_top.v \
	../src/spc_core/dsp.v \
	../src/spc_core/smp.v \
	../src/spc_core/spc_parser.v \
	../src/spc_core/test_aram.v \
	../src/spc_core/spc700/_spc700_package.v \
	../src/spc_core/spc700/addrgen.v \
	../src/spc_core/spc700/addsub.v \
	../src/spc_core/spc700/alu.v \
	../src/spc_core/spc700/bcdadj.v \
	../src/spc_core/spc700/mcode.v \
	../src/spc_core/spc700/muldiv.v \
	../src/spc_core/spc700/spc700.v

.PHONY: build run spc-to-hex clean

build: $(BUILD_DIR)/V$(TOP)

$(BUILD_DIR)/V$(TOP): sim_main.cpp $(RTL)
	mkdir -p $(BUILD_DIR)
	verilator --sv --top-module $(TOP) --trace --cc --exe -O3 \
		--Mdir $(BUILD_DIR) \
		-I../src -I../src/spc_core -I../src/spc_core/spc700 \
		$(RTL) sim_main.cpp
	$(MAKE) -C $(BUILD_DIR) -f V$(TOP).mk V$(TOP)

# Run from the project root: test_aram's relative image path is src/data/...
# The default is a three-second capture at the actual board sample rate.
run: build
	mkdir -p output
	cd .. && sim/$(BUILD_DIR)/V$(TOP) --output sim/output/primer25k_nanospc_smoke.wav

# Example:
# make spc-to-hex SPC="/home/tilc/Downloads/MMX/01 Title Screen.spc"
# This deliberately changes the image that both simulation and synthesis load.
SPC ?=
HEX ?= ../src/data/test_spc.spc.hex
spc-to-hex:
	@test -n "$(SPC)" || (echo 'Set SPC=/path/to/song.spc'; exit 2)
	python3 ../tools/spc_to_hex.py "$(SPC)" "$(HEX)"

clean:
	rm -rf $(BUILD_DIR) output
