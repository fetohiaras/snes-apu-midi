#!/usr/bin/env python3
"""Quick repository checks before opening the main design in Gowin."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parent.parent
EXPECTED_IMAGE_LINES = 0x10180


def check_hex(path: Path, required: bool) -> bool:
    if not path.exists():
        print(("ERROR" if required else "INFO") + f": missing {path.relative_to(ROOT)}")
        return not required
    lines = path.read_text(encoding="ascii").splitlines()
    ok = len(lines) == EXPECTED_IMAGE_LINES and all(re.fullmatch(r"[0-9a-fA-F]{2}", x) for x in lines)
    print(("OK" if ok else "ERROR") + f": {path.relative_to(ROOT)} ({len(lines)} lines)")
    return ok


def main() -> int:
    ok = True
    ok &= check_hex(ROOT / "src/data/core_smoke.spc.hex", True)
    main_image = ROOT / "src/data/midi_boot.spc.hex"
    if not main_image.exists():
        print("INFO: src/data/midi_boot.spc.hex is not built yet.")
        print("      Full synthesizer: cd sim && make midi-image  (requires spcasm)")
    else:
        ok &= check_hex(main_image, True)

    cst = (ROOT / "primer25k_nanospc.cst").read_text(encoding="utf-8")
    if re.search(r'^\s*IO_LOC\s+"i2s_bclk"', cst, re.M):
        print("OK: main I2S pins are assigned")
    else:
        print("INFO: main I2S pins are intentionally unassigned; do not load the full audio top")
        print("      on hardware until the DAC/PMOD pins are chosen. Diagnostic projects are safe to use.")

    for rel in [
        "primer25k_uart_smoketest.gprj",
        "primer25k_core_smoketest.gprj",
        "primer25k_nanospc.gprj",
    ]:
        exists = (ROOT / rel).exists()
        print(("OK" if exists else "ERROR") + f": {rel}")
        ok &= exists
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
