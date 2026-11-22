#!/usr/bin/env python3
"""Generate a tiny redistributable SPC image for the FPGA core smoke test.

The SPC700 program at $0500 is only six bytes:

    mov a, $F7   ; read FPGA->SPC sequence byte
    mov $F7, a   ; echo it back as ACK
    bra $0500

This validates SPC parsing, CPU execution and the four APU communication ports
without needing spcasm, a MIDI device, or copyrighted SPC data.
"""
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "data" / "core_smoke.spc.hex"
IMAGE_BYTES = 0x10180
ENTRY = 0x0500


def build() -> bytearray:
    image = bytearray(IMAGE_BYTES)
    magic = b"SNES-SPC700 Sound File Data v0.30"
    image[:len(magic)] = magic
    image[0x21:0x23] = b"\x1a\x1a"
    image[0x23] = 27
    image[0x24] = 30
    image[0x25:0x27] = ENTRY.to_bytes(2, "little")
    image[0x27:0x2A] = b"\x00\x00\x00"  # A, X, Y
    image[0x2A] = 0x00                    # PSW: direct page = $0000
    image[0x2B] = 0xFF                    # SP

    # SPC700 opcodes verified against src/spc_core/spc700/mcode.v:
    # E4 dd = MOV A,d ; C4 dd = MOV d,A ; 2F rr = BRA rel8
    code = bytes([0xE4, 0xF7, 0xC4, 0xF7, 0x2F, 0xFA])
    image[0x100 + ENTRY:0x100 + ENTRY + len(code)] = code

    # Keep DSP safely reset/muted; this smoke test does not exercise audio.
    image[0x10100 + 0x6C] = 0xE0
    return image


def main() -> int:
    image = build()
    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text("".join(f"{b:02x}\n" for b in image), encoding="ascii", newline="\n")
    print(f"wrote {OUT} ({len(image)} bytes / lines)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
