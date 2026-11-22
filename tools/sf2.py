#!/usr/bin/env python3
"""Minimal SoundFont 2 reader: presets, and for a preset + key the sample
zone that plays it (sample data, rate, root key, tuning, loop, envelope).

Only what the instrument builder needs: preset/instrument zones with key
and velocity ranges, the global zones, sample offsets, tuning and the
volume-envelope generators. Modulators are ignored.
"""

from __future__ import annotations

import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path

import numpy as np

# generator numbers (SoundFont 2.04, section 8.1.2)
G_START_OFS, G_END_OFS, G_LOOP_START_OFS, G_LOOP_END_OFS = 0, 1, 2, 3
G_START_COARSE, G_END_COARSE = 4, 12
G_ATTEN = 48
G_DELAY_VOL, G_ATTACK_VOL, G_HOLD_VOL, G_DECAY_VOL, G_SUSTAIN_VOL, G_RELEASE_VOL = 33, 34, 35, 36, 37, 38
G_INSTRUMENT = 41
G_KEY_RANGE, G_VEL_RANGE = 43, 44
G_LOOP_START_COARSE = 45
G_COARSE_TUNE, G_FINE_TUNE = 51, 52
G_SAMPLE_ID, G_SAMPLE_MODES = 53, 54
G_LOOP_END_COARSE = 50
G_ROOT_KEY = 58

# defaults for the volume envelope (timecents; sustain in centibels)
DEFAULTS = {G_DELAY_VOL: -12000, G_ATTACK_VOL: -12000, G_HOLD_VOL: -12000,
            G_DECAY_VOL: -12000, G_SUSTAIN_VOL: 0, G_RELEASE_VOL: -12000}


@dataclass
class Zone:
    gens: dict[int, int] = field(default_factory=dict)
    key_range: tuple[int, int] = (0, 127)
    vel_range: tuple[int, int] = (0, 127)


@dataclass
class Sample:
    """One playable sample of a preset, resolved for a key."""
    name: str
    data: np.ndarray            # int16, mono
    rate: int
    root_key: int
    cents: float                # tuning correction to apply (sample + zone)
    loop_start: int             # relative to data, or -1 if not looped
    loop_end: int
    attack_s: float
    decay_s: float
    sustain_db: float           # attenuation of the sustain level (dB)
    release_s: float
    attenuation_db: float


def _timecents(tc: int) -> float:
    return 0.0 if tc <= -12000 else 2.0 ** (tc / 1200.0)


class SoundFont:
    def __init__(self, path: str | Path):
        self.path = Path(path)
        raw = self.path.read_bytes()
        if raw[:4] != b"RIFF" or raw[8:12] != b"sfbk":
            raise ValueError(f"{path}: not a SoundFont 2 file")
        self._chunks: dict[str, bytes] = {}
        self._walk(raw, 12, len(raw))
        smpl = self._chunks["smpl"]
        self._smpl = np.frombuffer(smpl[: len(smpl) // 2 * 2], dtype="<i2")
        self._parse_pdta()

    def _walk(self, raw: bytes, pos: int, end: int) -> None:
        while pos + 8 <= end:
            cid = raw[pos:pos + 4].decode("latin-1")
            size = struct.unpack_from("<I", raw, pos + 4)[0]
            if cid == "LIST":
                self._walk(raw, pos + 12, pos + 8 + size)
            else:
                self._chunks[cid] = raw[pos + 8:pos + 8 + size]
            pos += 8 + size + (size & 1)

    @staticmethod
    def _records(data: bytes, fmt: str) -> list[tuple]:
        n = struct.calcsize(fmt)
        return [struct.unpack_from(fmt, data, i) for i in range(0, len(data) - n + 1, n)]

    def _parse_pdta(self) -> None:
        c = self._chunks
        self._phdr = self._records(c["phdr"], "<20sHHHIII")
        self._pbag = self._records(c["pbag"], "<HH")
        self._pgen = self._records(c["pgen"], "<Hh")
        self._inst = self._records(c["inst"], "<20sH")
        self._ibag = self._records(c["ibag"], "<HH")
        self._igen = self._records(c["igen"], "<Hh")
        self._shdr = self._records(c["shdr"], "<20sIIIIIBbHH")
        self.presets = []       # (bank, program, name, index), the terminal record excluded
        for i, (name, prog, bank, *_rest) in enumerate(self._phdr[:-1]):
            self.presets.append((bank, prog, name.split(b"\0")[0].decode("latin-1").strip(), i))
        self.presets.sort()

    def _zones(self, bags: list[tuple], gens: list[tuple], first: int, last: int) -> list[Zone]:
        zones = []
        for b in range(first, last):
            z = Zone()
            for g in range(bags[b][0], bags[b + 1][0]):
                oper, amount = gens[g]
                if oper in (G_KEY_RANGE, G_VEL_RANGE):
                    lo, hi = amount & 0xFF, (amount >> 8) & 0xFF
                    if oper == G_KEY_RANGE:
                        z.key_range = (lo, hi)
                    else:
                        z.vel_range = (lo, hi)
                else:
                    z.gens[oper] = amount
            zones.append(z)
        return zones

    def preset_name(self, bank: int, program: int) -> str:
        for b, p, n, _ in self.presets:
            if (b, p) == (bank, program):
                return n
        raise KeyError(f"no preset {bank}:{program}")

    def sample_for(self, bank: int, program: int, key: int = 60, velocity: int = 100) -> Sample:
        idx = next(i for b, p, _, i in self.presets if (b, p) == (bank, program))
        pz = self._zones(self._pbag, self._pgen, self._phdr[idx][3], self._phdr[idx + 1][3])
        pglobal = pz[0].gens if pz and G_INSTRUMENT not in pz[0].gens else {}
        for p in pz:
            if G_INSTRUMENT not in p.gens:
                continue
            if not (p.key_range[0] <= key <= p.key_range[1] and p.vel_range[0] <= velocity <= p.vel_range[1]):
                continue
            inst = p.gens[G_INSTRUMENT]
            iz = self._zones(self._ibag, self._igen, self._inst[inst][1], self._inst[inst + 1][1])
            iglobal = iz[0].gens if iz and G_SAMPLE_ID not in iz[0].gens else {}
            for z in iz:
                if G_SAMPLE_ID not in z.gens:
                    continue
                if not (z.key_range[0] <= key <= z.key_range[1] and z.vel_range[0] <= velocity <= z.vel_range[1]):
                    continue
                g = dict(DEFAULTS)
                g.update(iglobal)
                g.update(z.gens)
                # preset-level generators are relative offsets (added)
                for src in (pglobal, p.gens):
                    for k, v in src.items():
                        if k in (G_INSTRUMENT, G_KEY_RANGE, G_VEL_RANGE, G_SAMPLE_ID):
                            continue
                        g[k] = g.get(k, 0) + v
                return self._resolve(g)
        raise KeyError(f"preset {bank}:{program} has no zone for key {key}")

    def _resolve(self, g: dict[int, int]) -> Sample:
        name, start, end, ls, le, rate, orig, corr, _link, _typ = self._shdr[g[G_SAMPLE_ID]]
        start += g.get(G_START_OFS, 0) + 32768 * g.get(G_START_COARSE, 0)
        end += g.get(G_END_OFS, 0) + 32768 * g.get(G_END_COARSE, 0)
        ls += g.get(G_LOOP_START_OFS, 0) + 32768 * g.get(G_LOOP_START_COARSE, 0)
        le += g.get(G_LOOP_END_OFS, 0) + 32768 * g.get(G_LOOP_END_COARSE, 0)
        data = self._smpl[start:end].astype(np.int16)
        looped = (g.get(G_SAMPLE_MODES, 0) & 3) in (1, 3) and start <= ls < le <= end and le - ls >= 8
        root = g.get(G_ROOT_KEY, -1)
        root = orig if root < 0 else root
        cents = corr + g.get(G_FINE_TUNE, 0) + 100 * g.get(G_COARSE_TUNE, 0)
        return Sample(
            name=name.split(b"\0")[0].decode("latin-1").strip(),
            data=data, rate=rate, root_key=root, cents=cents,
            loop_start=ls - start if looped else -1, loop_end=le - start if looped else -1,
            attack_s=_timecents(g[G_ATTACK_VOL]) + _timecents(g[G_HOLD_VOL]),
            decay_s=_timecents(g[G_DECAY_VOL]),
            sustain_db=max(0, g[G_SUSTAIN_VOL]) / 10.0,
            release_s=_timecents(g[G_RELEASE_VOL]),
            attenuation_db=max(0, g.get(G_ATTEN, 0)) / 10.0,
        )


if __name__ == "__main__":
    sf = SoundFont(sys.argv[1] if len(sys.argv) > 1 else "/usr/share/sounds/sf2/FluidR3_GM.sf2")
    for bank, prog, name, _ in sf.presets[:12]:
        s = sf.sample_for(bank, prog, 60)
        print(f"{bank:3d}:{prog:3d} {name:24s} {s.name:20s} {len(s.data):6d} @ {s.rate} root {s.root_key} "
              f"{s.cents:+.0f}c loop {s.loop_start}-{s.loop_end} A {s.attack_s:.3f} D {s.decay_s:.2f} "
              f"S -{s.sustain_db:.0f}dB R {s.release_s:.2f}")
