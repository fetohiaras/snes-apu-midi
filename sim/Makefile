TOP       := primer25k_nanospc_top
BUILD_DIR := obj_dir

APU_RTL := \
	../src/apu_core.v \
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

MIDI_IN_RTL := ../src/midi/uart_rx.v ../src/midi/midi_parser.v ../src/midi/midi_mailbox.v
RTL := ../src/primer25k_nanospc_top.v ../src/i2s_tx.v $(MIDI_IN_RTL) $(APU_RTL)

.PHONY: build run test-i2s test-midi test-apu-midi test-top-midi midi-image spc-to-hex clean

build: $(BUILD_DIR)/V$(TOP)

I2S_DIR := obj_i2s

$(I2S_DIR)/Vi2s_tx: tb_i2s_tx.cpp ../src/i2s_tx.v
	mkdir -p $(I2S_DIR)
	verilator --sv --top-module i2s_tx --cc --exe -O3 -Wall \
		--Mdir $(I2S_DIR) ../src/i2s_tx.v tb_i2s_tx.cpp
	$(MAKE) -C $(I2S_DIR) -f Vi2s_tx.mk Vi2s_tx

test-i2s: $(I2S_DIR)/Vi2s_tx
	$(I2S_DIR)/Vi2s_tx

MIDI_DIR := obj_midi
MIDI_RTL := ../src/midi/uart_rx.v ../src/midi/midi_parser.v tb_midi_top.v

$(MIDI_DIR)/Vtb_midi_top: tb_midi.cpp $(MIDI_RTL)
	mkdir -p $(MIDI_DIR)
	verilator --sv --top-module tb_midi_top --cc --exe -O3 -Wall \
		--Mdir $(MIDI_DIR) $(MIDI_RTL) tb_midi.cpp
	$(MAKE) -C $(MIDI_DIR) -f Vtb_midi_top.mk Vtb_midi_top

test-midi: $(MIDI_DIR)/Vtb_midi_top
	$(MIDI_DIR)/Vtb_midi_top

# End-to-end MIDI test: builds the SPC700 driver image (needs spcasm; set
# SPCASM=/path/to/spcasm if it is not on PATH), plays check_apu_midi.py's
# event script through midi_mailbox + apu_core and checks the audio.
SPCASM       ?= spcasm
MIDI_IMG_DIR := build_midi
APU_MIDI_DIR := obj_apu_midi

$(MIDI_IMG_DIR)/midi_boot.spc.hex: ../src/driver/midi_driver.s ../tools/build_midi_image.py \
		../tools/soundbank.py ../tools/spc_to_hex.py
	SPCASM=$(SPCASM) python3 ../tools/build_midi_image.py $(MIDI_IMG_DIR)

$(APU_MIDI_DIR)/Vtb_apu_midi_top: tb_apu_midi.cpp tb_apu_midi_top.v ../src/midi/midi_mailbox.v $(APU_RTL)
	mkdir -p $(APU_MIDI_DIR)
	verilator --sv --top-module tb_apu_midi_top --cc --exe -O3 \
		--Mdir $(APU_MIDI_DIR) \
		-I../src -I../src/spc_core -I../src/spc_core/spc700 \
		tb_apu_midi_top.v ../src/midi/midi_mailbox.v $(APU_RTL) tb_apu_midi.cpp
	$(MAKE) -C $(APU_MIDI_DIR) -f Vtb_apu_midi_top.mk Vtb_apu_midi_top

test-apu-midi: $(APU_MIDI_DIR)/Vtb_apu_midi_top $(MIDI_IMG_DIR)/midi_boot.spc.hex
	mkdir -p output
	python3 check_apu_midi.py script $(MIDI_IMG_DIR)/script.txt > $(MIDI_IMG_DIR)/samples.txt
	$(APU_MIDI_DIR)/Vtb_apu_midi_top $(MIDI_IMG_DIR)/script.txt $$(cat $(MIDI_IMG_DIR)/samples.txt) \
		output/apu_midi.wav $(MIDI_IMG_DIR)/summary.txt
	python3 check_apu_midi.py check output/apu_midi.wav $(MIDI_IMG_DIR)/summary.txt

# The synthesis top boots src/data/midi_boot.spc.hex; `make midi-image`
# (re)generates it before running Gowin.
../src/data/midi_boot.spc.hex: $(MIDI_IMG_DIR)/midi_boot.spc.hex
	cp $< $@

midi-image: ../src/data/midi_boot.spc.hex

# Full-chain integration: MIDI bytes on the midi_rx pin of the real top,
# checked stage by stage (see check_top_midi.py).
TOP_MIDI_DIR := obj_top_midi

$(TOP_MIDI_DIR)/V$(TOP): tb_top_midi.cpp $(RTL)
	mkdir -p $(TOP_MIDI_DIR)
	verilator --sv --top-module $(TOP) --cc --exe -O3 \
		--Mdir $(TOP_MIDI_DIR) \
		-I../src -I../src/spc_core -I../src/spc_core/spc700 \
		$(RTL) tb_top_midi.cpp
	$(MAKE) -C $(TOP_MIDI_DIR) -f V$(TOP).mk V$(TOP)

test-top-midi: $(TOP_MIDI_DIR)/V$(TOP) $(APU_MIDI_DIR)/Vtb_apu_midi_top ../src/data/midi_boot.spc.hex
	python3 check_top_midi.py stimulus $(MIDI_IMG_DIR) > $(MIDI_IMG_DIR)/top_frames.txt
	cd .. && sim/$(TOP_MIDI_DIR)/V$(TOP) sim/$(MIDI_IMG_DIR)/uart.txt \
		$$(cat sim/$(MIDI_IMG_DIR)/top_frames.txt) sim/$(MIDI_IMG_DIR)
	$(APU_MIDI_DIR)/Vtb_apu_midi_top $(MIDI_IMG_DIR)/top_events.txt $$(cat $(MIDI_IMG_DIR)/top_frames.txt) \
		$(MIDI_IMG_DIR)/ref.wav $(MIDI_IMG_DIR)/ref_summary.txt
	python3 check_top_midi.py check $(MIDI_IMG_DIR)

$(BUILD_DIR)/V$(TOP): sim_main.cpp $(RTL)
	mkdir -p $(BUILD_DIR)
	verilator --sv --top-module $(TOP) --trace --cc --exe -O3 \
		--Mdir $(BUILD_DIR) \
		'-GBOOT_IMAGE="src/data/test_spc.spc.hex"' \
		-I../src -I../src/spc_core -I../src/spc_core/spc700 \
		$(RTL) sim_main.cpp
	$(MAKE) -C $(BUILD_DIR) -f V$(TOP).mk V$(TOP)

# Run from the project root: test_aram's relative image path is src/data/...
# The default is a three-second capture at the actual board sample rate.
run: build
	mkdir -p output
	cd .. && sim/$(BUILD_DIR)/V$(TOP) --output sim/output/primer25k_nanospc_smoke.wav \
		--i2s-output sim/output/primer25k_nanospc_i2s.wav

# Example:
# make spc-to-hex SPC="/home/tilc/Downloads/MMX/01 Title Screen.spc"
# This deliberately changes the image that both simulation and synthesis load.
SPC ?=
HEX ?= ../src/data/test_spc.spc.hex
spc-to-hex:
	@test -n "$(SPC)" || (echo 'Set SPC=/path/to/song.spc'; exit 2)
	python3 ../tools/spc_to_hex.py "$(SPC)" "$(HEX)"

clean:
	rm -rf $(BUILD_DIR) $(I2S_DIR) $(MIDI_DIR) $(APU_MIDI_DIR) $(TOP_MIDI_DIR) $(MIDI_IMG_DIR) output
