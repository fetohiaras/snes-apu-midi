#!/usr/bin/env python3
"""Offline preview of S-DSP voices, for the instrument editor.

One voice follows the hardware (src/spc_core/dsp.v): BRR decoding (the
bit-exact decoder of soundbank.py), pitch stepping with 12-bit fractions,
4-point Gaussian interpolation with the DSP's own table, and the ADSR/GAIN
envelope. Voices are mixed with their velocity/volume, master volume, and
the echo unit (delay, 8-tap FIR, feedback, echo volume). The envelope's
global-counter phase and the echo's exact rounding are approximated, so
this is a listening aid, not a bit-exact model.
"""

from __future__ import annotations

import re
from pathlib import Path

import numpy as np

from instrument import COUNTER_RATES
from soundbank import DSP_RATE, brr_decode, velocity_curve

ROOT = Path(__file__).resolve().parent.parent
MASTER_VOLUME = 0x50        # as in the driver


def _gauss_table() -> np.ndarray:
    text = (ROOT / "src" / "spc_core" / "dsp.vh").read_text()
    body = text[text.index("GTBL[0:511]"):]
    body = body[:body.index("};")]
    vals = [int(v, 16) for v in re.findall(r"12'h([0-9a-fA-F]{3})", body)]
    t = np.array(vals[:512], np.int64)
    return np.where(t >= 0x800, t - 0x1000, t)


GTBL = _gauss_table()


def _decoded(brr: bytes, loop_block: int | None, needed: int) -> np.ndarray:
    """Decoded sample stream, long enough for `needed` samples (looping)."""
    first = np.array(brr_decode(brr), np.int64)
    if loop_block is None:
        return np.concatenate([first, np.zeros(max(0, needed - len(first)) + 4, np.int64)])
    loop = first[loop_block * 16:]
    reps = max(1, (needed - len(first)) // max(1, len(loop)) + 2)
    return np.concatenate([first, np.tile(loop, reps)])


def _envelope(n: int, key_off: int, adsr1: int, adsr2: int, release_gain: int) -> np.ndarray:
    env = np.zeros(n, np.int64)
    e, mode = 0, "attack"
    counter = 0
    for i in range(n):
        if i == key_off:
            mode = "gain" if release_gain else "release"
        if mode == "release":
            e = max(0, e - 8)
        else:
            if mode == "gain":
                rate = release_gain & 31
                cand = e - ((e - 1) >> 8) - 1
            elif mode == "attack":
                rate = ((adsr1 & 15) << 1) | 1
                cand = e + (0x400 if rate == 31 else 0x20)
            elif mode == "decay":
                rate = 0x10 | (((adsr1 >> 4) & 7) << 1)
                cand = e - ((e - 1) >> 8) - 1
            else:
                rate = adsr2 & 31
                cand = e - ((e - 1) >> 8) - 1
            if mode == "decay" and (cand >> 8) & 7 == adsr2 >> 5:
                mode = "sustain"
            if cand > 0x7FF or cand < 0:
                cand = 0x7FF if cand > 0 else 0
                if mode == "attack":
                    mode = "decay"
            period = COUNTER_RATES[rate]
            if period and counter % period == 0:
                e = cand
        env[i] = e
        counter += 1
    return env


def render_voice(inst: dict, note: int, velocity: int, hold: float, total: float) -> np.ndarray:
    """One note: inst has brr, loop_block, pitch (128 words, bytes), adsr1, adsr2, release."""
    n = int(total * DSP_RATE)
    pitch = int.from_bytes(inst["pitch"][2 * note:2 * note + 2], "little")
    pos = np.arange(n, dtype=np.int64) * pitch                   # 12-bit fraction
    idx = pos >> 12
    frac = (pos >> 4) & 0xFF
    s = _decoded(inst["brr"], inst["loop_block"], int(idx[-1]) + 8)
    s = np.concatenate([np.zeros(3, np.int64), s])               # the 3 samples before the first
    s0, s1, s2, s3 = s[idx], s[idx + 1], s[idx + 2], s[idx + 3]
    out = (GTBL[255 - frac] * s0 + GTBL[511 - frac] * s1 + GTBL[256 + frac] * s2 + GTBL[frac] * s3) >> 11
    out = np.clip(out, -32768, 32767) & ~1
    env = _envelope(n, int(hold * DSP_RATE), inst["adsr1"], inst["adsr2"], inst["release"])
    out = (out * env) >> 11
    amp = velocity_curve(velocity) * 100 // 128                  # channel volume 100, centre pan
    return (out * amp) >> 7


def echo(dry: np.ndarray, delay: int, feedback: int, volume: int, fir: list[int]) -> np.ndarray:
    """Wet signal of the echo unit for a (mono) voice mix."""
    d = max(1, delay * 512)                                      # 16 ms = 512 samples per step
    buf = np.zeros(d, np.int64)
    hist = np.zeros(8, np.int64)
    c = np.array(fir, np.int64)
    wet = np.zeros(len(dry), np.int64)
    for i in range(len(dry)):
        k = i % d
        hist = np.roll(hist, -1)
        hist[7] = buf[k] >> 1
        f = int(np.dot(hist, c) >> 6)
        f = max(-32768, min(32767, f))
        wet[i] = (f * volume) >> 7
        buf[k] = max(-32768, min(32767, int(dry[i]) + ((f * feedback) >> 7))) & ~1
    return wet


def render(inst: dict, notes: list[tuple[int, int, float, float]], total: float,
           echo_cfg: dict | None = None) -> np.ndarray:
    """notes: (note, velocity, start s, hold s). Returns float in -1..1 at DSP_RATE."""
    n = int(total * DSP_RATE)
    mix = np.zeros(n, np.int64)
    for note, vel, start, hold in notes:
        a = int(start * DSP_RATE)
        v = render_voice(inst, note, vel, hold, total - start)
        mix[a:a + len(v)] += v[: n - a]
    out = (mix * MASTER_VOLUME) >> 7
    if echo_cfg and echo_cfg.get("voices", 0) and (echo_cfg.get("volume") or echo_cfg.get("feedback")):
        out = out + echo(mix, echo_cfg["delay"], echo_cfg["feedback"], echo_cfg["volume"], echo_cfg["fir"])
    return np.clip(out, -32768, 32767) / 32768.0


def fir_response(fir: list[int], points: int = 256) -> tuple[np.ndarray, np.ndarray]:
    """Magnitude (dB) of the echo FIR (gain c/128) over 0..fs/2."""
    w = np.linspace(0, np.pi, points)
    h = np.array([sum(c / 128 * np.exp(-1j * wk * (7 - i)) for i, c in enumerate(fir)) for wk in w])
    return w / np.pi * DSP_RATE / 2, 20 * np.log10(np.maximum(np.abs(h), 1e-6))


def envelope_curve(adsr1: int, adsr2: int, release: int, hold: float = 1.5, total: float = 2.5):
    n = int(total * DSP_RATE)
    e = _envelope(n, int(hold * DSP_RATE), adsr1, adsr2, release)
    return np.arange(n) / DSP_RATE, e / 0x7FF
