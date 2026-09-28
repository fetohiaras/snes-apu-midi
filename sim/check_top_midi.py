#!/usr/bin/env python3
"""Full-chain integration test of primer25k_nanospc_top.

  check_top_midi.py stimulus DIR BAUD  write DIR/uart.txt (byte schedule for
                                       the midi_rx pin); prints the frame count
  check_top_midi.py check DIR BAUD FORMAT DECIM
                                       evaluate a tb_top_midi run plus its
                                       tb_apu_midi replay (DIR/ref.wav);
                                       FORMAT/DECIM are PC_AUDIO_FORMAT/_DECIM

BAUD is the top's UART_BAUD (the serial link carries MIDI in and PC audio out).

The byte stream plays check_apu_midi.SCRIPT the way a keyboard such as the
CTK-3500 sends it: running status, Note Off as Note On velocity 0, active
sensing, plus a SysEx, MIDI clock bursts and a real-time byte inside a
message. Stages checked:
  1. UART + parser: decoded events equal the script, each within one byte
     time of its last byte.
  2. Mailbox + driver + APU: the parallel APU output is bit-identical to a
     replay of the same events (same clocks) through the verified
     tb_apu_midi path.
  3. Pins: audio decoded from the I2S pins is bit-identical to the APU
     output; BCLK/LRCLK periods; data changes only on BCLK falling edges;
     the PC audio stream on uart_tx is exactly the expected encoding (mu-law
     of the decimated mono mix, or 14-bit mono) of the APU output, with no
     framing errors or overruns; the status LED blinks.
  4. Music: check_apu_midi's pitch/level/pan/envelope checks on the audio
     decoded from the I2S pins.
"""

from __future__ import annotations

import sys
from pathlib import Path

import numpy as np

import check_apu_midi as ref

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "tools"))
import midi_bridge  # noqa: E402

CLK_HZ = 24_528_000
DIAG_LEN = 22

SYSEX_GM_ON = [0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7]


def clk(t: float) -> int:
    return round(t * CLK_HZ)


def build_stream(cpb: float) -> tuple[list[tuple[int, int]], list[tuple[int, int, int, int]]]:
    """Returns ([(start_clock, byte)], [(last_byte_start_clock, s, d1, d2)])."""
    byte_clks = 10 * cpb
    items: list[tuple[float, int, list[int], tuple[int, int, int] | None]] = []
    order = 0
    for t, s, d1, d2 in ref.SCRIPT:
        if s >> 4 == 0x8:                          # Note Off as Note On velocity 0
            s, d2 = 0x90 | (s & 0x0F), 0
        n_data = 1 if s >> 4 in (0xC, 0xD) else 2
        items.append((t, order, [s, d1, d2][:1 + n_data], (s, d1, d2 if n_data == 2 else 0)))
        order += 1
    for k in range(46):                             # active sensing every 100 ms up to
        items.append((0.1 * k, order, [0xFE], None)); order += 1   # 4.5 s, as midi_bridge.py sends
    items.append((0.15, order, SYSEX_GM_ON, None)); order += 1
    for k in range(24):                             # a MIDI clock burst
        items.append((3.2 + k * 0.02, order, [0xF8], None)); order += 1
    items.append((5.45, order, [0xF9], None)); order += 1   # diagnostic request (midi_bridge --diag)
    items.sort(key=lambda i: (i[0], i[1]))

    stream: list[tuple[int, int]] = []
    expected: list[tuple[int, int, int, int]] = []
    line_free = 0.0
    running = None
    inject_rt = True
    for t, _, msg, ev in items:
        start = max(float(clk(t)), line_free)
        if msg[0] >= 0xF8:
            out = msg
        elif msg[0] >= 0xF0:
            out, running = msg, None
        else:
            out = msg[1:] if msg[0] == running else msg
            running = msg[0]
        if ev is not None and inject_rt and ev[0] >> 4 == 0x9 and len(out) >= 2:
            out = out[:-1] + [0xF8] + out[-1:]      # real-time byte inside a message
            inject_rt = False
        for i, b in enumerate(out):
            stream.append((round(start + i * byte_clks), b))
        last_start = round(start + (len(out) - 1) * byte_clks)
        line_free = start + len(out) * byte_clks
        if ev is not None:
            s, d1, d2 = ev
            if s >> 4 == 0x9 and d2 == 0:
                s = 0x80 | (s & 0x0F)               # what the parser reports
            expected.append((last_start, s, d1, d2))
    return stream, expected


def ulaw_ref(x: np.ndarray) -> np.ndarray:
    """G.711 mu-law encoder, a direct port of the reference g711.c linear2ulaw."""
    v = x.astype(np.int32) >> 2
    mask = np.where(v < 0, 0x7F, 0xFF)
    v = np.minimum(np.abs(v), 8159) + 33
    seg = np.searchsorted(np.array([0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF]), v)
    uval = np.where(seg >= 8, 0x7F, (np.minimum(seg, 7) << 4) | ((v >> (np.minimum(seg, 7) + 1)) & 0xF))
    return (uval ^ mask).astype(np.uint8)


def read_summary(path: Path) -> dict[str, int]:
    return {k: int(v) for k, v in (line.split() for line in path.read_text().splitlines() if line.strip())}


def main() -> int:
    if len(sys.argv) < 4 or sys.argv[1] not in ("stimulus", "check"):
        print(__doc__)
        return 2
    d = Path(sys.argv[2])
    baud = int(sys.argv[3])
    fmt = int(sys.argv[4]) if len(sys.argv) > 4 else 1
    decim = int(sys.argv[5]) if len(sys.argv) > 5 else 4
    cpb = CLK_HZ / baud
    stream, expected = build_stream(cpb)

    if sys.argv[1] == "stimulus":
        (d / "uart.txt").write_text("".join(f"{c} {b}\n" for c, b in stream))
        print(ref.sample(ref.END))
        return 0

    c = ref.Checker()
    summ = read_summary(d / "top_summary.txt")

    print(f"stimulus at {baud} baud: {len(stream)} UART bytes, {len(expected)} MIDI messages "
          f"(running status, Note Off as velocity 0, SysEx, active sensing, MIDI clock)")

    print("1. UART pin -> parser events")
    got = [tuple(int(x) for x in line.split()) for line in (d / "top_events.txt").read_text().splitlines()]
    c.check(summ["bytes_sent"] == len(stream), "all bytes sent on midi_rx", f"{summ['bytes_sent']}/{len(stream)}")
    c.check([g[1:] for g in got] == [e[1:] for e in expected], "decoded events equal the script",
            f"{len(got)} events, {len(expected)} expected")
    if len(got) == len(expected):
        delays = [(g[0] - e[0]) / cpb for g, e in zip(got, expected)]
        c.check(all(9.0 <= x <= 10.5 for x in delays), "each event within one byte time of its last byte",
                f"{min(delays):.2f}..{max(delays):.2f} bit times after the last start bit")

    print("2. mailbox -> driver -> APU vs replay of the same events")
    top_l, top_r = ref.read_wav(d / "top_midi.wav")
    ref_l, ref_r = ref.read_wav(d / "ref.wav")
    n = min(len(top_l), len(ref_l))
    same = n > 0 and np.array_equal(top_l[:n], ref_l[:n]) and np.array_equal(top_r[:n], ref_r[:n])
    detail = f"{n} frames"
    if not same and n:
        diff = np.nonzero((top_l[:n] != ref_l[:n]) | (top_r[:n] != ref_r[:n]))[0]
        detail += f", first difference at frame {diff[0]} ({diff[0] / ref.FS:.3f} s), {len(diff)} differ"
    c.check(same, "APU output bit-identical to the tb_apu_midi replay", detail)
    ref_summ = read_summary(d / "ref_summary.txt")
    c.check(summ["sent_count"] == ref_summ["sent_count"], "same number of mailbox commands",
            f"{summ['sent_count']} vs {ref_summ['sent_count']}")

    print("3. output pins")
    i2s_l, i2s_r = ref.read_wav(d / "top_midi_i2s.wav")
    base = summ["i2s_before_audio"]
    delay = next((k for k in range(3)
                  if base + k + n <= len(i2s_l)
                  and np.array_equal(i2s_l[base + k:base + k + n], top_l[:n])
                  and np.array_equal(i2s_r[base + k:base + k + n], top_r[:n])), None)
    c.check(delay is not None, "I2S pins carry the APU output bit-exactly",
            f"{n} frames, pipeline delay {delay} frame(s)" if delay is not None else "no alignment found")
    c.check(summ["bclk_min_period"] == summ["bclk_max_period"] == 12, "BCLK period 12 clocks (2.044 MHz = 64 fs)",
            f"{summ['bclk_min_period']}..{summ['bclk_max_period']}, {summ['bclk_rises']} cycles")
    c.check(summ["lrclk_min_period"] == summ["lrclk_max_period"] == 768, "LRCLK period 768 clocks (31937.5 Hz)",
            f"{summ['lrclk_min_period']}..{summ['lrclk_max_period']}, {summ['lrclk_rises']} frames")
    c.check(summ["lr_sd_off_edge_changes"] == 0, "LRCLK/SDATA change only on BCLK falling edges",
            f"{summ['lr_sd_off_edge_changes']} violations")
    c.check(summ["sdata_toggles"] > 1000, "SDATA active", f"{summ['sdata_toggles']} toggles")
    # PC stream on uart_tx, decoded with the bridge script's own decoders.
    # The last 22 bytes are the diagnostic packet requested at 5.45 s.
    raw = (d / "top_pc_bytes.bin").read_bytes()
    diag = raw[-DIAG_LEN:] if raw[-DIAG_LEN:-DIAG_LEN + 2] == b"\xa5\x5a" else b""
    pc_bytes = raw[:-DIAG_LEN] if diag else raw
    mono = (top_l[:n].astype(np.int32) + top_r[:n].astype(np.int32)) >> 1
    if fmt == 1:
        # The stream starts once the first keep-alive byte arrives, a few
        # samples after the APU; find that offset, then require an exact match.
        got_b = np.frombuffer(pc_bytes, dtype=np.uint8)
        start = None
        for off in range(64):
            groups = (n - off) // decim
            avg = mono[off:off + groups * decim].reshape(groups, decim).sum(axis=1) >> int(np.log2(decim))
            want = ulaw_ref(avg)
            if 0 < len(got_b) <= groups and np.array_equal(got_b, want[:len(got_b)]):
                start = off
                break
        c.check(start is not None,
                f"uart_tx carries the mu-law stream of the APU output exactly (1/{decim} rate)",
                f"{len(got_b)} bytes, stream starts {start} samples after the APU"
                if start is not None else f"{len(got_b)} bytes, no alignment found")
        dec = midi_bridge.decodificar_ulaw(bytes(want)).astype(float)
        snr = 10 * np.log10(np.mean(avg.astype(float) ** 2) / max(np.mean((dec - avg) ** 2), 1e-9))
        c.check(snr > 30, "bridge mu-law decoder reconstructs the signal", f"SNR {snr:.1f} dB")
    else:
        pc, _, sync_err = midi_bridge.decodificar_pcm14(pc_bytes)
        want = (mono >> 2 << 2).astype(np.int16)
        start = next((off for off in range(64)
                      if 0 < len(pc) <= n - off and np.array_equal(pc, want[off:off + len(pc)])), None)
        c.check(start is not None and sync_err == 0 and len(pc) > n // 2,
                "uart_tx carries the 14-bit mono mix of the APU output exactly",
                f"{len(pc)} samples, stream starts {start} samples after the APU, {sync_err} sync errors")
    # Keep-alive: streaming must stop ~PC_AUDIO_KEEPALIVE_MS after the last
    # byte received on midi_rx.
    if start is not None:
        per_sec = ref.FS / (decim if fmt == 1 else 1) * (1 if fmt == 1 else 2)
        last_in = max(c for c, b in stream if b == 0xFE)     # only Active Sensing keeps it alive
        stop = (last_in + 10 * cpb) / CLK_HZ + 0.3
        want_bytes = (stop - start / ref.FS) * per_sec
        c.check(abs(len(pc_bytes) - want_bytes) < 0.01 * per_sec and stop < ref.END - 0.1,
                "stream stops 300 ms after the last keep-alive (0xFE), despite later MIDI",
                f"{len(pc_bytes)} bytes, {want_bytes:.0f} expected (last byte in at {stop - 0.3:.2f} s)")
    c.check(summ["pc_frame_errors"] == 0 and summ["pc_overrun"] == 0,
            f"PC stream clean at {baud} baud",
            f"{summ['pc_bytes']} bytes, framing errors {summ['pc_frame_errors']}, overrun {summ['pc_overrun']}")
    # Diagnostic packet: every field against an independent expectation.
    info = midi_bridge.decodificar_diag(diag) if diag else None
    c.check(info is not None and info["ok"], "diagnostic packet received after 0xF9 (magic, checksum)",
            f"{len(diag)} bytes")
    if info and info["ok"]:
        img = bytes(int(x, 16) for x in (d / "midi_boot.spc.hex").read_text().split())
        sa = sb = 0
        for b in img:
            sa = (sa + b) & 0xFFFF
            sb = (sb + sa) & 0xFFFF
        c.check((info["soma_a"], info["soma_b"]) == (sa, sb) and info["soma_ok"],
                "A-RAM boot checksum matches the image file", f"{info['soma_a']:04X}/{info['soma_b']:04X}, file {sa:04X}/{sb:04X}")
        c.check(info["pc_boot"] == 0x0500, "boot PC from the image header", f"${info['pc_boot']:04X}")
        note_ons = sum(1 for _, st, _, v in ref.SCRIPT if st >> 4 == 9 and v > 0)
        c.check(info["enviados"] == summ["sent_count"] and info["porta3"] == summ["sent_count"] & 0xFF,
                "mailbox counters and driver acknowledgement", f"sent {info['enviados']}, port 3 = {info['porta3']}")
        # +1: the driver's init code clears KON once.
        c.check(info["kon"] == note_ons + 1 and info["escritas_dsp"] > 0 and info["ciclos_05"] > 1000,
                "SPC700 runs the driver and keys on every note",
                f"KON writes {info['kon']} (note-ons {note_ons} + 1 at init), DSP writes {info['escritas_dsp']}, "
                f"page $05 cycles {info['ciclos_05']}")
        peak = int(max(np.abs(top_l.astype(np.int32)).max(), np.abs(top_r.astype(np.int32)).max()))
        c.check(info["pico"] == peak and info["rodando"] and info["audio"] and not info["erros"],
                "status flags and output peak", f"peak {info['pico']} (expected {peak}), flags {info['flags']:08b}")

    # LED: on while booted and silent; off at the first non-zero sample, then
    # bit 14 of the sample counter toggles it every 16384 samples.
    led = [int(x) for x in (d / "top_led.txt").read_text().split()]
    c.check(summ["led_level_at_start"] == 1, "status LED on once the APU runs",
            f"level {summ['led_level_at_start']} when the APU started")
    blink = 16384 * ref.CLOCKS_PER_SAMPLE
    first = led[0] / CLK_HZ if led else -1
    periods = {b - a for a, b in zip(led[1:], led[2:])}
    c.check(0.200 < first < 0.205 and periods == {blink} and len(led) >= 9,
            "status LED: off at first audio, then blinks every 16384 samples",
            f"{len(led)} toggles, first at {first * 1000:.1f} ms, intervals {sorted(p / CLK_HZ for p in periods)} s")

    print("4. music, measured on the audio decoded from the I2S pins")
    if delay is not None:
        audio = ref.Audio(l=i2s_l[base + delay:base + delay + n], r=i2s_r[base + delay:base + delay + n])
        mapped_summary = dict(summ, injected=len(got), max_busy_clocks=0)
        failed = ref.run_checks(audio, mapped_summary)
        c.total += 1
        c.failed += failed
    print(f"\nintegration: {c.total - c.failed}/{c.total} stage checks passed")
    return 1 if c.failed else 0


if __name__ == "__main__":
    raise SystemExit(main())
