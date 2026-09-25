#!/usr/bin/env python3
"""Ponte MIDI -> serial para o sintetizador na Tang Primer 25K.

Encaminha as mensagens de um teclado/controlador MIDI conectado ao PC para a
UART da FPGA (pino B3, via ponte USB-serial BL616 da dock), onde o parser
MIDI as recebe. Sem teclado, `--teste` toca uma sequência de demonstração.

Exemplos:
  python3 tools/midi_bridge.py --listar
  python3 tools/midi_bridge.py --porta /dev/ttyUSB1
  python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --midi CASIO
  python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --teste

Dependências: pip install mido python-rtmidi pyserial
"""

from __future__ import annotations

import argparse
import sys
import time


def _imports():
    try:
        import mido
        import serial
        import serial.tools.list_ports
    except ImportError as e:
        raise SystemExit(f"dependência ausente ({e.name}); instale com: pip install mido python-rtmidi pyserial")
    return mido, serial


def listar(mido, serial) -> None:
    print("Entradas MIDI:")
    for name in mido.get_input_names() or ["  (nenhuma)"]:
        print(f"  {name}")
    print("Portas seriais:")
    for p in serial.tools.list_ports.comports() or []:
        print(f"  {p.device}  {p.description}")


def tocar_teste(send) -> None:
    """Escala, acorde e troca de instrumentos (programas 0-3)."""
    def nota(n: int, dur: float, vel: int = 100) -> None:
        send([0x90, n, vel])
        time.sleep(dur)
        send([0x90, n, 0])

    send([0xB0, 7, 100])                      # volume
    send([0xB0, 10, 64])                      # pan central
    for prog in range(4):
        print(f"programa {prog}")
        send([0xC0, prog])
        for n in (60, 62, 64, 65, 67, 69, 71, 72):
            nota(n, 0.18)
        for n in (60, 64, 67):
            send([0x90, n, 100])
        time.sleep(0.8)
        send([0xB0, 123, 0])                  # all notes off
        time.sleep(0.3)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--porta", help="porta serial da FPGA (ex.: /dev/ttyUSB1, COM5)")
    ap.add_argument("--baud", type=int, default=31250,
                    help="taxa da UART; deve ser igual ao parâmetro MIDI_BAUD do topo (padrão: 31250)")
    ap.add_argument("--midi", help="parte do nome da entrada MIDI (padrão: a primeira encontrada)")
    ap.add_argument("--listar", action="store_true", help="lista entradas MIDI e portas seriais e sai")
    ap.add_argument("--teste", action="store_true", help="toca uma sequência de demonstração, sem teclado")
    ap.add_argument("-v", "--verbose", action="store_true", help="mostra cada mensagem enviada")
    args = ap.parse_args()

    mido, serial = _imports()
    if args.listar:
        listar(mido, serial)
        return 0
    if not args.porta:
        ap.error("informe --porta (use --listar para ver as portas)")

    ser = serial.Serial(args.porta, args.baud)

    def send(data: list[int]) -> None:
        ser.write(bytes(data))
        if args.verbose:
            print(" ".join(f"{b:02X}" for b in data))

    try:
        if args.teste:
            tocar_teste(send)
            return 0

        names = mido.get_input_names()
        if args.midi:
            names = [n for n in names if args.midi.lower() in n.lower()]
        if not names:
            raise SystemExit("nenhuma entrada MIDI encontrada (use --listar)")
        print(f"{names[0]} -> {args.porta} @ {args.baud} baud  (Ctrl+C para sair)")
        # mido descarta SysEx, clock e active sensing por padrão; o parser da
        # FPGA os ignoraria de qualquer forma.
        with mido.open_input(names[0]) as entrada:
            for msg in entrada:
                send(msg.bytes())
    except KeyboardInterrupt:
        pass
    finally:
        ser.write(bytes([0xB0, 123, 0]))       # não deixa notas presas
        ser.flush()
        ser.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
