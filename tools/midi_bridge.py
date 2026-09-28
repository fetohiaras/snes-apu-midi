#!/usr/bin/env python3
"""Ponte MIDI <-> serial para o sintetizador na Tang Primer 25K.

Encaminha as mensagens de um teclado/controlador MIDI conectado ao PC para a
UART da FPGA (pino B3, via ponte USB-serial BL616 da dock). Com o parâmetro
PC_AUDIO do topo ativo, a FPGA devolve pela mesma porta (pino C3) o áudio da
APU em mono (por padrão mu-law de 8 bits a 7984 Hz, 115200 baud), que o
script pode tocar (`--audio`) e/ou gravar (`--gravar`).
Sem teclado, `--teste` toca uma sequência de demonstração.

Exemplos:
  python3 tools/midi_bridge.py --listar
  python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --teste --audio
  python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --midi CASIO --audio
  python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --teste --gravar teste.wav
  python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --diag
  python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --teste --diag

Dependências: pip install mido python-rtmidi pyserial numpy sounddevice
(sounddevice só é necessário com --audio).
"""

from __future__ import annotations

import argparse
import sys
import threading
import time
import wave

TAXA_APU = 24_528_000 / 768          # taxa de amostragem da APU na placa (Hz)


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
    portas = [p for p in serial.tools.list_ports.comports() if p.description != "n/a"]  # sem ttyS* vazias
    for p in portas:
        print(f"  {p.device}  {p.description}")
    if not portas:
        print("  (nenhuma; a placa está conectada pelo USB-C?)")


def decodificar_pcm14(dados: bytes, sobra: bytes = b""):
    """Formato PCM14: cada amostra de 14 bits chega em dois bytes, `1 m[15:9]`
    e `0 m[8:2]`; o bit 7 permite ressincronizar se um byte se perder.
    Retorna (amostras int16, bytes que sobram para a próxima leitura, erros)."""
    import numpy as np
    b = np.frombuffer(sobra + dados, dtype=np.uint8)
    if len(b) < 2:
        return np.zeros(0, np.int16), bytes(b), 0
    alto = b[:-1] >= 0x80
    pares = np.nonzero(alto & (b[1:] < 0x80))[0]
    erros = int(np.count_nonzero(alto[1:] & alto[:-1]))
    v = ((b[pares].astype(np.int32) & 0x7F) << 7) | b[pares + 1]
    v = ((v ^ 0x2000) - 0x2000) * 4
    resto = bytes(b[-1:]) if b[-1] >= 0x80 else b""
    return v.astype(np.int16), resto, erros


def decodificar_ulaw(dados: bytes):
    """Formato ULAW8: um byte mu-law G.711 por amostra (ulaw2linear do g711.c)."""
    import numpy as np
    u = (~np.frombuffer(dados, dtype=np.uint8).astype(np.int32)) & 0xFF
    t = (((u & 0x0F) << 3) + 0x84) << ((u & 0x70) >> 4)
    return np.where(u & 0x80, 0x84 - t, t - 0x84).astype(np.int16)


def decodificar_diag(pkt: bytes) -> dict | None:
    """Pacote de diagnóstico de 22 bytes da FPGA (ver primer25k_nanospc_top.v)."""
    if len(pkt) != 22 or pkt[:2] != b"\xa5\x5a":
        return None
    x = 0
    for b in pkt[:21]:
        x ^= b
    u16 = lambda i: pkt[i] | (pkt[i + 1] << 8)
    f = pkt[8]
    return {
        "ok": x == pkt[21],
        "soma_a": u16(2), "soma_b": u16(4), "pc_boot": u16(6),
        "flags": f, "soma_ok": bool(f & 0x80), "rodando": bool(f & 0x40), "audio": bool(f & 0x20),
        "descartados": bool(f & 0x08), "timeout": bool(f & 0x04), "overrun": bool(f & 0x02),
        "ocupado": bool(f & 0x01), "erros": bool(f & 0x0E),
        "enviados": u16(9), "porta3": pkt[11], "smp_a": u16(12), "ciclos_05": u16(14),
        "escritas_dsp": u16(16), "kon": pkt[18], "pico": u16(19),
    }


def checksum_imagem(caminho: str) -> tuple[int, int] | None:
    try:
        texto = open(caminho).read().split()
    except OSError:
        return None
    a = b = 0
    for h in texto:
        a = (a + int(h, 16)) & 0xFFFF
        b = (b + a) & 0xFFFF
    return a, b


def diagnostico(ser, send) -> None:
    time.sleep(0.5)                         # deixa o fluxo de áudio parar (keep-alive)
    ser.reset_input_buffer()
    send([0xF9])
    dados = b""
    fim = time.time() + 1.0
    while time.time() < fim:
        dados += ser.read(4096)
        i = dados.find(b"\xa5\x5a")
        if i >= 0 and len(dados) >= i + 22:
            break
    i = dados.find(b"\xa5\x5a")
    d = decodificar_diag(dados[i:i + 22]) if i >= 0 else None
    if d is None:
        extra = ""
        if dados and dados.count(0xFF) > len(dados) // 2:
            extra = ("\n  (só silêncio mu-law: a ponte BL616 provavelmente travou com o buffer cheio;"
                     " reconecte a placa)")
        print(f"diagnóstico: nenhum pacote ({len(dados)} bytes recebidos: {dados[:24].hex(' ')}){extra}")
        return
    if i > 0:
        print(f"(ignorados {i} bytes antes do pacote)")
    ref = checksum_imagem("src/data/midi_boot.spc.hex")
    sim = lambda v: "sim" if v else "NÃO"
    print("diagnóstico da FPGA" + ("" if d["ok"] else " (XOR do pacote INVÁLIDO)"))
    print(f"  checksum da A-RAM no boot: {d['soma_a']:04X}/{d['soma_b']:04X}"
          + (f", arquivo {ref[0]:04X}/{ref[1]:04X} -> {'OK' if ref == (d['soma_a'], d['soma_b']) else 'DIFERENTE: imagem não carregada na BSRAM'}"
             if ref else " (src/data/midi_boot.spc.hex não encontrado para comparar)"))
    print(f"  PC de boot: ${d['pc_boot']:04X} (esperado $0500)   APU rodando: {sim(d['rodando'])}")
    print(f"  SPC700 agora em ${d['smp_a']:04X}; ciclos na página $05 (driver): {d['ciclos_05']}"
          + ("  <- driver não está executando" if d["ciclos_05"] == 0 else ""))
    print(f"  escritas no DSP: {d['escritas_dsp']}, das quais KON: {d['kon']}")
    print(f"  caixa de correio: {d['enviados']} comandos confirmados, porta 3 = {d['porta3']}, "
          f"descartados: {sim(d['descartados'])}, timeout: {sim(d['timeout'])}, ocupada: {sim(d['ocupado'])}")
    print(f"  pico do áudio: {d['pico']}   áudio visto: {sim(d['audio'])}   overrun do áudio: {sim(d['overrun'])}")


class ReceptorAudio(threading.Thread):
    """Lê o fluxo de áudio da FPGA (ver src/pc_audio_tx.v) e o toca e/ou grava."""

    def __init__(self, ser, tocar: bool, arquivo: str | None, formato: str, decim: int, ganho_db: float = 0.0):
        super().__init__(daemon=True)
        import numpy as np
        self.np = np
        self.ser = ser
        self.formato = formato
        self.ganho = 10 ** (ganho_db / 20)
        self.taxa = TAXA_APU / (decim if formato == "ulaw" else 1)
        self.parar = threading.Event()
        self.amostras = 0
        self.erros_sync = 0
        self.stream = None
        self.wav = None
        if arquivo:
            self.wav = wave.open(arquivo, "wb")
            self.wav.setnchannels(1)
            self.wav.setsampwidth(2)
            self.wav.setframerate(round(self.taxa))
        if tocar:
            try:
                import sounddevice as sd
            except ImportError:
                raise SystemExit("para --audio instale: pip install sounddevice numpy")
            for taxa in (round(self.taxa), 48000, 44100):
                try:
                    self.stream = sd.OutputStream(samplerate=taxa, channels=1, dtype="int16", latency="low")
                    break
                except Exception:
                    continue
            if self.stream is None:
                raise SystemExit("a placa de som não aceitou nenhuma taxa de amostragem testada")
            self.fator = self.stream.samplerate / self.taxa
            print(f"áudio: {self.formato}, {self.taxa:.0f} Hz da FPGA -> {self.stream.samplerate:.0f} Hz na placa de som")
            self.stream.start()
            self.stream.write(np.zeros(int(0.05 * self.stream.samplerate), dtype=np.int16))  # ~50 ms de folga

    def run(self) -> None:
        np = self.np
        sobra = b""
        while not self.parar.is_set():
            dados = self.ser.read(4096)
            if not dados:
                continue
            if self.formato == "ulaw":
                x = decodificar_ulaw(dados)
            else:
                x, sobra, erros = decodificar_pcm14(dados, sobra)
                self.erros_sync += erros
            self.amostras += len(x)
            if self.ganho != 1.0:
                x = np.clip(x.astype(np.float32) * self.ganho, -32768, 32767).astype(np.int16)
            if self.wav:
                self.wav.writeframes(x.tobytes())
            if self.stream is not None and len(x):
                if self.fator != 1.0:
                    n = max(1, round(len(x) * self.fator))
                    x = np.interp(np.linspace(0, len(x) - 1, n), np.arange(len(x)), x).astype(np.int16)
                self.stream.write(x)

    def fechar(self) -> None:
        self.parar.set()
        self.join(timeout=1)
        if self.wav:
            self.wav.close()
        if self.stream is not None:
            self.stream.stop()
            self.stream.close()
        extra = f", erros de sincronismo: {self.erros_sync}" if self.formato == "pcm14" else ""
        print(f"áudio recebido: {self.amostras} amostras ({self.amostras / self.taxa:.1f} s){extra}")


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
    ap.add_argument("--baud", type=int, default=115200,
                    help="taxa da UART; deve ser igual ao parâmetro UART_BAUD do topo (padrão: 115200)")
    ap.add_argument("--formato", choices=("ulaw", "pcm14"), default="ulaw",
                    help="formato do áudio da FPGA, parâmetro PC_AUDIO_FORMAT do topo: 1 = ulaw, 0 = pcm14 (padrão: ulaw)")
    ap.add_argument("--decim", type=int, default=4,
                    help="PC_AUDIO_DECIM do topo, usado com ulaw (padrão: 4 -> 7984 Hz)")
    ap.add_argument("--midi", help="parte do nome da entrada MIDI (padrão: a primeira encontrada)")
    ap.add_argument("--listar", action="store_true", help="lista entradas MIDI e portas seriais e sai")
    ap.add_argument("--teste", action="store_true", help="toca uma sequência de demonstração, sem teclado")
    ap.add_argument("--audio", action="store_true", help="toca no PC o áudio devolvido pela FPGA (PC_AUDIO = 1)")
    ap.add_argument("--gravar", metavar="ARQUIVO.wav", help="grava o áudio devolvido pela FPGA")
    ap.add_argument("--ganho", type=float, default=0.0, metavar="DB",
                    help="ganho extra em dB no áudio do PC (reprodução e gravação), com saturação (padrão: 0)")
    ap.add_argument("--diag", action="store_true",
                    help="pede e mostra o pacote de diagnóstico da FPGA (depois de --teste, se usado)")
    ap.add_argument("-v", "--verbose", action="store_true", help="mostra cada mensagem enviada")
    args = ap.parse_args()

    mido, serial = _imports()
    if args.listar:
        listar(mido, serial)
        return 0
    if not args.porta:
        ap.error("informe --porta (use --listar para ver as portas)")

    ser = serial.Serial(args.porta, args.baud, timeout=0.05)
    ser.reset_input_buffer()
    lock = threading.Lock()

    def send(data: list[int]) -> None:
        with lock:
            ser.write(bytes(data))
        if args.verbose and data != [0xFE]:
            print(" ".join(f"{b:02X}" for b in data))

    receptor = None
    if args.audio or args.gravar:
        receptor = ReceptorAudio(ser, args.audio, args.gravar, args.formato, args.decim, args.ganho)
        receptor.start()

        # A FPGA só envia áudio enquanto recebe Active Sensing (0xFE, ignorado
        # pelo parser MIDI; PC_AUDIO_KEEPALIVE_MS): a ponte BL616 trava se o
        # buffer dela encher sem ninguém lendo, então só pedimos áudio aqui.
        def manter_ativo() -> None:
            while not receptor.parar.is_set():
                send([0xFE])
                time.sleep(0.1)
        threading.Thread(target=manter_ativo, daemon=True).start()

    try:
        if args.teste:
            tocar_teste(send)
            time.sleep(0.5)
        if args.diag:
            if receptor:
                receptor.parar.set()            # para o keep-alive antes do pedido
                receptor.join(timeout=1)
            diagnostico(ser, send)
        if args.teste or args.diag:
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
        send([0xB0, 123, 0])                   # não deixa notas presas
        if receptor:
            time.sleep(0.2)
            receptor.fechar()
        ser.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
