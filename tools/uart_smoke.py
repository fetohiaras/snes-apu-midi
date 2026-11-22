#!/usr/bin/env python3
"""Send slow bytes to the Tang Primer 25K onboard USB-UART for LED testing."""
import argparse
import time


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("port", help="Windows example: COM5")
    ap.add_argument("--baud", type=int, default=115200)
    ap.add_argument("--count", type=int, default=10)
    ap.add_argument("--interval", type=float, default=0.8)
    args = ap.parse_args()

    try:
        import serial
    except ImportError:
        raise SystemExit("pyserial is required: py -m pip install pyserial")

    with serial.Serial(args.port, args.baud, timeout=1) as ser:
        for n in range(args.count):
            value = (0x40 + n) & 0x7F
            ser.write(bytes([value]))
            ser.flush()
            print(f"sent 0x{value:02X}")
            time.sleep(args.interval)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
