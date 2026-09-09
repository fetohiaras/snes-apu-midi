# Project Proposal — Standalone SNES Audio Processing Unit on FPGA

> **Course:** LAB III / Digital Signal Processing project  
> **Team:** _[names]_  
> **Target FPGA:** Sipeed Tang Primer 25K — Gowin GW5A-25 (GW5A-LV25MG121NC1/I0)  
> **Working title:** *Recreating and validating the SNES Audio Processing Unit: SPC700 S-SMP and S-DSP on FPGA*

## 1. Abstract

This project proposes an FPGA implementation and experimental validation of the audio subsystem of the Super Nintendo Entertainment System (SNES). Rather than implementing a complete console, including video and game logic, the work isolates the Audio Processing Unit (APU): the SPC700-derived S-SMP audio processor, the eight-voice S-DSP synthesis engine, and their shared 64 KiB audio RAM (A-RAM).

The core technical objective is to execute an APU program and generate its 32 kHz stereo PCM sample stream in simulation and, if time permits, on a Sipeed Tang Primer 25K FPGA. The first audible result will be produced without requiring an external DAC or audio codec: a SystemVerilog testbench will capture the simulated left/right PCM samples, and a small Python utility will write them to a standard WAV file that can be played and inspected on a PC. This makes audio generation testable from the beginning and avoids making codec bring-up a prerequisite for demonstrating DSP functionality.

The expected full demonstration is a standalone APU image player: a controlled, legal/homebrew SPC700 driver plus BRR-compressed sample bank is loaded into A-RAM; the S-SMP configures the S-DSP over its normal registers; and the S-DSP produces recognizable multi-voice SNES-style audio. An optional later stage adds a PC/UART or MIDI command interface and an I2S audio PMOD/DAC output. The project combines FPGA architecture, fixed-point DSP, audio compression/decompression, real-time scheduling, RTL verification, and retro-computing preservation.

## 2. Motivation

### 2.1 DSP and FPGA relevance

The SNES APU is a compact, historically important example of a real-time digital audio system. It combines an 8-bit control processor with a specialised DSP that performs several operations central to an introductory DSP course:

- BRR predictive/ADPCM sample decompression;
- sample-rate and pitch control by interpolation;
- ADSR/GAIN envelope generation;
- fixed-point multiplication for per-voice and master volume;
- summation and saturation of up to eight voices;
- stereo mixing;
- echo generation and an eight-tap FIR filter.

Unlike a purely software audio project, these operations must be scheduled deterministically in RTL. The S-DSP needs to produce a fresh stereo sample at 32 kHz while sharing memory with the S-SMP. This creates a clear hardware/software partition and a realistic verification problem.

### 2.2 New FPGA hardware and a practical learning path

The project uses the Tang Primer 25K, giving the team a concrete reason to develop competence with the Gowin tool flow, SystemVerilog synthesis, timing constraints, block RAM, clock enables, and board-level integration. The initial simulation-first path is deliberately practical: it gives an audible deliverable before external analog hardware, I2S wiring, or USB audio device implementation is required.

### 2.3 Retro-computing and preservation value

The SNES is notable for its sample-based music and characteristic sound. Recreating its APU separately from the console preserves and exposes a system that is usually hidden inside a larger emulator. The project gives a visible answer to questions such as how compressed instrument data becomes PCM, how game music drivers control synthesis voices, and how limited memory and processing power shaped the sound of an era of games.

The intention is educational and technical rather than to distribute copyrighted game content. Demonstrations will use self-authored, public-domain, homebrew, or otherwise authorised test programs and samples.

## 3. Technical background

### 3.1 SNES audio architecture

The original sound subsystem contains:

| Component | Role in this project |
| --- | --- |
| **S-SMP / SPC700** | 8-bit audio CPU. It executes a music driver, handles timing, and writes S-DSP registers. |
| **S-DSP** | Dedicated eight-voice sample synthesis and effects processor. It does not execute song code. |
| **A-RAM** | 64 KiB shared audio RAM containing the SPC700 program, music sequence, BRR sample data, sample directory, and optionally the echo buffer. |
| **IPL ROM and I/O/timers** | Small boot ROM, four main-CPU communication ports, DSP address/data ports, and three timers. Needed for compatibility with normal APU software. |
| **Audio output** | Stereo PCM sample pair produced at 32 kHz. A DAC converts it to analog sound in a physical console; initially this project captures it as a WAV file instead. |

The full SNES main CPU, cartridge ROM interface, video processor, PPU, HDMI output, controller subsystem, and operating-system/menu functions are intentionally outside project scope.

### 3.2 BRR, instruments, voices, and songs

**BRR** (Bit Rate Reduction) is the S-DSP's compressed instrument-sample format. Each nine-byte block contains a header and eight bytes of 4-bit sample values; the DSP reconstructs sixteen signed sample values using a range and a predictor filter. Blocks can terminate a sample or loop it.

A BRR stream represents an instrument waveform or sound effect, not a whole song. At any instant, the S-DSP has up to eight hardware voices. Each voice selects an entry in the sample directory, reads a BRR sample, applies pitch and an envelope, and contributes to the left/right mix. A software driver on the S-SMP decides which notes and instruments use which voices.

An `.spc` file is an APU state snapshot rather than a universal music format. It generally contains A-RAM and the CPU/DSP state needed to resume playback, but sequence formats differ between game sound engines. This is why tools such as SPC2MID require engine-specific converters.

### 3.3 PCM and WAV

PCM (Pulse-Code Modulation) represents audio as a sequence of signed amplitude samples. For this project, the relevant stream is one signed left value and one signed right value at 32,000 sample pairs per second. A WAV file is a simple container: it has a header describing sample rate, channels, and sample width, followed by PCM bytes. Therefore, a testbench can validate the DSP by recording its sample stream and a Python program can turn it into a playable file.

## 4. Proposed system architecture

### 4.1 Standalone APU boundary

The standalone design keeps the original internal APU relationship while replacing SNES-console-specific logic with a small project wrapper.

```text
             Simulation loader / future UART loader
                         │
                         ▼
    ┌───────────────────────────────────────────────────┐
    │                   apu_standalone                   │
    │                                                   │
    │  host/debug interface ──┐                         │
    │                         ▼                         │
    │                ┌─────────────────┐                │
    │                │ 64 KiB A-RAM    │                │
    │                └───────▲─────────┘                │
    │                        │ scheduled memory access  │
    │   ┌─────────────┐      │      ┌─────────────┐     │
    │   │ S-SMP       │◄─────┴─────►│ S-DSP       │     │
    │   │ SPC700 CPU  │  DSP ports  │ 8 voices    │     │
    │   └─────────────┘              └──────┬──────┘     │
    │                                        │            │
    │                      pcm_left, pcm_right, sample_valid
    └────────────────────────────────────────┼────────────┘
                                             ▼
                            testbench PCM capture / future FIFO
                                             ▼
                                     Python WAV writer
```

The DSP owns the arbitration schedule of the shared memory interface. The S-SMP memory bus is connected to the S-DSP's S-SMP interface, and the S-DSP's RAM interface connects to the A-RAM wrapper. This preserves the intended sharing behaviour instead of treating the CPU and DSP as independent RAM clients.

### 4.2 RTL provenance and extraction plan

The principal upstream reference is the GPL-3.0 SNESTang project. Its source is a complete SNES implementation for Tang boards, but only the APU-relevant RTL will be retained initially.

The initial DSP-only test configuration will use:

- `src/dsp.v` and its included `dsp.vh`;
- `src/CEGen.v` for clock-enable generation;
- newly written project files: A-RAM model, DSP register-write test driver, testbench, and WAV conversion utility.

The full standalone APU configuration will additionally use:

- `src/smp.v`;
- the complete `src/spc700/` CPU RTL group and its packages/microcode;
- the same DSP and clock-enable sources.

The project will **not** initially import the SNES 65C816 main CPU, PPU, HDMI/video RTL, controller logic, SDRAM controller, menu/SD-card system, or RISC-V soft core. These would obscure the audio project and increase resource and debugging risk.

All third-party files will be preserved in an identifiable third-party directory, left unmodified where possible, and documented with their licence and source revision. Locally written wrapper, RAM, loader, verification, capture, and MIDI/host-interface work will be clearly identified as project contributions.

### 4.3 Role of PicoRV32

The Gowin PicoRV32 IP is not a replacement for the SPC700. Replacing the SPC700 would prevent original SPC700 audio-driver code from running and would no longer reproduce the APU architecture.

PicoRV32 is optional as a later **external host controller**. It could load A-RAM, expose status registers, receive MIDI events, or emulate writes to the APU's four communication ports. For the early simulation and hardware bring-up milestones, a simple PC-side loader and small RTL control interface are preferable.

### 4.4 Audio-output strategy

The development order deliberately decouples DSP validation from physical audio output:

1. **Simulation:** capture PCM output at each valid sample event and generate a WAV file.
2. **Real-board capture, optional:** packetise PCM over the board's debugger/UART connection and let Python reconstruct/play it. Full uncompressed 16-bit stereo 32 kHz PCM needs at least approximately 1.28 Mbaud on 8N1 UART before protocol overhead; mono or decimated debug capture can use lower rates.
3. **Physical output, stretch goal:** connect an external I2S audio DAC/PMOD. S/PDIF transport IP is not a codec or DAC and would still need external digital/analog audio hardware.

## 5. Objectives

### 5.1 Primary objective

Develop a standalone FPGA/simulation implementation of the SNES APU data path capable of producing a valid, audible 32 kHz stereo PCM stream from BRR instrument data and an SPC700-controlled S-DSP.

### 5.2 Detailed objectives

1. Study the S-SMP, S-DSP, A-RAM, BRR, DSP register map, timers, and APU boot/communication model.
2. Extract and compile the audio-specific RTL from an upstream FPGA SNES implementation without the rest of the console.
3. Create an A-RAM wrapper and standalone `apu_standalone` top-level module with explicit clock, reset, enable, loader/debug, and PCM output interfaces.
4. Build a DSP-only verification testbench that loads a known legal BRR sample, configures a voice, writes `KON` (key-on), and captures PCM output.
5. Implement a reproducible PCM-to-WAV conversion script and listen to/inspect the output on a PC.
6. Integrate the S-SMP/SPC700 and its timers/I/O with the S-DSP through the normal `$F2/$F3` DSP access mechanism.
7. Run a controlled small SPC700 driver and sample bank from A-RAM, demonstrating that CPU-generated register writes produce music or notes.
8. Synthesize the standalone design for the Tang Primer 25K and measure LUT, register, block-RAM, DSP-block, clock, and timing utilisation.
9. If the board build is successful, create a simple host-loading/debug path and reproduce the PCM output from physical hardware.
10. Document test vectors, known limitations, audio observations, upstream RTL provenance, and the division of original versus reused work.

### 5.3 Measurable success criteria

The project will consider a milestone complete only when it has reproducible evidence:

| Objective | Evidence |
| --- | --- |
| DSP data path works | WAV generated from a deterministic simulation and a documented BRR/register input image. |
| Correct output cadence | Captured sample-valid events correspond to 32 kHz in simulated time. |
| Multi-voice behaviour | At least two independently configured voices can be mixed and heard/observed. |
| S-SMP integration | An SPC700 program writes DSP registers and triggers audio without direct testbench register writes. |
| FPGA feasibility | Successful Gowin synthesis/place-and-route report for the standalone top, or a documented resource/timing limitation. |
| Hardware result, if reached | Captured FPGA PCM stream converted to WAV and compared against the simulation. |

## 6. Development plan and milestones

The phases below are ordered by technical dependency rather than calendar dates. They can be mapped to the course schedule after the team and instructor confirm expected duration.

### Phase A — Environment and source audit

- Create a clean Gowin Education project for the Tang Primer 25K.
- Keep the existing LED/clock project as a known-good board/programming sanity check.
- Obtain and record a fixed upstream SNESTang revision.
- Identify required source files and compilation order; set SystemVerilog 2017 where required.
- Inspect third-party licensing and confirm the course policy on reuse of open RTL.

**Output:** source manifest, build notes, licence/provenance note, and a compiling shell project.

### Phase B — DSP-only simulation

- Instantiate the S-DSP, clock-enable generator, and a simulation A-RAM model.
- Implement a small S-SMP-bus test driver that performs normal DSP address/data writes.
- Load one BRR sample, configure one voice's source, pitch, volume, envelope, directory, and key-on state.
- Capture `AUDIO_L`, `AUDIO_R` when the RTL indicates a fresh sample.
- Convert the capture to WAV and inspect waveform/spectrum/listening result.

**Output:** self-contained testbench, BRR input fixture, PCM capture, WAV file, and a short verification log.

### Phase C — Full standalone APU simulation

- Add `SMP`, the full SPC700 RTL group, IPL ROM/timers/ports, and the scheduled shared A-RAM path.
- Start with a minimal known SPC700 program that configures and triggers an S-DSP voice.
- Replace direct testbench DSP control with real SPC700 writes through `$F2/$F3`.
- Validate several notes/voices and, if practical, echo/filter behaviour.

**Output:** `apu_standalone`, simple A-RAM image, audio WAV generated by the complete CPU + DSP system.

### Phase D — APU image/snapshot loader and comparison

- Define an input image format suitable for simulation: A-RAM bytes plus necessary CPU/DSP initial state.
- Optionally create a PC-side preparer for an authorised `.spc` state or a team-created format.
- Compare output against a reference emulator or a previous stable simulation output. Exact mid-song `.spc` restoration may require special treatment because not every internal timing state is saved.

**Output:** documented image format, loader/preparer, reference comparison notes, and one reproducible demonstration track or sequence.

### Phase E — FPGA synthesis and board bring-up

- Replace simulation RAM with Gowin-compatible block RAM implementation.
- Add only necessary top-level constraints: 50 MHz board clock, reset/control button if used, status LED, and optional UART pins.
- Run synthesis/place-and-route early and iterate on RAM inference, clock enables, and timing.
- Use LED/status counters to show reset, loaded, running, and sample-active states.

**Output:** Gowin project, constraint file, bitstream, utilisation/timing report, and board demonstration evidence.

### Phase F — Stretch interfaces

- Implement PC/UART A-RAM loading and PCM capture.
- Add a small command mailbox. MIDI events from a PC or serial MIDI receiver can become note/program/velocity commands for a team-authored SPC700 music driver.
- Add I2S PMOD DAC output only after the simulation and FPGA PCM stream are stable.

**Output:** optional interactive MIDI demonstration and/or live external audio output.

## 7. Deliverables and scope scenarios

### 7.1 Conservative minimum deliverable (worst-case but valid project outcome)

If full S-SMP integration, board timing, or external I/O becomes difficult, the project will still deliver a complete and defensible DSP experiment:

- isolated S-DSP RTL simulation;
- documented BRR decoding and DSP register configuration;
- one or more reproducible BRR voice tests;
- captured 32 kHz PCM converted to a WAV file;
- waveform/listening analysis and explanation of the fixed-point DSP chain;
- source/provenance report and a documented assessment of why the next integration stage was blocked.

This is not merely an audio file conversion: the WAV is generated from the RTL DSP model after BRR decoding, interpolation/mixing, and audio scheduling.

### 7.2 Expected deliverable

- standalone `apu_standalone` containing S-SMP/SPC700, S-DSP, and 64 KiB A-RAM;
- minimal SPC700 driver running from A-RAM and programming the S-DSP;
- several instrument/note or short musical-sequence demonstrations;
- deterministic WAV output from simulation;
- successful Tang Primer 25K synthesis/place-and-route and resource report;
- clear architecture, verification, limitations, licence, and contribution documentation.

### 7.3 Best-case deliverable

All expected deliverables, plus:

- standalone APU image/snapshot loading on the FPGA;
- PCM data captured from the physical FPGA and played on a PC;
- UART/PC command protocol;
- MIDI input controlling a team-authored SPC700 driver and selected BRR instrument bank;
- I2S PMOD/DAC real-time stereo output;
- comparison of FPGA output with a reference implementation and a demonstration video.

### 7.4 Explicit non-goals for the initial proposal

- Full SNES console emulation, game loading, video/HDMI, and controller emulation.
- Generic `.sf2` soundfont parsing in FPGA.
- Generic conversion of arbitrary `.spc` files to MIDI.
- USB Audio Device implementation on the board's debugger USB-C interface.
- Claiming perfect transistor/cycle-level reproduction before the core audio milestones are verified.

These are deliberately excluded to preserve a coherent DSP-focused project.

## 8. Risk assessment and mitigations

| Risk | Impact | Mitigation |
| --- | --- | --- |
| Upstream source may be incomplete, difficult to extract, or have hidden dependencies | Full APU integration delayed | Start with the S-DSP-only vertical slice; retain an audio-only reference project for comparison; make source dependencies explicit. |
| Fidelity differs between upstream implementations | Output may not match every original APU nuance | State the fidelity goal honestly; use controlled tests; compare audio/waveforms; prioritise BRR, voices, envelopes, mixing, and timing before rare edge cases. |
| Tang Primer 25K resource/timing limits | Full core may fail P&R | Synthesize early; avoid importing whole SNES; use block RAM efficiently; keep PicoRV32 and physical interfaces as later options. |
| Shared A-RAM access is subtle | Incorrect CPU/DSP behaviour or audio artifacts | Preserve the DSP's existing scheduled memory interface; do not substitute naive simultaneous RAM access. |
| SystemVerilog/vendor-tool compatibility | Simulation may compile while synthesis fails | Use a small reproducible source set; compile incrementally; set the appropriate language standard; replace only platform glue as needed. |
| A `.spc` snapshot does not capture every internal timing state | Exact mid-song reproduction may differ | Begin with a known program at reset/start; describe `.spc` loading as an advanced experiment; use reference captures for qualitative/functional checks. |
| UART bandwidth is insufficient for raw stereo PCM at a selected baud rate | Hardware streaming glitches or data loss | Capture small blocks, use a higher baud rate, use mono/debug modes, or defer live streaming until after simulation proof. |
| No onboard audio codec | Live sound output delayed | WAV-based simulation is a core deliverable; use PC playback or an external I2S DAC only later. |
| MIDI integration becomes too large | Feature creep | Keep MIDI as a stretch goal; start with a small custom command protocol and a fixed BRR bank. |
| Copyright or licensing ambiguity | Distribution/course-policy problem | Use authorised test assets; document SNESTang GPL-3.0 provenance; ask the instructor about accepted third-party RTL reuse before submission. |
| Reused RTL reduces perceived original contribution | Assessment risk | Define original work up front: standalone architecture, board port, RAM/loader, host/MIDI interface, verification, capture tooling, tests, measurement, documentation, and modifications. |

## 9. Hardware and software resources

### Hardware

- Sipeed Tang Primer 25K FPGA board.
- Board clock and READY/status LED for early bring-up.
- Optional serial/UART path via the board debugger connection.
- Optional later I2S audio PMOD/DAC and amplifier/headphones.
- Optional serial MIDI interface or PC-mediated MIDI input.

### Software and tool flow

- Gowin FPGA Designer Education Edition for synthesis, place-and-route, and programming.
- SystemVerilog/Verilog RTL.
- A simulator compatible with the chosen source set (for example, Verilator or another course-approved simulator).
- Python 3 standard library (`wave`, `struct`) or equivalent for PCM/WAV conversion and optional serial capture.
- Git for recording upstream revision and project changes.

## 10. Verification methodology

Verification will be treated as a project feature, not an afterthought.

1. **Unit-style DSP tests:** known BRR block input, decoded sample values, register-write behaviour, key-on/key-off, and pitch/envelope cases.
2. **Integration tests:** S-SMP program writes through `$F2/$F3`; verify that S-DSP state changes and PCM starts at the expected point.
3. **Audio capture tests:** each `sample_valid`/`SND_RDY` event is recorded with its simulation timestamp and stereo values.
4. **WAV inspection:** listen for expected pitch/looping/envelope behaviour; inspect waveform and optionally use a spectrum plot.
5. **Regression fixtures:** keep fixed legal/homebrew input images and expected hashes/statistics so RTL changes can be checked reproducibly.
6. **FPGA comparison:** if board capture is implemented, compare duration, sample count, waveform segments, and audible output against simulation.

The project report will distinguish bit-exact tests, where available, from qualitative listening tests. It will avoid claiming compatibility beyond the cases that were actually exercised.

## 11. Anticipated learning outcomes

By completing the proposed work, the team expects to gain experience with:

- FPGA project setup, constraints, clocking, block RAM, and synthesis reports;
- RTL integration of a processor, a DSP, and shared memory;
- hardware scheduling and clock-enable design;
- signed fixed-point arithmetic and overflow/saturation concerns;
- BRR/ADPCM compression and decompression;
- PCM sample streams, WAV files, sample rate, and stereo audio;
- FIR echo filtering, interpolation, envelopes, and multi-voice mixing;
- testbench-driven verification and host-side analysis tools;
- open-source RTL provenance, licensing, and responsible retro-computing practice.

## 12. Source and knowledge base

### FPGA implementations and project references

- [SNESTang — Super Nintendo Entertainment System for Tang FPGA boards](https://github.com/nand2mario/snestang) — primary APU RTL reference; GPL-3.0.
- [SNESTang `smp.v`](https://github.com/nand2mario/snestang/blob/main/src/smp.v) — S-SMP wrapper, ports, timers, IPL ROM, and SPC700 integration.
- [SNESTang `dsp.v`](https://github.com/nand2mario/snestang/blob/main/src/dsp.v) — S-DSP, scheduled RAM interface, PCM outputs, and sample-valid signal.
- [SNESTang Primer 25K project file](https://github.com/nand2mario/snestang/blob/main/snestang_primer25k.gprj) — useful source-file inventory and target device reference.
- [fpga-spc700](https://github.com/brandonpelfrey/fpga-spc700) — audio-only FPGA recreation; useful simulation, UART, DSP scheduling, and DAC bring-up reference. Its README documents current fidelity limitations, so it should be treated as a comparison/bring-up aid rather than assumed fully accurate.
- [TangCore build documentation](https://nand2mario.github.io/tangcore/dev-guide/building/) — upstream build context; not necessarily the recommended minimal flow for this standalone project.
- [FPGA SID](https://www.fpgasid.de/) — earlier retro-audio FPGA inspiration considered during brainstorming.

### SNES APU, SPC700, S-DSP, and BRR documentation

- [Official SNES Development Manual](https://floating.muncher.se/bot/manual/book1_text.pdf) — especially the sound-source chapter beginning around PDF page 152 / chapter 3.
- [SnesLab: S-SMP](https://sneslab.net/wiki/S-SMP) — short architectural description of the audio CPU.
- [SnesLab: S-DSP](https://sneslab.net/wiki/S-DSP) — DSP overview.
- [SnesLab: BRR](https://sneslab.net/wiki/S-DSP/BRR) — BRR compression overview.
- [Super Famicom Development Wiki: SPC700 Reference](https://wiki.superfamicom.org/spc700-reference) — CPU, memory, ports, timers, BRR, and DSP reference.
- [SNESdev Wiki: S-DSP registers](https://snes.nesdev.org/wiki/S-DSP_registers) — voice, directory, key-on, echo, and global register details.
- [SNESdev Wiki: BRR samples](https://snes.nesdev.org/wiki/BRR_samples) — BRR sample storage and links to low-level references.
- [SPC-700 Programming Information](https://snesmusic.org/files/spc700_documentation.html) — practical older programming notes, sample encoding discussion, and register examples.
- Local downloaded notes: `spc700.txt`, `apudsp_jwdonal.txt`, and `spc700cyc.txt` in `~/Downloads` — supplementary timing and implementation references already collected by the team.

### Music analysis and MIDI

- [SPC2MID](https://github.com/turboboy215/SPC2MID) — game-engine-specific SPC-to-MIDI converters. Useful for studying driver diversity, not as a generic SPC/MIDI format solution.
- [MIDI Association](https://midi.org/) — MIDI protocol and educational material for any later controller/interface stage.

### DSP, audio, and FPGA learning references

- [The Scientist and Engineer's Guide to Digital Signal Processing — Steven W. Smith](https://www.analog.com/en/resources/technical-books/scientist_engineers_guide.html) — free practical DSP text. Priority topics: sampling/ADC/DAC, linear systems, convolution, digital filters, and audio processing.
- [The Scientist and Engineer's Guide to DSP — alternate chapter index](https://cmp.felk.cvut.cz/cmp/courses/dzo/resources/book_dsp_smith/) — convenient free chapter navigation.
- Richard G. Lyons, *Understanding Digital Signal Processing* — recommended follow-up textbook for a more detailed DSP foundation.
- Uwe Meyer-Baese, *Digital Signal Processing with Field Programmable Gate Arrays* — later reference for FPGA-oriented DSP implementation, fixed-point design, and architectures.

### Gowin / board documentation retained locally

- Tang Primer 25K board schematic, user documentation, constraints, and Gowin toolchain documents in `/home/tilc/Prog/SV/Sipeed Tang Primer 25K`.
- Gowin PicoRV32 documentation and reference designs in `/home/tilc/Prog/SV/Sipeed Tang Primer 25K/Gowin PicoRV32`.
- Gowin DSP/IP documentation, including hard multiplier and DSP blocks, retained in the same local knowledge base.

## 13. Questions to confirm with the instructor

1. Is reuse and modification of GPL-licensed open-source RTL acceptable if clearly attributed and submitted under compatible terms?
2. What fraction of the assessed work must be original RTL versus integration, verification, and system design?
3. Is simulation-generated WAV output an acceptable primary DSP demonstration, with physical DAC/MIDI as stretch objectives?
4. Is an authorised/homebrew sample bank preferable to any captured historical game state for the final demonstration?
5. Are there required report sections, timing/resource targets, or mandatory hardware demonstrations that should alter the milestone order?

---

## Revision notes

- _[Date]_ Initial proposal drafted from project brainstorming and source audit.
- _[Date]_ _[Add later changes, scope decisions, instructor feedback, and selected upstream revision.]_
