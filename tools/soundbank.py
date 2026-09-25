#!/usr/bin/env python3
"""BRR codec and the synthesizer's fixed instrument bank.

The BRR decoder follows the S-DSP arithmetic bit-exactly (as documented by
blargg's snes_spc), and the encoder searches every filter/shift per block
against that decoder, so what we measure in simulation is what we encoded.
"""

from __future__ import annotations

import math
from dataclasses import dataclass


def _clamp16(v: int) -> int:
    return max(-32768, min(32767, v))


def _wrap16(v: int) -> int:
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def _decode_nibble(nib: int, shift: int, filt: int, p1: int, p2_raw: int) -> int:
    """One BRR sample; p1/p2_raw are the previous two decoded outputs."""
    s = nib - 16 if nib & 8 else nib
    s = (s << shift) >> 1
    if shift >= 13:
        s = -2048 if s < 0 else 0
    p2 = p2_raw >> 1
    if filt == 1:
        s += p1 >> 1
        s += (-p1) >> 5
    elif filt == 2:
        s += p1 - p2 + (p2 >> 4) + ((p1 * -3) >> 6)
    elif filt == 3:
        s += p1 - p2 + ((p1 * -13) >> 7) + ((p2 * 3) >> 4)
    return _wrap16(_clamp16(s) * 2)


def brr_decode(data: bytes, loops: int = 1) -> list[int]:
    """Decode a BRR stream (optionally repeating from block 0) to int16 samples."""
    out: list[int] = []
    p1 = p2 = 0
    for _ in range(loops):
        for blk in range(0, len(data), 9):
            hdr = data[blk]
            shift, filt = hdr >> 4, (hdr >> 2) & 3
            for byte in data[blk + 1: blk + 9]:
                for nib in (byte >> 4, byte & 15):
                    s = _decode_nibble(nib, shift, filt, p1, p2)
                    p2, p1 = p1, s
                    out.append(s)
    return out


def brr_encode(samples: list[int], loop: bool = True) -> bytes:
    """Encode int16 samples (multiple of 16) choosing the best filter/shift
    per block. Block 0 uses filter 0 so a loop back to it is seamless."""
    assert len(samples) % 16 == 0
    out = bytearray()
    p1 = p2 = 0
    nblocks = len(samples) // 16
    for b in range(nblocks):
        target = samples[b * 16:(b + 1) * 16]
        best = None
        for filt in ((0,) if b == 0 else (0, 1, 2, 3)):
            for shift in range(13):
                q1, q2, err, nibs = p1, p2, 0, []
                for t in target:
                    cand = min(range(16),
                               key=lambda n: abs(_decode_nibble(n, shift, filt, q1, q2) - t))
                    s = _decode_nibble(cand, shift, filt, q1, q2)
                    err += (s - t) ** 2
                    q2, q1 = q1, s
                    nibs.append(cand)
                if best is None or err < best[0]:
                    best = (err, filt, shift, nibs, q1, q2)
        _, filt, shift, nibs, p1, p2 = best
        end = b == nblocks - 1
        out.append((shift << 4) | (filt << 2) | ((1 if loop else 0) << 1 if end else 0) | (1 if end else 0))
        out.extend((nibs[i] << 4) | nibs[i + 1] for i in range(0, 16, 2))
    return bytes(out)


# ---------------------------------------------------------------------------
# Instrument bank
# ---------------------------------------------------------------------------

CYCLE_LEN = 32             # samples per single-cycle waveform
PEAK = 14000               # keeps BRR's (s * 2) wrap well out of reach
# The board's PLL gives 24.528 MHz and the S-DSP emits a sample every 768
# clocks, i.e. 31937.5 Hz rather than the console's 32000 Hz; tuning against
# the real rate avoids a 3.4-cent flat offset.
DSP_RATE = 24_528_000 / 768
REF_HZ = DSP_RATE / CYCLE_LEN  # frequency at PITCH = 0x1000


def _additive(harmonics: dict[int, float]) -> list[int]:
    raw = [sum(a * math.sin(2 * math.pi * k * i / CYCLE_LEN) for k, a in harmonics.items())
           for i in range(CYCLE_LEN)]
    scale = PEAK / max(abs(v) for v in raw)
    return [round(v * scale) for v in raw]


# Band-limited to the 15 harmonics a 32-sample cycle can hold.
WAVES = {
    "sine":     _additive({1: 1.0}),
    "square":   _additive({k: 1.0 / k for k in range(1, 16, 2)}),
    "saw":      _additive({k: 1.0 / k for k in range(1, 16)}),
    "triangle": _additive({k: (1.0 if (k // 2) % 2 == 0 else -1.0) / (k * k) for k in range(1, 16, 2)}),
}


@dataclass
class Instrument:
    name: str
    wave: str
    adsr1: int     # bit 7 enables ADSR; 6-4 decay rate; 3-0 attack rate
    adsr2: int     # 7-5 sustain level; 4-0 sustain rate
    transpose: int = 0


# Program number = index (MIDI program modulo len(INSTRUMENTS)).
INSTRUMENTS = [
    Instrument("sine",   "sine",     0x8F, 0xE0),          # instant attack, full sustain
    Instrument("square", "square",   0x8F, 0xE0),
    Instrument("saw",    "saw",      0x8F, 0xE0),
    Instrument("pluck",  "triangle", 0xFF, 0x2E),          # fast decay to low sustain, then fades
]


def pitch_for_note(note: int, transpose: int = 0) -> int:
    """S-DSP PITCH register value that plays `note` from a REF_HZ cycle."""
    hz = 440.0 * 2 ** ((note + transpose - 69) / 12)
    return min(0x3FFF, max(0, round(0x1000 * hz / REF_HZ)))


def pitch_table() -> bytes:
    """128 little-endian words, indexed by MIDI note."""
    return b"".join(pitch_for_note(n).to_bytes(2, "little") for n in range(128))


if __name__ == "__main__":
    for name, wave in WAVES.items():
        brr = brr_encode(wave)
        dec = brr_decode(brr, loops=2)[CYCLE_LEN:]
        err = math.sqrt(sum((a - b) ** 2 for a, b in zip(wave, dec)) / CYCLE_LEN)
        print(f"{name:9s} {len(brr)} bytes, loop RMS error {err:6.1f} ({20 * math.log10(err / PEAK + 1e-12):.1f} dBFS-rel)")
    print("A4 pitch", hex(pitch_for_note(69)))
