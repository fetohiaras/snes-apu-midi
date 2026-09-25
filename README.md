# Introdução

Projeto para a disciplina ENGG54 - Laboratório Integrado III, consistindo na
implementação de um sintetizador MIDI baseado no subsistema de
áudio (SPC700 e S-DSP) do SNES para a placa Sipeed Tang Primer
25K (`GW5A-LV25MG121NC1/I0, Version A`).

## Arquitetura e blocos funcionais

```text
Instrumento MIDI -> PC (tools/midi_bridge.py) -> USB-serial BL616 -> pino B3 (midi_rx)
  -> uart_rx -> midi_parser -> midi_mailbox -> portas $F4-$F7 da APU
  -> driver SPC700 (A-RAM) -> S-DSP (8 vozes BRR) -> i2s_tx -> PCM5102A -> saída P2
```

- **`uart_rx` + `midi_parser`**: recebem MIDI 1.0 a 31250 baud (8N1) e
  produzem mensagens completas (com *running status*; mensagens de tempo real
  e SysEx são ignoradas; *Note On* com velocidade 0 vira *Note Off*).
- **`midi_mailbox`**: traduz as mensagens em comandos para o driver e os
  entrega pelas portas de comunicação da APU (ver [protocolo](#protocolo-da-caixa-de-correio)).
- **`apu_core`**: S-SMP/SPC700 + S-DSP + 64 KiB de A-RAM (derivado do NanoSPC),
  inicializado a partir de uma imagem `.spc` convertida em `.hex`.
- **Driver SPC700** (`src/driver/midi_driver.s`): lê os comandos, aloca as
  oito vozes (com *voice stealing*) e programa o S-DSP.
- **`i2s_tx`**: envia o PCM estéreo de 16 bits ao DAC em formato I²S
  (BCLK = 64·fs = 2.044 MHz, LRCLK = fs = 31.9375 KHz), sem MCLK.

O núcleo roda a 24,528 MHz (PLL a partir dos 50 MHz da placa), enquanto a APU produz
uma amostra stereo a cada 768 ciclos (31.9375 KHz).

## Organização

```text
src/
  primer25k_nanospc_top.v   topo: PLL, reset, cadeia MIDI, APU, I²S e LED
  apu_core.v                S-SMP + S-DSP + A-RAM + carregador de imagem .spc
  i2s_tx.v                  transmissor I²S
  midi/                     uart_rx, midi_parser, midi_mailbox
  driver/midi_driver.s      driver SPC700 (sintaxe spcasm)
  spc_core/                 núcleo da APU (NanoSPC, GPL-3.0)
  gowin_pll/, pll_init.v    PLL gerado pela IDE Gowin
  data/                     imagens .hex carregadas na A-RAM (geradas)
sim/                        testbenches Verilator e verificações em Python
tools/
  soundbank.py              codificador BRR e banco de instrumentos
  build_midi_image.py       monta o driver + banco e gera a imagem de boot
  spc_to_hex.py             converte .spc para o .hex da A-RAM
  midi_bridge.py            ponte teclado MIDI (PC) -> UART da FPGA
docs/                       manuais do SNES, da placa, IPs Gowin e MIDI
primer25k_nanospc.gprj      projeto Gowin
primer25k_nanospc.cst/.sdc  pinos e restrições de tempo
PROPOSAL.md                 proposta e planejamento do projeto
THIRD_PARTY_NOTICES.md      licença e origem do código do NanoSPC
```

## Pré-requisitos

- **Simulação**: Verilator v5.020, `g++`, `make` e Python 3 com `numpy`.
- **Imagem de boot da APU**: [spcasm](https://codeberg.org/filmroellchen/spcasm)
  2.0.0 no `PATH`. Ele usa recursos *nightly* do Rust; com o Rust estável:

  ```bash
  RUSTC_BOOTSTRAP=1 cargo install --locked spcasm   # instala spcasm e brr em ~/.cargo/bin
  export PATH="$HOME/.cargo/bin:$PATH"
  ```

  Alternativa: `SPCASM=/caminho/para/spcasm make ...` em qualquer alvo que
  gere a imagem.
- **Síntese**: Gowin IDE (a versão Education 1.9.11 suporta o GW5A-25A). No
  Linux, se a IDE não abrir por conflito de bibliotecas, renomeie
  `IDE/lib/libfreetype.so.6` e `IDE/lib/libstdc++.so.6` (por exemplo, para
  `.bak`) e inicie com
  `cd IDE && LD_LIBRARY_PATH="$PWD/lib" ./bin/gw_ide`.
- **Teste com teclado**: `pip install mido python-rtmidi pyserial`.

## Simulação

Todos os alvos são executados a partir de `sim/`:

| Comando | O que verifica |
| --- | --- |
| `make test-i2s` | Transmissor I²S isolado: 2000 quadros bit a bit, períodos de BCLK/LRCLK, atraso de 1 BCLK do formato I²S. |
| `make test-midi` | UART + parser MIDI: casos dirigidos, baud ±2 %, erros de enquadramento e 300 000 bytes aleatórios contra um modelo de referência. |
| `make test-apu-midi` | Eventos MIDI → caixa de correio → driver SPC700 → S-DSP; analisa o áudio (afinação, acordes, velocidade, pan, troca de programa, volume, envelope, *voice stealing*, latência). |
| `make test-top-midi` | Integração da cadeia completa no topo: bytes MIDI no pino `midi_rx` até os pinos I²S e o LED, etapa por etapa (ver abaixo). |
| `make run` | Toca a imagem `src/data/test_spc.spc.hex` (uma trilha `.spc`) e grava `sim/output/primer25k_nanospc_smoke.wav`, conferindo que o áudio decodificado dos pinos I²S é idêntico ao PCM do S-DSP. |

`make test-top-midi` envia 157 bytes no estilo de um teclado real (*running
status*, *Note Off* como velocidade 0, SysEx, *active sensing*, *MIDI clock*)
e verifica:

1. os eventos decodificados pelo parser são exatamente os do roteiro;
2. a saída da APU é idêntica, amostra a amostra, a uma repetição dos mesmos
   eventos nos mesmos ciclos pelo caminho já verificado de `test-apu-midi`;
3. o áudio decodificado dos pinos I²S é idêntico à saída da APU, com BCLK e
   LRCLK nos períodos corretos, e o LED segue a sequência esperada;
4. as mesmas verificações musicais de `test-apu-midi`, medidas no áudio dos
   pinos I²S.

Os áudios gerados ficam em `sim/output/` (`apu_midi.wav`) e em
`sim/build_midi/` (`top_midi.wav`, `top_midi_i2s.wav`, `ref.wav`) para
inspeção. O roteiro MIDI e as expectativas estão em `sim/check_apu_midi.py`.

Para tocar outro arquivo `.spc` no teste de fumaça:

```bash
make spc-to-hex SPC="/caminho/do/arquivo.spc"
make run
```

## Driver e banco de instrumentos

A imagem de boot da APU (`src/data/midi_boot.spc.hex`) contém o driver
SPC700, a tabela de afinação, a tabela de instrumentos e as amostras BRR. Ela
é gerada por `tools/build_midi_image.py`:

- **Instrumentos**: edite `INSTRUMENTS` e `WAVES` em `tools/soundbank.py`
  (forma de onda, ADSR, transposição). O número de programa MIDI é tomado
  módulo o número de instrumentos, que deve ser potência de 2. Os
  instrumentos atuais (seno, quadrada, dente de serra, *pluck*) são
  provisórios.
- **Afinação**: calculada para a taxa real do S-DSP na placa (31 937,5 Hz).
- **Driver**: `src/driver/midi_driver.s`. Depois de qualquer alteração, rode
  `make test-apu-midi` e `make test-top-midi`.

### Protocolo mailbox

O módulo de conexão MIDI-APU escreve `opcode`, `arg0` e `arg1` nas portas 0–2 e um número de sequência na porta 3. 
O driver copia os argumentos quando a
sequência muda e a devolve na sua porta 3 como confirmação, que habilita o próximo comando.

| Comando (nibble alto do opcode) | Origem MIDI | arg0 | arg1 |
| --- | --- | --- | --- |
| 1 `NOTE_ON` | `9n nota vel` | nota | velocidade |
| 2 `NOTE_OFF` | `8n nota vel` / `9n nota 0` | nota | velocidade |
| 3 `PROGRAM` | `Cn prog` | programa | 0 |
| 4 `VOLUME` | `Bn 07 v` | valor | 0 |
| 5 `PAN` | `Bn 0A v` | valor | 0 |
| 6 `ALL_OFF` | `Bn 78 xx` / `Bn 7B xx` | 0 | 0 |

Os últimos 16 bits do opcode informam o canal MIDI
Atualmente, o driver responde a todos
os canais e ignora outras mensagens (*pitch bend* e outros CCs).

## Síntese e gravação

1. Gere a imagem de boot da APU (spcasm necessário):

   ```bash
   cd sim
   make midi-image        # gera src/data/midi_boot.spc.hex
   ```

2. Abra o projeto `primer25k_nanospc.gprj` na Gowin IDE.
 Em *Project → Configuration → Place & Route →
   Dual-Purpose Pin*, verifique que as opções *Use READY as regular IO* e *Use CPU as regular
   IO* estão ativadas.
3. Execute as etapas de *Synthesis* e *Place & Route*.
4. Grave o bitstream `.fs` no Gowin Programmer com as seguintes opções:
   - Access Mode: External Flash Mode 5A
   - Operation: exFlash Background Erase,Program,Verify 5A
   - Programming Options → File name: <`arquivo.fs`>
   - External Flash Options → Device: Generic Flash
   - External Flash Options → Start Address: 0x000000

### Pinos

| Sinal | Pino | Observação |
| --- | --- | --- |
| `clk_50m` | E2 | Oscilador de 50 MHz da placa |
| `led_ready` | E8 | LED READY (pino de uso duplo) |
| `midi_rx` | B3 | RX da UART da FPGA, ligado à ponte USB-serial BL616 da dock |
| `i2s_bclk`, `i2s_lrclk`, `i2s_sdata` | **a definir** | Escolha 3 pinos de um PMOD 3,3 V e descomente os `IO_LOC` em `primer25k_nanospc.cst`; senão, o *placer* escolhe pinos arbitrários |

### Módulo DAC PCM5102A

Conecte BCLK, LRCLK (LCK), DIN (dados), 3,3 V e GND à placa. Configure o módulo com
**SCK em GND** (PLL interno sem usar MCLK), **FMT em nível baixo** (I²S),
**XSMT em nível alto** (sem *mute*) e FLT/DEMP em nível baixo. 

### LED READY

| Estado | Significado |
| --- | --- |
| apagado desde a gravação | Reset inicial ou APU não iniciou |
| aceso fixo | APU e driver rodando, ainda sem áudio |
| piscando (~1 Hz) | Áudio sendo gerado |
| apaga e não volta a piscar | Erro no envio de mensagens (driver não confirmou um comando ou a fila de comandos transbordou) |

## Teste no hardware com um teclado MIDI

1. Conecte a placa ao PC pelo USB-C (debugger BL616) e o teclado ao PC por
   USB. 
2. Liste as entradas MIDI e as portas seriais:

   ```bash
   python3 tools/midi_bridge.py --listar
   ```

   O BL616 expõe duas interfaces, com a UART da FPGA estando normalmente na segunda
   (ex.: `/dev/ttyUSB1`). 
   Usuários Linux precisam estar no grupo `dialout`.
3. Sem teclado, verifique a cadeia com a sequência de demonstração (escala e
   acorde em cada um dos quatro instrumentos):

   ```bash
   python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --teste
   ```

4. Com teclado:

   ```bash
   python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --midi CASIO
   ```

**Taxa BAUD da UART**: ainda não foi confirmado se a ponte BL616 aceita 31 250 baud.
Se não aceitar, altere o parâmetro `MIDI_BAUD` em
`src/primer25k_nanospc_top.v` (por exemplo, para 115200), sintetize de novo e
use `--baud 115200` no script. Um receptor DIN-MIDI com optoacoplador em um
pino PMOD funciona com os 31 250 baud padrão.

## Licença

O código RTL da APU do SNES é adaptado do projeto NanoSPC, sob a licença
GPL-3.0-only (ver `COPYING` e `THIRD_PARTY_NOTICES.md`).
