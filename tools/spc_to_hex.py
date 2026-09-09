#!/usr/bin/env python3
"""Convert an SPC snapshot into the $readmemh image used by primer25k_nanospc.

The FPGA loader consumes only these standard SPC regions:
  0x00000..0x000ff  SPC header (CPU/SMP register state is read here)
  0x00100..0x100ff  64 KiB A-RAM (code, BRR samples, song data)
  0x10100..0x1017f  128 S-DSP registers

Normal SPC files are at least 0x10200 bytes and may contain additional
reserved bytes or XID6 metadata.  Those trailing bytes are intentionally not
part of the FPGA's test_aram memory and are ignored.
"""

from __future__ import annotations

import argparse
from pathlib import Path


IMAGE_BYTES = 0x10180
SPC_MAGIC = b"SNES-SPC700 Sound File Data"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path, help="input .spc snapshot")
    parser.add_argument("output", type=Path, help="output one-byte-per-line .hex image")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    source = args.input.read_bytes()

    if len(source) < IMAGE_BYTES:
        raise SystemExit(
            f"error: {args.input} is {len(source)} bytes; an SPC image needs at least "
            f"{IMAGE_BYTES} bytes"
        )
    if not source.startswith(SPC_MAGIC):
        raise SystemExit(
            f"error: {args.input} does not begin with the standard SNES-SPC700 signature"
        )

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", encoding="ascii", newline="\n") as output:
        output.writelines(f"{byte:02x}\n" for byte in source[:IMAGE_BYTES])

    extra = len(source) - IMAGE_BYTES
    print(f"Converted {args.input} -> {args.output}: {IMAGE_BYTES} bytes")
    if extra:
        print(f"Ignored {extra} trailing SPC/XID6/reserved bytes.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
