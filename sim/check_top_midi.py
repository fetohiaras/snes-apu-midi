#!/usr/bin/env python3
"""Full-chain integration test of primer25k_nanospc_top.

  check_top_midi.py stimulus DIR    write DIR/uart.txt (byte schedule for the
                                    midi_rx pin); prints the frame count
  check_top_midi.py check DIR       evaluate a tb_top_midi run plus its
                                    tb_apu_midi replay (DIR/ref.wav)

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
     the status LED blinks.
  4. Music: check_apu_midi's pitch/level/pan/envelope checks on the audio
     decoded from the I2S pins.
"""

from __future__ import annotations

import sys
from pathlib import Path

import numpy as np

import check_apu_midi as ref

CLK_HZ = 24_528_000
CPB = CLK_HZ / 31250            # clocks per UART bit
BYTE_CLKS = 10 * CPB

SYSEX_GM_ON = [0xF0, 0x7E, 0x7F, 0x09, 0x01, 0xF7]


def clk(t: float) -> int:
    return round(t * CLK_HZ)


def build_stream() -> tuple[list[tuple[int, int]], list[tuple[int, int, int, int]]]:
    """Returns ([(start_clock, byte)], [(last_byte_start_clock, s, d1, d2)])."""
    items: list[tuple[float, int, list[int], tuple[int, int, int] | None]] = []
    order = 0
    for t, s, d1, d2 in ref.SCRIPT:
        if s >> 4 == 0x8:                          # Note Off as Note On velocity 0
            s, d2 = 0x90 | (s & 0x0F), 0
        n_data = 1 if s >> 4 in (0xC, 0xD) else 2
        items.append((t, order, [s, d1, d2][:1 + n_data], (s, d1, d2 if n_data == 2 else 0)))
        order += 1
    for k in range(18):                             # active sensing every 300 ms
        items.append((0.05 + 0.3 * k, order, [0xFE], None)); order += 1
    items.append((0.15, order, SYSEX_GM_ON, None)); order += 1
    for k in range(24):                             # a MIDI clock burst
        items.append((3.2 + k * 0.02, order, [0xF8], None)); order += 1
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
            stream.append((round(start + i * BYTE_CLKS), b))
        last_start = round(start + (len(out) - 1) * BYTE_CLKS)
        line_free = start + len(out) * BYTE_CLKS
        if ev is not None:
            s, d1, d2 = ev
            if s >> 4 == 0x9 and d2 == 0:
                s = 0x80 | (s & 0x0F)               # what the parser reports
            expected.append((last_start, s, d1, d2))
    return stream, expected


def read_summary(path: Path) -> dict[str, int]:
    return {k: int(v) for k, v in (line.split() for line in path.read_text().splitlines() if line.strip())}


def main() -> int:
    if len(sys.argv) != 3 or sys.argv[1] not in ("stimulus", "check"):
        print(__doc__)
        return 2
    d = Path(sys.argv[2])
    stream, expected = build_stream()

    if sys.argv[1] == "stimulus":
        (d / "uart.txt").write_text("".join(f"{c} {b}\n" for c, b in stream))
        print(ref.sample(ref.END))
        return 0

    c = ref.Checker()
    summ = read_summary(d / "top_summary.txt")

    print(f"stimulus: {len(stream)} UART bytes, {len(expected)} MIDI messages "
          f"(running status, Note Off as velocity 0, SysEx, active sensing, MIDI clock)")

    print("1. UART pin -> parser events")
    got = [tuple(int(x) for x in line.split()) for line in (d / "top_events.txt").read_text().splitlines()]
    c.check(summ["bytes_sent"] == len(stream), "all bytes sent on midi_rx", f"{summ['bytes_sent']}/{len(stream)}")
    c.check([g[1:] for g in got] == [e[1:] for e in expected], "decoded events equal the script",
            f"{len(got)} events, {len(expected)} expected")
    if len(got) == len(expected):
        delays = [(g[0] - e[0]) / CPB for g, e in zip(got, expected)]
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
