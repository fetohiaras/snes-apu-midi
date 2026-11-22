#!/usr/bin/env python3
"""Sample-based instruments for the S-DSP: SoundFont sample -> BRR.

  1. truncate to the length budget; keep the SoundFont loop if it fits,
     otherwise build one at the end of the window: a whole number of
     periods of the root note, crossfaded so the jump is seamless
  2. resample (FFT, so the loop stays exactly periodic) by a ratio chosen
     to make the loop a multiple of 16 samples, and pad the start so the
     loop begins on a BRR block
  3. normalise and BRR-encode (filter 0 on the loop's first block)
  4. per-instrument pitch table: the S-DSP PITCH for every MIDI note, from
     the sample's root key, tuning and the resampling ratio, so tuning is
     exact whatever the rate

The time and level values the S-DSP envelope can take are here too.
"""

from __future__ import annotations

import math
from dataclasses import dataclass

import numpy as np

from sf2 import Sample
from soundbank import DSP_RATE, brr_decode

# ---------------------------------------------------------------------------
# S-DSP envelope tables (fullsnes / blargg): time to cross the full range
# ---------------------------------------------------------------------------
ATTACK_MS = [4100, 2600, 1500, 1000, 640, 380, 260, 160, 96, 64, 40, 24, 16, 10, 6, 0]
DECAY_MS = [1200, 740, 440, 290, 180, 110, 74, 37]
# sustain rate / GAIN rate: exponential decrease time to ~1/8 (0 = never)
SR_MS = [0, 38000, 28000, 24000, 19000, 14000, 12000, 9400, 7100, 5900, 4700, 3500, 2900, 2400,
         1800, 1500, 1200, 880, 740, 590, 440, 370, 290, 220, 180, 150, 110, 92, 74, 55, 37, 18]
# samples between envelope steps for each 5-bit rate (0 = never)
COUNTER_RATES = [0, 2048, 1536, 1280, 1024, 768, 640, 512, 384, 320, 256, 192, 160, 128, 96, 80,
                 64, 48, 40, 32, 24, 20, 16, 12, 10, 8, 6, 5, 4, 3, 2, 1]

PEAK = 14000            # normalised peak: room for 8 voices plus echo
MAX_PEAK = 30000        # ... unless the body is quiet (see prepare)
BODY_RMS = 4500


def nearest(table: list[int], ms: float, skip_zero: bool = False) -> int:
    best = min((abs(v - ms), i) for i, v in enumerate(table) if not (skip_zero and v == 0))
    return best[1]


def adsr_from_sf2(s: Sample) -> dict:
    """First guess of the S-DSP envelope from the SoundFont's (editable in the GUI)."""
    sl = int(round(8 * 10 ** (-s.sustain_db / 20))) - 1
    sustain_level = max(0, min(7, sl))
    # a sustain at -100 dB means "decays while held": use the decay time as sustain rate
    sr = 0 if s.sustain_db < 1 else nearest(SR_MS, s.decay_s * 1000, skip_zero=True)
    return {
        "attack": nearest(ATTACK_MS, s.attack_s * 1000),
        "decay": nearest(DECAY_MS, min(s.decay_s, 1.2) * 1000),
        "sustain_level": sustain_level if s.sustain_db >= 1 else 7,
        "sustain_rate": sr,
        "release": nearest(SR_MS, s.release_s * 1000, skip_zero=True) if s.release_s > 0.02 else 0,
    }


def adsr_regs(env: dict) -> tuple[int, int, int]:
    """(ADSR1, ADSR2, release GAIN byte; 0 = hardware key-off release)."""
    adsr1 = 0x80 | (env["decay"] << 4) | env["attack"]
    adsr2 = (env["sustain_level"] << 5) | env["sustain_rate"]
    rel = env.get("release", 0)
    return adsr1, adsr2, (0xA0 | rel) if rel else 0       # GAIN: exponential decrease


# ---------------------------------------------------------------------------
# Fast BRR encoder: all filter/shift candidates of a block in parallel
# ---------------------------------------------------------------------------
_CAND_F = np.repeat(np.arange(4), 13)
_CAND_S = np.tile(np.arange(13), 4)


def _decode_vec(nib, shift, filt, p1, p2raw):
    """usb soundbank._decode_nibble, vectorised (int64 arrays)."""
    s = np.where(nib >= 8, nib - 16, nib)
    s = (s << shift) >> 1
    p2 = p2raw >> 1
    pred = np.zeros_like(s)
    pred = np.where(filt == 1, (p1 >> 1) + ((-p1) >> 5), pred)
    pred = np.where(filt == 2, p1 - p2 + (p2 >> 4) + ((p1 * -3) >> 6), pred)
    pred = np.where(filt == 3, p1 - p2 + ((p1 * -13) >> 7) + ((p2 * 3) >> 4), pred)
    s = np.clip(s + pred, -32768, 32767) * 2
    return ((s + 32768) & 0xFFFF) - 32768


def brr_encode_fast(samples: np.ndarray, loop_block: int | None) -> bytes:
    """BRR-encode int16 samples (length multiple of 16). Blocks 0 and
    loop_block use filter 0 so playback can enter them with any history."""
    x = np.asarray(samples, dtype=np.int64)
    assert len(x) % 16 == 0
    nblocks = len(x) // 16
    out = bytearray()
    p1 = p2 = 0
    for b in range(nblocks):
        target = x[b * 16:(b + 1) * 16]
        sel = (_CAND_F == 0) if b == 0 or b == loop_block else np.ones(52, bool)
        f, sh = _CAND_F[sel], _CAND_S[sel]
        n = len(f)
        q1 = np.full(n, p1, np.int64)
        q2 = np.full(n, p2, np.int64)
        err = np.zeros(n, np.int64)
        nibs = np.zeros((n, 16), np.int64)
        for i, t in enumerate(target):
            base = _decode_vec(np.zeros(n, np.int64), sh, f, q1, q2)       # prediction with nibble 0
            step = np.maximum(1, (1 << sh) >> 1) * 2                         # output units per nibble step
            guess = np.clip(np.round((t - base) / step), -8, 7).astype(np.int64)
            best_e = None
            for d in (-1, 0, 1):
                g = np.clip(guess + d, -8, 7)
                v = _decode_vec(g & 15, sh, f, q1, q2)
                e = (v - t) ** 2
                if best_e is None:
                    best_e, best_g, best_v = e, g, v
                else:
                    better = e < best_e
                    best_e = np.where(better, e, best_e)
                    best_g = np.where(better, g, best_g)
                    best_v = np.where(better, v, best_v)
            err += best_e
            nibs[:, i] = best_g & 15
            q2, q1 = q1, best_v
        k = int(np.argmin(err))
        p1, p2 = int(q1[k]), int(q2[k])
        end = b == nblocks - 1
        flags = (3 if loop_block is not None else 1) if end else 0
        out.append((int(sh[k]) << 4) | (int(f[k]) << 2) | flags)
        out.extend((int(nibs[k, i]) << 4) | int(nibs[k, i + 1]) for i in range(0, 16, 2))
    return bytes(out)


# ---------------------------------------------------------------------------
# Sample preparation
# ---------------------------------------------------------------------------
def _fft_resample(x: np.ndarray, n: int) -> np.ndarray:
    if n == len(x):
        return x.astype(float)
    X = np.fft.rfft(x)
    m = n // 2 + 1
    Y = np.zeros(m, complex)
    k = min(len(X), m)
    Y[:k] = X[:k]
    return np.fft.irfft(Y, n) * (n / len(x))


@dataclass
class Prepared:
    pcm: np.ndarray             # int16 at the playback rate, multiple of 16 samples
    brr: bytes
    loop_block: int | None
    rate: float                 # sample rate the data is meant to play at with PITCH 0x1000 = rate/DSP_RATE
    root_hz: float              # fundamental of the root key in this data, at `rate`
    loop_note: str              # where the loop came from

    @property
    def size(self) -> int:
        return len(self.brr)


def prepare(s: Sample, target_rate: int = 16000, max_ms: int = 800) -> Prepared:
    x = s.data.astype(float)
    root_hz = 440.0 * 2 ** ((s.root_key - 69) / 12 + s.cents / 1200)
    max_len = int(s.rate * max_ms / 1000)
    rho = min(1.0, target_rate / s.rate)

    if s.loop_start >= 0 and s.loop_end <= max_len:
        ls, le = s.loop_start, s.loop_end
        note = "loop do SoundFont"
    elif s.loop_start >= 0 or len(x) > max_len // 2:
        # own loop at the end of the window: whole periods, ~20-40 ms long
        le = min(len(x), max_len)
        period = s.rate / root_hz
        periods = max(1, round(0.03 * s.rate / period))
        L = int(round(periods * period))
        ls = max(L, le - L)
        if ls < L or le - ls < 8:
            ls, le = -1, -1
            note = "sem loop"
        else:
            fade = min(L // 2, int(0.01 * s.rate))
            i = np.arange(fade) / fade
            x = x.copy()
            x[le - fade:le] = x[le - fade:le] * (1 - i) + x[ls - fade:ls] * i
            note = f"loop criado ({periods} períodos)"
    else:
        ls = le = -1
        note = "sem loop"

    if ls >= 0:
        L = le - ls
        L16 = max(16, int(round(L * rho / 16)) * 16)
        rho = L16 / L
        attack = _fft_resample(x[:ls], max(1, int(round(ls * rho))))
        loop = _fft_resample(x[ls:le], L16)
        pad = (-len(attack)) % 16
        data = np.concatenate([np.zeros(pad), attack, loop])
        loop_block = (pad + len(attack)) // 16
    else:
        x = x[:max_len]
        n = int(round(len(x) * rho))
        data = _fft_resample(x, n)
        fade = min(len(data), int(0.005 * target_rate))
        if fade:
            data[-fade:] *= np.linspace(1, 0, fade)
        data = np.concatenate([data, np.zeros((-len(data)) % 16 + 16)])
        loop_block = None
    # Level: by the peak, but samples whose body (the loop) is much quieter
    # than their attack get up to BODY_RMS there, letting the attack reach
    # full scale (otherwise a piano sounds far softer than an organ), and
    # dense waves (square) are held to 1.6 x BODY_RMS.
    peak = np.max(np.abs(data)) or 1.0
    body = data[loop_block * 16:] if loop_block is not None else data
    rms = float(np.sqrt(np.mean(body ** 2))) or 1.0
    gain = min(max(PEAK / peak, min(MAX_PEAK / peak, BODY_RMS / rms)), 1.6 * BODY_RMS / rms)
    pcm = np.round(np.clip(data * gain, -32767, 32767)).astype(np.int16)
    brr = brr_encode_fast(pcm, loop_block)
    return Prepared(pcm=pcm, brr=brr, loop_block=loop_block, rate=s.rate * rho, root_hz=root_hz, loop_note=note)


def pitch_table(p: Prepared) -> bytes:
    """128 little-endian PITCH words: note n plays 440*2**((n-69)/12) Hz."""
    out = bytearray()
    for n in range(128):
        hz = 440.0 * 2 ** ((n - 69) / 12)
        out += min(0x3FFF, max(1, round(0x1000 * (hz / p.root_hz) * (p.rate / DSP_RATE)))).to_bytes(2, "little")
    return bytes(out)


def snr_db(p: Prepared) -> float:
    dec = np.array(brr_decode(p.brr), float)[: len(p.pcm)]
    e = np.mean((dec - p.pcm) ** 2)
    return 10 * math.log10(np.mean(p.pcm.astype(float) ** 2) / e) if e > 0 else 99.0


if __name__ == "__main__":
    import sys
    import time
    from sf2 import SoundFont
    sf = SoundFont(sys.argv[1] if len(sys.argv) > 1 else "/usr/share/sounds/sf2/FluidR3_GM.sf2")
    for prog in (0, 4, 19, 33, 48, 56, 73, 80):
        s = sf.sample_for(0, prog, 60)
        t = time.time()
        p = prepare(s)
        print(f"{prog:3d} {sf.preset_name(0, prog):22s} {p.size:6d} B  loop blk {p.loop_block}  "
              f"rate {p.rate:7.0f}  SNR {snr_db(p):5.1f} dB  {p.loop_note}  ({time.time() - t:.1f}s)  "
              f"env {adsr_from_sf2(s)}")
