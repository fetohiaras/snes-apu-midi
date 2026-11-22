#!/usr/bin/env python3
"""Editor do banco de instrumentos do sintetizador (PyQt6).

Monta o banco de até 8 instrumentos a partir de um SoundFont (.sf2):
escolhe o preset, o orçamento de amostra (taxa, duração), o envelope
ADSR do S-DSP e o release por GAIN; ajusta o eco (atraso, realimentação,
volume e o FIR de 8 coeficientes). A prévia é offline (tools/sdsp_preview.py:
BRR, interpolação gaussiana e envelope como no S-DSP). "Gerar imagem"
grava soundfont/bank.json e src/data/midi_boot.spc.hex (precisa do spcasm).

  python3 tools/soundfont_gui.py [banco.json]
"""

from __future__ import annotations

import json
import os
import shutil
import sys
import traceback
from pathlib import Path

import numpy as np
from matplotlib.backends.backend_qtagg import FigureCanvasQTAgg
from matplotlib.figure import Figure
from PyQt6.QtCore import Qt
from PyQt6.QtWidgets import (
    QApplication, QComboBox, QDoubleSpinBox, QFileDialog, QFormLayout, QGridLayout, QGroupBox,
    QHBoxLayout, QLabel, QLineEdit, QListWidget, QMainWindow, QMessageBox, QProgressBar, QPushButton,
    QSlider, QSpinBox, QTabWidget, QVBoxLayout, QWidget,
)

sys.path.insert(0, str(Path(__file__).resolve().parent))
import build_midi_image as builder  # noqa: E402
import instrument as I  # noqa: E402
import sdsp_preview as P  # noqa: E402
from sf2 import SoundFont  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_BANK = ROOT / "soundfont" / "bank.json"
DEFAULT_SF2 = "/usr/share/sounds/sf2/FluidR3_GM.sf2"
A_RAM_SAMPLES = 0x10000 - builder.BRR_ADDR          # minus the echo buffer
RATES = [8000, 11025, 16000, 22050, 32000]
NOTE_NAMES = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]


def note_name(n: int) -> str:
    return f"{NOTE_NAMES[n % 12]}{n // 12 - 1}"


def ms(v: float) -> str:
    return "∞" if v == 0 else (f"{v / 1000:.1f} s" if v >= 1000 else f"{v:.0f} ms")


def fir_design(kind: str, cut: float) -> list[int]:
    """8-tap FIR (gain c/128). cut: fraction of fs/2."""
    n = np.arange(8) - 3.5
    if kind == "passa-tudo":
        return [127, 0, 0, 0, 0, 0, 0, 0]
    if kind == "passa-altas":                           # low-pass at 1 - cut, shifted to fs/2
        h = np.sinc((1 - cut) * n) * np.hamming(8)
        h = h / h.sum() * (-1.0) ** np.arange(8)
        c = np.round(h * 127).astype(int)
        return [int(v) for v in np.clip(c, -128, 127)]
    h = np.sinc(cut * n) * np.hamming(8)
    h /= h.sum()
    if kind == "passa-faixa":
        h = h * np.cos(np.pi * 0.5 * n) * 2             # centre at fs/4
    c = np.round(h * 127).astype(int)
    return [int(v) for v in np.clip(c, -128, 127)]


class Plot(FigureCanvasQTAgg):
    def __init__(self, h: float = 2.2):
        self.fig = Figure(figsize=(5, h), tight_layout=True)
        super().__init__(self.fig)
        self.ax = self.fig.add_subplot(111)


class Slider(QWidget):
    """Slider + label showing the value and what it means."""

    def __init__(self, lo: int, hi: int, fmt, on_change):
        super().__init__()
        self.fmt = fmt
        self.s = QSlider(Qt.Orientation.Horizontal)
        self.s.setRange(lo, hi)
        self.lab = QLabel()
        self.lab.setMinimumWidth(150)
        lay = QHBoxLayout(self)
        lay.setContentsMargins(0, 0, 0, 0)
        lay.addWidget(self.s, 1)
        lay.addWidget(self.lab)
        self.s.valueChanged.connect(lambda v: (self.lab.setText(self.fmt(v)), on_change()))
        self.lab.setText(self.fmt(lo))

    def value(self) -> int:
        return self.s.value()

    def set(self, v: int) -> None:
        self.s.blockSignals(True)
        self.s.setValue(v)
        self.s.blockSignals(False)
        self.lab.setText(self.fmt(v))


class Editor(QMainWindow):
    def __init__(self, bank_path: Path):
        super().__init__()
        self.setWindowTitle("Banco de instrumentos — SNES APU MIDI")
        self.fonts: dict[str, SoundFont] = {}
        self.cache: dict[tuple, tuple] = {}
        self.bank_path = bank_path
        self.bank = {"sf2": DEFAULT_SF2, "instruments": [], "echo": {
            "delay": 3, "feedback": 40, "volume": 35, "fir": [127, 0, 0, 0, 0, 0, 0, 0], "voices": 255}}
        self.loading = False
        self._build_ui()
        if bank_path.exists():
            self.load(bank_path)
        else:
            self.refresh_list()

    # ---------------------------------------------------------------- UI
    def _build_ui(self) -> None:
        central = QWidget()
        self.setCentralWidget(central)
        root = QVBoxLayout(central)

        top = QHBoxLayout()
        self.sf2_edit = QLineEdit(DEFAULT_SF2)
        b_sf2 = QPushButton("SoundFont…")
        b_sf2.clicked.connect(self.pick_sf2)
        b_open = QPushButton("Abrir banco…")
        b_open.clicked.connect(self.open_bank)
        b_save = QPushButton("Salvar banco")
        b_save.clicked.connect(self.save)
        b_gen = QPushButton("Gerar imagem")
        b_gen.setToolTip("salva o banco e grava src/data/midi_boot.spc.hex (spcasm); depois, síntese na Gowin")
        b_gen.clicked.connect(self.generate)
        for w in (QLabel("SF2:"), self.sf2_edit, b_sf2, b_open, b_save, b_gen):
            top.addWidget(w, 1 if w is self.sf2_edit else 0)
        root.addLayout(top)

        self.budget = QProgressBar()
        self.budget.setFormat("A-RAM: %v de %m bytes")
        root.addWidget(self.budget)

        tabs = QTabWidget()
        root.addWidget(tabs, 1)
        tabs.addTab(self._instrument_tab(), "Instrumentos")
        tabs.addTab(self._echo_tab(), "Eco (S-DSP)")
        self.status = QLabel()
        root.addWidget(self.status)

    def _instrument_tab(self) -> QWidget:
        w = QWidget()
        lay = QHBoxLayout(w)

        left = QVBoxLayout()
        self.list = QListWidget()
        self.list.currentRowChanged.connect(self.select)
        left.addWidget(QLabel("Instrumentos (programa MIDI mod 8)"))
        left.addWidget(self.list, 1)
        row = QHBoxLayout()
        for text, fn in (("+", self.add), ("−", self.remove), ("↑", lambda: self.move(-1)), ("↓", lambda: self.move(1))):
            b = QPushButton(text)
            b.clicked.connect(fn)
            row.addWidget(b)
        left.addLayout(row)
        lay.addLayout(left, 1)

        mid = QVBoxLayout()
        g = QGroupBox("Amostra")
        f = QFormLayout(g)
        self.preset = QComboBox()
        self.preset.setEditable(True)
        self.preset.setInsertPolicy(QComboBox.InsertPolicy.NoInsert)
        self.preset.activated.connect(self.edited_source)
        self.name = QLineEdit()
        self.name.editingFinished.connect(self.edited_env)
        self.key = QSpinBox()
        self.key.setRange(0, 127)
        self.key.valueChanged.connect(self.edited_source)
        self.rate = QComboBox()
        self.rate.addItems([str(r) for r in RATES])
        self.rate.currentIndexChanged.connect(self.edited_source)
        self.maxms = QSpinBox()
        self.maxms.setRange(50, 3000)
        self.maxms.setSingleStep(50)
        self.maxms.setSuffix(" ms")
        self.maxms.valueChanged.connect(self.edited_source)
        self.info = QLabel()
        f.addRow("Preset", self.preset)
        f.addRow("Nome", self.name)
        f.addRow("Tecla da zona", self.key)
        f.addRow("Taxa alvo (Hz)", self.rate)
        f.addRow("Duração máx.", self.maxms)
        f.addRow(self.info)
        mid.addWidget(g)

        g = QGroupBox("Envelope do S-DSP")
        f = QFormLayout(g)
        self.att = Slider(0, 15, lambda v: f"{v}: {ms(I.ATTACK_MS[v]) if v < 15 else 'imediato'}", self.edited_env)
        self.dec = Slider(0, 7, lambda v: f"{v}: {ms(I.DECAY_MS[v])}", self.edited_env)
        self.sl = Slider(0, 7, lambda v: f"{v}: {(v + 1) / 8:.3f} ({20 * np.log10((v + 1) / 8):.1f} dB)", self.edited_env)
        self.sr = Slider(0, 31, lambda v: f"{v}: {ms(I.SR_MS[v])}", self.edited_env)
        self.rel = Slider(0, 31, lambda v: "0: key-off (~8 ms)" if v == 0 else f"{v}: GAIN {ms(I.SR_MS[v])}", self.edited_env)
        f.addRow("Ataque (A)", self.att)
        f.addRow("Decaimento (D)", self.dec)
        f.addRow("Nível de sustentação (SL)", self.sl)
        f.addRow("Taxa de sustentação (SR)", self.sr)
        f.addRow("Release", self.rel)
        b = QPushButton("Envelope sugerido pelo SoundFont")
        b.clicked.connect(self.env_from_sf2)
        f.addRow(b)
        mid.addWidget(g)

        g = QGroupBox("Prévia (offline)")
        gl = QGridLayout(g)
        self.pnote = QSpinBox()
        self.pnote.setRange(24, 108)
        self.pnote.setValue(60)
        self.pnote.valueChanged.connect(lambda v: self.pnote.setSuffix(f"  ({note_name(v)})"))
        self.pnote.setSuffix("  (C4)")
        self.pvel = QSpinBox()
        self.pvel.setRange(1, 127)
        self.pvel.setValue(100)
        self.phold = QDoubleSpinBox()
        self.phold.setRange(0.05, 5)
        self.phold.setValue(1.0)
        self.phold.setSuffix(" s")
        self.pecho = QComboBox()
        self.pecho.addItems(["com eco", "sem eco"])
        for i, (lab, wdg) in enumerate((("Nota", self.pnote), ("Velocidade", self.pvel), ("Duração", self.phold), ("", self.pecho))):
            gl.addWidget(QLabel(lab), 0, 2 * i)
            gl.addWidget(wdg, 0, 2 * i + 1)
        for i, (text, kind) in enumerate((("Tocar nota", "note"), ("Acorde maior", "chord"), ("Escala", "scale"), ("Parar", "stop"))):
            b = QPushButton(text)
            b.clicked.connect(lambda _=False, k=kind: self.preview(k))
            gl.addWidget(b, 1, 2 * i, 1, 2)
        mid.addWidget(g)
        mid.addStretch(1)
        lay.addLayout(mid, 2)

        right = QVBoxLayout()
        self.wave_plot = Plot()
        self.env_plot = Plot()
        right.addWidget(QLabel("Amostra decodificada do BRR (loop em vermelho)"))
        right.addWidget(self.wave_plot)
        right.addWidget(QLabel("Envelope (nota de 1,5 s)"))
        right.addWidget(self.env_plot)
        lay.addLayout(right, 3)
        return w

    def _echo_tab(self) -> QWidget:
        w = QWidget()
        lay = QHBoxLayout(w)
        g = QGroupBox("Eco")
        f = QFormLayout(g)
        self.edl = Slider(0, 15, lambda v: f"{v}: {16 * v} ms, {2 * v} KB de A-RAM" if v else "0: desligado", self.edited_echo)
        self.efb = Slider(-128, 127, lambda v: f"{v} ({v / 128:+.2f})", self.edited_echo)
        self.evol = Slider(0, 127, lambda v: f"{v} ({v / 128:.2f}); CC91 altera ao vivo", self.edited_echo)
        f.addRow("Atraso (EDL)", self.edl)
        f.addRow("Realimentação (EFB)", self.efb)
        f.addRow("Volume do eco (EVOL)", self.evol)
        lay.addWidget(g, 1)

        g = QGroupBox("Filtro FIR do eco (8 coeficientes, ganho c/128)")
        v = QVBoxLayout(g)
        row = QHBoxLayout()
        self.fir_kind = QComboBox()
        self.fir_kind.addItems(["passa-tudo", "passa-baixas", "passa-altas", "passa-faixa"])
        self.fir_cut = QDoubleSpinBox()
        self.fir_cut.setRange(0.05, 0.95)
        self.fir_cut.setSingleStep(0.05)
        self.fir_cut.setValue(0.3)
        self.fir_cut.setPrefix("corte ")
        self.fir_cut.setSuffix(" × fs/2")
        b = QPushButton("Projetar")
        b.clicked.connect(self.design_fir)
        for x in (self.fir_kind, self.fir_cut, b):
            row.addWidget(x)
        v.addLayout(row)
        row = QHBoxLayout()
        self.fir = []
        for i in range(8):
            s = QSpinBox()
            s.setRange(-128, 127)
            s.setPrefix(f"C{i} ")
            s.valueChanged.connect(self.edited_echo)
            self.fir.append(s)
            row.addWidget(s)
        v.addLayout(row)
        self.fir_plot = Plot(2.6)
        v.addWidget(self.fir_plot)
        self.fir_warn = QLabel()
        v.addWidget(self.fir_warn)
        lay.addWidget(g, 2)
        return w

    # ------------------------------------------------------------ data
    def font(self, path: str) -> SoundFont:
        if path not in self.fonts:
            self.fonts[path] = SoundFont(path)
        return self.fonts[path]

    def prepared(self, d: dict):
        key = (self.bank["sf2"], d.get("bank", 0), d["program"], d.get("key", 60), d.get("target_rate", 16000), d.get("max_ms", 600))
        if key not in self.cache:
            s = self.font(self.bank["sf2"]).sample_for(d.get("bank", 0), d["program"], d.get("key", 60))
            p = I.prepare(s, d.get("target_rate", 16000), d.get("max_ms", 600))
            self.cache[key] = (s, p, I.snr_db(p))
        return self.cache[key]

    def cur(self) -> dict | None:
        i = self.list.currentRow()
        return self.bank["instruments"][i] if 0 <= i < len(self.bank["instruments"]) else None

    def refresh_list(self) -> None:
        row = self.list.currentRow()
        self.list.blockSignals(True)
        self.list.clear()
        for i, d in enumerate(self.bank["instruments"]):
            self.list.addItem(f"{i}: {d.get('name', '?')}  (GM {d['program']})")
        self.list.blockSignals(False)
        if self.bank["instruments"]:
            self.list.setCurrentRow(max(0, min(row, len(self.bank["instruments"]) - 1)))
        self.update_budget()

    def update_budget(self) -> None:
        used = 0
        for d in self.bank["instruments"]:
            try:
                used += self.prepared(d)[1].size
            except Exception:
                pass
        e = self.bank["echo"]
        avail = A_RAM_SAMPLES - (e["delay"] * 2048 if e["delay"] else 256)
        self.budget.setMaximum(avail)
        self.budget.setValue(min(used, avail))
        over = used > avail
        self.budget.setStyleSheet("QProgressBar::chunk{background:%s}" % ("#c33" if over else "#3a3"))
        self.budget.setFormat(f"A-RAM para amostras: {used} de {avail} bytes" + ("  — EXCEDIDO" if over else ""))

    # --------------------------------------------------------- actions
    def pick_sf2(self) -> None:
        p, _ = QFileDialog.getOpenFileName(self, "SoundFont", str(Path(self.bank["sf2"]).parent), "SoundFont (*.sf2)")
        if p:
            self.bank["sf2"] = p
            self.sf2_edit.setText(p)
            self.fill_presets()
            self.cache.clear()
            self.refresh_list()

    def fill_presets(self) -> None:
        self.preset.blockSignals(True)
        self.preset.clear()
        for bank, prog, name, _ in self.font(self.bank["sf2"]).presets:
            self.preset.addItem(f"{bank:03d}:{prog:03d} {name}", (bank, prog))
        self.preset.blockSignals(False)

    def add(self) -> None:
        if len(self.bank["instruments"]) >= builder.MAX_INSTR:
            QMessageBox.information(self, "Banco", f"no máximo {builder.MAX_INSTR} instrumentos")
            return
        d = {"name": "Grand Piano", "bank": 0, "program": 0, "key": 60, "target_rate": 16000, "max_ms": 500}
        s = self.font(self.bank["sf2"]).sample_for(0, 0, 60)
        d["env"] = I.adsr_from_sf2(s)
        self.bank["instruments"].append(d)
        self.refresh_list()
        self.list.setCurrentRow(len(self.bank["instruments"]) - 1)

    def remove(self) -> None:
        i = self.list.currentRow()
        if 0 <= i < len(self.bank["instruments"]):
            del self.bank["instruments"][i]
            self.refresh_list()

    def move(self, step: int) -> None:
        i = self.list.currentRow()
        j = i + step
        ins = self.bank["instruments"]
        if 0 <= i < len(ins) and 0 <= j < len(ins):
            ins[i], ins[j] = ins[j], ins[i]
            self.refresh_list()
            self.list.setCurrentRow(j)

    def select(self, _row: int) -> None:
        d = self.cur()
        if d is None:
            return
        self.loading = True
        if self.preset.count() == 0:
            self.fill_presets()
        idx = self.preset.findData((d.get("bank", 0), d["program"]))
        self.preset.setCurrentIndex(max(0, idx))
        self.name.setText(d.get("name", ""))
        self.key.setValue(d.get("key", 60))
        self.rate.setCurrentText(str(d.get("target_rate", 16000)))
        self.maxms.setValue(d.get("max_ms", 600))
        env = d.get("env") or I.adsr_from_sf2(self.prepared(d)[0])
        d["env"] = env
        self.att.set(env["attack"])
        self.dec.set(env["decay"])
        self.sl.set(env["sustain_level"])
        self.sr.set(env["sustain_rate"])
        self.rel.set(env.get("release", 0))
        self.loading = False
        self.redraw()

    def edited_source(self, *_):
        d = self.cur()
        if d is None or self.loading:
            return
        bank, prog = self.preset.currentData() or (d.get("bank", 0), d["program"])
        changed = (bank, prog) != (d.get("bank", 0), d["program"])
        d.update(bank=bank, program=prog, key=self.key.value(), target_rate=int(self.rate.currentText()),
                 max_ms=self.maxms.value())
        if changed:
            d["name"] = self.preset.currentText()[8:]
            d["env"] = I.adsr_from_sf2(self.font(self.bank["sf2"]).sample_for(bank, prog, d["key"]))
            self.refresh_list()
            self.select(self.list.currentRow())
            return
        self.update_budget()
        self.redraw()

    def edited_env(self, *_):
        d = self.cur()
        if d is None or self.loading:
            return
        d["name"] = self.name.text() or d.get("name", "")
        d["env"] = {"attack": self.att.value(), "decay": self.dec.value(), "sustain_level": self.sl.value(),
                    "sustain_rate": self.sr.value(), "release": self.rel.value()}
        item = self.list.currentItem()
        if item:
            item.setText(f"{self.list.currentRow()}: {d['name']}  (GM {d['program']})")
        self.redraw(wave=False)

    def env_from_sf2(self) -> None:
        d = self.cur()
        if d:
            d["env"] = I.adsr_from_sf2(self.prepared(d)[0])
            self.select(self.list.currentRow())

    def design_fir(self) -> None:
        c = fir_design(self.fir_kind.currentText(), self.fir_cut.value())
        for s, v in zip(self.fir, c):
            s.blockSignals(True)
            s.setValue(v)
            s.blockSignals(False)
        self.edited_echo()

    def edited_echo(self, *_):
        if self.loading:
            return
        e = self.bank["echo"]
        e.update(delay=self.edl.value(), feedback=self.efb.value(), volume=self.evol.value(),
                 fir=[s.value() for s in self.fir], voices=255)
        f, db = P.fir_response(e["fir"])
        ax = self.fir_plot.ax
        ax.clear()
        ax.plot(f / 1000, db)
        ax.set_xlabel("kHz")
        ax.set_ylabel("dB")
        ax.set_ylim(-40, 12)
        ax.grid(alpha=0.3)
        self.fir_plot.draw_idle()
        loop_gain = 10 ** (db.max() / 20) * abs(e["feedback"]) / 128
        self.fir_warn.setText(f"ganho máximo do FIR {db.max():+.1f} dB; ganho da malha de realimentação "
                              f"{loop_gain:.2f}" + ("  — INSTÁVEL (≥ 1): o eco cresce até saturar" if loop_gain >= 1 else ""))
        self.update_budget()

    def redraw(self, wave: bool = True) -> None:
        d = self.cur()
        if d is None:
            return
        try:
            s, p, snr = self.prepared(d)
        except Exception as ex:
            self.info.setText(f"erro: {ex}")
            return
        self.info.setText(f"{s.name}: {len(s.data)} amostras @ {s.rate} Hz, raiz {note_name(s.root_key)}\n"
                          f"BRR: {p.size} bytes, {p.rate:.0f} Hz, SNR {snr:.1f} dB, {p.loop_note}")
        if wave:
            ax = self.wave_plot.ax
            ax.clear()
            dec = np.array(I.brr_decode(p.brr), float)
            t = np.arange(len(dec)) / p.rate
            ax.plot(t, dec / 32768, lw=0.5)
            if p.loop_block is not None:
                ax.axvspan(p.loop_block * 16 / p.rate, t[-1], color="r", alpha=0.15)
            ax.set_xlabel("s")
            ax.set_ylim(-1, 1)
            self.wave_plot.draw_idle()
        a1, a2, rel = I.adsr_regs(d["env"])
        tt, ee = P.envelope_curve(a1, a2, rel)
        ax = self.env_plot.ax
        ax.clear()
        ax.plot(tt, ee)
        ax.axvline(1.5, color="k", ls=":", lw=0.8)
        ax.set_xlabel("s")
        ax.set_ylim(0, 1.05)
        self.env_plot.draw_idle()

    def preview(self, kind: str) -> None:
        import sounddevice as sd
        sd.stop()
        d = self.cur()
        if kind == "stop" or d is None:
            return
        _s, p, _snr = self.prepared(d)
        a1, a2, rel = I.adsr_regs(d["env"])
        inst = dict(brr=p.brr, loop_block=p.loop_block, pitch=I.pitch_table(p), adsr1=a1, adsr2=a2, release=rel)
        n, v, h = self.pnote.value(), self.pvel.value(), self.phold.value()
        if kind == "note":
            notes = [(n, v, 0.0, h)]
        elif kind == "chord":
            notes = [(n + k, v, 0.0, h) for k in (0, 4, 7)]
        else:
            notes = [(n + k, v, i * h / 2, h / 2) for i, k in enumerate((0, 2, 4, 5, 7, 9, 11, 12))]
        end = max(s + hd for _, _, s, hd in notes) + 1.0 + 0.016 * self.bank["echo"]["delay"] * 4
        echo_cfg = self.bank["echo"] if self.pecho.currentIndex() == 0 else None
        self.status.setText("calculando a prévia…")
        QApplication.processEvents()
        y = P.render(inst, notes, end, echo_cfg)
        self.status.setText(f"prévia: {len(y) / P.DSP_RATE:.1f} s, pico {np.max(np.abs(y)):.2f}")
        try:
            sd.play(y.astype(np.float32), 32000)
        except Exception:           # placa sem 32 kHz: reamostra para 48 kHz
            t48 = np.arange(int(len(y) * 48000 / 32000)) * (32000 / 48000)
            sd.play(np.interp(t48, np.arange(len(y)), y).astype(np.float32), 48000)

    # ------------------------------------------------------------- files
    def load(self, path: Path) -> None:
        self.bank = json.loads(path.read_text())
        self.bank.setdefault("echo", {"delay": 0, "feedback": 0, "volume": 0, "fir": [127] + [0] * 7, "voices": 255})
        sf2 = self.bank.get("sf2", DEFAULT_SF2)
        if not Path(sf2).is_absolute():
            sf2 = str((path.parent / sf2).resolve())
        self.bank["sf2"] = sf2
        self.bank_path = path
        self.sf2_edit.setText(sf2)
        self.fill_presets()
        e = self.bank["echo"]
        self.loading = True
        self.edl.set(e["delay"])
        self.efb.set(e["feedback"])
        self.evol.set(e["volume"])
        for s, c in zip(self.fir, e["fir"]):
            s.setValue(c)
        self.loading = False
        self.edited_echo()
        self.refresh_list()
        self.status.setText(f"banco {path}")

    def open_bank(self) -> None:
        p, _ = QFileDialog.getOpenFileName(self, "Banco", str(self.bank_path.parent), "Banco (*.json)")
        if p:
            self.load(Path(p))

    def save(self) -> bool:
        self.bank_path.parent.mkdir(parents=True, exist_ok=True)
        self.bank_path.write_text(json.dumps(self.bank, indent=2, ensure_ascii=False) + "\n")
        self.status.setText(f"salvo em {self.bank_path}")
        return True

    def generate(self) -> None:
        self.save()
        spcasm = os.environ.get("SPCASM") or shutil.which("spcasm") or str(Path.home() / ".cargo/bin/spcasm")
        try:
            self.status.setText("gerando a imagem…")
            QApplication.processEvents()
            hexfile = builder.build(ROOT / "sim" / "build_sf", self.bank_path, spcasm=spcasm)
            shutil.copy(hexfile, ROOT / "src" / "data" / "midi_boot.spc.hex")
            self.status.setText("imagem gravada em src/data/midi_boot.spc.hex — rode a síntese na Gowin e grave a placa")
        except SystemExit as ex:
            QMessageBox.warning(self, "Gerar imagem", str(ex))
        except Exception:
            QMessageBox.warning(self, "Gerar imagem", traceback.format_exc())


def main() -> int:
    app = QApplication(sys.argv)
    ed = Editor(Path(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT_BANK)
    ed.resize(1400, 860)
    ed.show()
    return app.exec()


if __name__ == "__main__":
    raise SystemExit(main())
