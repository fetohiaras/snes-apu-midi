#!/usr/bin/env python3
"""MIDI -> mailbox -> SPC700 driver -> S-DSP end-to-end test.

  check_apu_midi.py script OUT.txt   write the event script for tb_apu_midi
  check_apu_midi.py check WAV SUMMARY analyse the recorded audio

Times are seconds from the first S-DSP sample after the SPC700 starts.
Events are parser-level MIDI messages (status, data1, data2).
"""

from __future__ import annotations

import sys
import wave
from pathlib import Path

import numpy as np

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "tools"))
import soundbank  # noqa: E402

FS = soundbank.DSP_RATE
CLOCKS_PER_SAMPLE = 768
END = 5.25

SCRIPT: list[tuple[float, int, int, int]] = [
    (0.10, 0xB0, 7, 100),                      # CC7 volume 100
    (0.10, 0xC0, 0, 0),                        # program 0: sine
    (0.20, 0x90, 69, 100), (0.70, 0x80, 69, 0),
    (0.80, 0x90, 60, 100), (0.80, 0x90, 64, 100), (0.80, 0x90, 67, 100),
    (1.30, 0x80, 60, 0), (1.30, 0x80, 64, 0), (1.30, 0x80, 67, 0),
    (1.40, 0x90, 69, 127), (1.70, 0x80, 69, 0),
    (1.80, 0x90, 69, 40), (2.10, 0x90, 69, 0),  # Note On velocity 0 = Note Off
    (2.15, 0xB0, 10, 0),                       # pan hard left
    (2.20, 0x90, 72, 100), (2.50, 0x80, 72, 0),
    (2.55, 0xB0, 10, 127),                     # pan hard right
    (2.60, 0x90, 72, 100), (2.90, 0x80, 72, 0),
    (2.95, 0xB0, 10, 64),
    (3.00, 0xC1, 1, 0),                        # program 1 (square) on channel 2: omni
    (3.05, 0x90, 57, 100), (3.35, 0x80, 57, 0),
    (3.40, 0xC0, 0, 0),
    (3.45, 0x90, 69, 100), (3.70, 0xB0, 7, 30), (3.98, 0x80, 69, 0),
    (4.00, 0xB0, 7, 100),
    (4.05, 0xC0, 3, 0),                        # program 3: pluck (decaying)
    (4.10, 0x90, 60, 100), (4.60, 0x80, 60, 0),
    (4.65, 0xC0, 2, 0),                        # program 2: saw
    *[(4.70 + k * 0.005, 0x90, 48 + 2 * k, 100) for k in range(10)],
    (4.75, 0xE0, 0, 64),                       # pitch bend: not mapped, ignored
    (4.76, 0xB0, 1, 64),                       # CC1: not mapped, ignored
    (5.00, 0xB0, 123, 0),                      # all notes off
]

UNMAPPED = sum(1 for _, s, d1, _ in SCRIPT if s >> 4 == 0xE or (s >> 4 == 0xB and d1 not in (7, 10, 120, 123)))


def sample(t: float) -> int:
    return round(t * FS)


def midi_hz(note: int) -> float:
    return 440.0 * 2 ** ((note - 69) / 12)


# ---------------------------------------------------------------------------
# Analysis helpers
# ---------------------------------------------------------------------------

def read_wav(path: Path) -> tuple[np.ndarray, np.ndarray]:
    """Stereo int16 WAV -> (left, right) int16 arrays."""
    with wave.open(str(path)) as w:
        raw = np.frombuffer(w.readframes(w.getnframes()), dtype="<i2")
    return raw[0::2], raw[1::2]


class Audio:
    def __init__(self, path: Path | None = None, l: np.ndarray | None = None, r: np.ndarray | None = None):
        if path is not None:
            l, r = read_wav(path)
        self.l, self.r = l.astype(float), r.astype(float)
        self.mono = (self.l + self.r) / 2

    def seg(self, t0: float, t1: float, ch: str = "mono") -> np.ndarray:
        return getattr(self, ch)[sample(t0):sample(t1)]

    def rms(self, t0: float, t1: float, ch: str = "mono") -> float:
        x = self.seg(t0, t1, ch)
        return float(np.sqrt(np.mean(x * x))) if len(x) else 0.0

    def spectrum(self, t0: float, t1: float) -> tuple[np.ndarray, np.ndarray]:
        x = self.seg(t0, t1)
        x = (x - x.mean()) * np.hanning(len(x))
        n = 8 * len(x)
        mag = np.abs(np.fft.rfft(x, n)) / (np.sum(np.hanning(len(x))) / 2)
        return np.fft.rfftfreq(n, 1 / FS), mag

    def peak(self, t0: float, t1: float, hz: float, width: float = 0.03) -> tuple[float, float]:
        """(frequency, amplitude) of the strongest component within +/-width of hz."""
        f, m = self.spectrum(t0, t1)
        band = (f > hz * (1 - width)) & (f < hz * (1 + width))
        i = np.argmax(np.where(band, m, 0))
        return float(f[i]), float(m[i])


class Checker:
    def __init__(self):
        self.failed = 0
        self.total = 0

    def check(self, ok: bool, name: str, detail: str) -> None:
        self.total += 1
        self.failed += not ok
        print(f"  [{'PASS' if ok else 'FAIL'}] {name}: {detail}")

    def near(self, value: float, target: float, rel: float, name: str, unit: str = "") -> None:
        ok = abs(value - target) <= rel * abs(target)
        self.check(ok, name, f"{value:.4g}{unit} (expected {target:.4g}{unit} +/-{rel * 100:.1f}%)")


def cents(f: float, ref: float) -> float:
    return 1200 * np.log2(f / ref)


def run_checks(a: Audio, summary: dict[str, int]) -> int:
    c = Checker()
    mapped = len(SCRIPT) - UNMAPPED

    print("mailbox")
    c.check(summary["injected"] == len(SCRIPT), "all events injected", f"{summary['injected']}/{len(SCRIPT)}")
    c.check(summary["sent_count"] == mapped, "every mapped event acknowledged by the driver",
            f"{summary['sent_count']} sent, {mapped} expected ({UNMAPPED} unmapped ignored)")
    c.check(summary["dropped"] == 0 and summary["ack_timeout"] == 0, "no FIFO overflow or ack timeout",
            f"dropped={summary['dropped']} ack_timeout={summary['ack_timeout']}")
    c.check(summary["busy_at_end"] == 0, "mailbox idle at the end", f"busy={summary['busy_at_end']}")
    print(f"  longest command backlog: {summary['max_busy_clocks'] / 24.528e3:.2f} ms")

    print("silence and latency")
    c.check(a.rms(0.0, 0.19) < 20, "silent before the first note", f"RMS {a.rms(0.0, 0.19):.1f}")
    x = np.abs(a.seg(0.20, 0.30))
    onset = int(np.argmax(x > 0.1 * x.max())) / FS * 1000
    c.check(onset < 3.0, "note-on to audio latency < 3 ms", f"{onset:.2f} ms")

    print("single note: sine A4")
    f, amp = a.peak(0.30, 0.68, 440)
    c.check(abs(cents(f, 440)) < 5, "A4 pitch", f"{f:.2f} Hz ({cents(f, 440):+.1f} cents)")
    harm = max(a.peak(0.30, 0.68, 440 * k)[1] for k in range(2, 6)) / amp
    c.check(harm < 0.05, "sine is clean (harmonics 2-5)", f"strongest harmonic {harm * 100:.1f}% of fundamental")
    lr = a.rms(0.30, 0.68, "l") / a.rms(0.30, 0.68, "r")
    c.near(lr, 1.0, 0.05, "centre pan L/R balance")
    on_rms = a.rms(0.30, 0.68)
    c.check(a.rms(0.73, 0.79) < 0.01 * on_rms, "released after Note Off",
            f"RMS {a.rms(0.73, 0.79):.1f} vs {on_rms:.0f} while held")

    print("chord: C4 E4 G4")
    peaks = {n: a.peak(0.85, 1.28, midi_hz(n)) for n in (60, 64, 67)}
    top = max(p[1] for p in peaks.values())
    for n, (f, m) in peaks.items():
        c.check(abs(cents(f, midi_hz(n))) < 5 and m > 0.5 * top, f"note {n} present",
                f"{f:.2f} Hz ({cents(f, midi_hz(n)):+.1f} cents), level {m / top:.2f}")
    c.check(a.rms(1.34, 1.39) < 0.01 * a.rms(0.85, 1.28), "chord released", f"RMS {a.rms(1.34, 1.39):.1f}")

    print("velocity")
    hi, lo = a.rms(1.45, 1.68), a.rms(1.85, 2.08)
    # amp = vel * volume / 128 -> 99 for velocity 127, 31 for velocity 40
    c.near(lo / hi, 31 / 99, 0.10, "velocity 40 vs 127 level ratio")
    c.check(a.rms(2.14, 2.19) < 0.01 * lo, "Note On velocity 0 releases", f"RMS {a.rms(2.14, 2.19):.1f}")

    print("pan")
    l, r = a.rms(2.25, 2.48, "l"), a.rms(2.25, 2.48, "r")
    c.check(r < 0.01 * l, "CC10=0 is hard left", f"L {l:.0f}  R {r:.1f}")
    l, r = a.rms(2.65, 2.88, "l"), a.rms(2.65, 2.88, "r")
    c.check(l < 0.01 * r, "CC10=127 is hard right", f"L {l:.1f}  R {r:.0f}")

    print("program change: square A3")
    f1, m1 = a.peak(3.10, 3.33, 220)
    _, m2 = a.peak(3.10, 3.33, 440)
    _, m3 = a.peak(3.10, 3.33, 660)
    c.check(abs(cents(f1, 220)) < 5, "A3 pitch", f"{f1:.2f} Hz ({cents(f1, 220):+.1f} cents)")
    c.near(m3 / m1, 1 / 3, 0.20, "3rd harmonic of a square is ~1/3")
    c.check(m2 / m1 < 0.05, "square has no even harmonics", f"2nd harmonic {m2 / m1 * 100:.1f}%")

    print("live volume (CC7 while a note is held)")
    before, after = a.rms(3.50, 3.68), a.rms(3.75, 3.95)
    # amp 100*100/128 = 78 -> 100*30/128 = 23
    c.near(after / before, 23 / 78, 0.10, "CC7 100 -> 30 level ratio")

    print("envelope: pluck instrument decays while held")
    early, late = a.rms(4.11, 4.16), a.rms(4.50, 4.58)
    c.check(late < 0.5 * early, "decay", f"RMS {early:.0f} -> {late:.0f}")

    print("voice stealing: 10 notes on 8 voices")
    t0, t1 = 4.80, 4.98
    _, ref = a.peak(t0, t1, midi_hz(66))
    for n in (48, 50):
        _, m = a.peak(t0, t1, midi_hz(n), width=0.02)
        c.check(m < 0.1 * ref, f"oldest note {n} was stolen", f"level {m / ref:.3f} of note 66")
    for n in (52, 64, 66):
        f, m = a.peak(t0, t1, midi_hz(n), width=0.02)
        c.check(m > 0.3 * ref, f"note {n} sounding", f"{f:.1f} Hz, level {m / ref:.2f}")
    c.check(a.rms(5.04, 5.20) < 0.01 * a.rms(t0, t1), "CC123 all notes off", f"RMS {a.rms(5.04, 5.20):.1f}")

    print(f"\n{c.total - c.failed}/{c.total} checks passed")
    return 1 if c.failed else 0


def main() -> int:
    if len(sys.argv) == 3 and sys.argv[1] == "script":
        with open(sys.argv[2], "w") as f:
            for t, s, d1, d2 in sorted(SCRIPT, key=lambda e: e[0]):
                f.write(f"{sample(t) * CLOCKS_PER_SAMPLE} {s} {d1} {d2}\n")
        print(f"{sample(END)}")
        return 0
    if len(sys.argv) == 4 and sys.argv[1] == "check":
        summary = {}
        for line in Path(sys.argv[3]).read_text().split("\n"):
            if line.strip():
                k, v = line.split()
                summary[k] = int(v)
        return run_checks(Audio(Path(sys.argv[2])), summary)
    print(__doc__)
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
