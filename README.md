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
                                                  \-> pc_audio_tx -> pino C3 (uart_tx)
                                                       -> USB-serial BL616 -> PC
```

- **`uart_rx` + `midi_parser`**: recebem MIDI 1.0 pela UART (8N1, 115 200
  baud por padrão; parâmetro `UART_BAUD` do topo) e
  produzem mensagens completas (com *running status*; mensagens de tempo real
  e SysEx são ignoradas; *Note On* com velocidade 0 vira *Note Off*).
- **`midi_mailbox`**: traduz as mensagens em comandos para o driver e os
  entrega pelas portas de comunicação da APU (ver [protocolo](#protocolo-mailbox)).
- **`apu_core`**: S-SMP/SPC700 + S-DSP + 64 KiB de A-RAM (derivado do NanoSPC),
  inicializado a partir de uma imagem `.spc` convertida em `.hex`.
- **Driver SPC700** (`src/driver/midi_driver.s`): lê os comandos, aloca as
  oito vozes (com *voice stealing*) e programa o S-DSP.
- **`i2s_tx`**: envia o PCM estéreo de 16 bits ao DAC em formato I²S
  (BCLK = 64·fs = 2.044 MHz, LRCLK = fs = 31.9375 KHz), sem MCLK.
- **`pc_audio_tx`** (parâmetro `PC_AUDIO`, ativo por padrão): devolve ao PC,
  pela mesma ponte USB-serial, o áudio da APU em mono ((L+R)/2), para ouvir e
  gravar sem o DAC. Por padrão (`PC_AUDIO_FORMAT = 1`) envia um byte mu-law
  (G.711) por amostra a fs/4 = 7984 Hz (`PC_AUDIO_DECIM = 4`), o que cabe em
  115 200 baud; `PC_AUDIO_FORMAT = 0` envia 14 bits por amostra na taxa
  completa, mas exige um enlace de pelo menos ~800 kbaud.

O núcleo roda a 24,528 MHz (PLL a partir dos 50 MHz da placa), enquanto a APU produz
uma amostra stereo a cada 768 ciclos (31.9375 KHz).

## Organização

```text
src/
  primer25k_nanospc_top.v   topo: PLL, reset, cadeia MIDI, APU, I²S e LED
  apu_core.v                S-SMP + S-DSP + A-RAM + carregador de imagem .spc
  i2s_tx.v                  transmissor I²S
  pc_audio_tx.v             fluxo de áudio mono para o PC pela UART
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
  midi_bridge.py            ponte teclado MIDI (PC) <-> UART da FPGA (MIDI e áudio)
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
- **Teste no hardware**: `pip install mido python-rtmidi pyserial numpy sounddevice`
  (`sounddevice` só é necessário para ouvir o áudio no PC).

## Simulação

Todos os alvos são executados a partir de `sim/`:

| Comando | O que verifica |
| --- | --- |
| `make test-i2s` | Transmissor I²S isolado: 2000 quadros bit a bit, períodos de BCLK/LRCLK, atraso de 1 BCLK do formato I²S. |
| `make test-midi` | UART + parser MIDI: casos dirigidos, baud ±2 %, erros de enquadramento e 300 000 bytes aleatórios contra um modelo de referência. |
| `make test-apu-midi` | Eventos MIDI → caixa de correio → driver SPC700 → S-DSP; analisa o áudio (afinação, acordes, velocidade, pan, troca de programa, volume, envelope, *voice stealing*, latência). |
| `make test-top-midi` | Integração da cadeia completa no topo: bytes MIDI no pino `midi_rx` até os pinos I²S, o fluxo de áudio em `uart_tx` e o LED, etapa por etapa (ver abaixo). |
| `make run` | Toca a imagem `src/data/test_spc.spc.hex` (uma trilha `.spc`) e grava `sim/output/primer25k_nanospc_smoke.wav`, conferindo que o áudio decodificado dos pinos I²S é idêntico ao PCM do S-DSP. |

`make test-top-midi` envia 157 bytes no estilo de um teclado real (*running
status*, *Note Off* como velocidade 0, SysEx, *active sensing*, *MIDI clock*)
e verifica:

1. os eventos decodificados pelo parser são exatamente os do roteiro;
2. a saída da APU é idêntica, amostra a amostra, a uma repetição dos mesmos
   eventos nos mesmos ciclos pelo caminho já verificado de `test-apu-midi`;
3. o áudio decodificado dos pinos I²S é idêntico à saída da APU, com BCLK e
   LRCLK nos períodos corretos; o fluxo em `uart_tx` é exatamente a
   codificação esperada (mu-law da mistura mono decimada, conferida contra
   uma implementação de referência do G.711) da saída da APU, sem erros de
   enquadramento; e o LED segue a sequência esperada;
4. as mesmas verificações musicais de `test-apu-midi`, medidas no áudio dos
   pinos I²S.

Os áudios gerados ficam em `sim/output/` (`apu_midi.wav`) e em
`sim/build_midi/` (`top_midi.wav`, `top_midi_i2s.wav`, `top_pc_audio.wav`, `ref.wav`) para
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

O nibble baixo (4 bits menos significativos) do opcode informa o canal MIDI.
Atualmente, o driver responde a todos os canais, e outras mensagens
(*pitch bend* e outros CCs) são ignoradas.

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
| `midi_rx` | B3 | RX da UART da FPGA, ligado à ponte USB-serial BL616 da dock (MIDI do PC) |
| `uart_tx` | C3 | TX da UART da FPGA, para a mesma ponte (áudio para o PC) |
| `i2s_bclk` | C11 | PMOD J4, pino 5 → BCK do PCM5102A |
| `i2s_lrclk` | B11 | PMOD J4, pino 7 → LCK |
| `i2s_sdata` | D11 | PMOD J4, pino 9 → DIN |
| +3V3 / GND | — | PMOD J4, pino 1 / pino 3 → VIN / GND do módulo |

### Módulo DAC PCM5102A

Conecte BCLK, LRCLK (LCK), DIN (dados), 3,3 V e GND à placa. Configure o módulo com
**SCK em GND** (PLL interno sem usar MCLK), **FMT em nível baixo** (I²S),
**XSMT em nível alto** (sem *mute*) e FLT/DEMP em nível baixo. 

### LED READY

O LED é controlado pelo clock de 50 MHz da placa (independente do PLL), então
também indica falhas de inicialização. Os códigos de piscadas se repetem a
cada ~2,7 s.

| Estado | Significado |
| --- | --- |
| apagado | reset inicial (~10 ms) ou FPGA sem bitstream |
| 1 piscada | o PLL (`dclk`, 24,528 MHz) não está rodando |
| 2 piscadas | `dclk` ok, mas a APU não iniciou (carga da imagem `.spc` travada) |
| 3 piscadas | erro na caixa de correio (driver não confirmou um comando ou a fila transbordou, por exemplo com MIDI corrompido por taxa de UART errada) ou áudio para o PC atrasado (`UART_BAUD` baixo demais para o formato de `PC_AUDIO`) |
| aceso fixo | APU e driver rodando, ainda sem áudio |
| piscando (~1 Hz) | áudio sendo gerado |

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

   Com Python do Miniconda/Anaconda, o `python-rtmidi` pode falhar com
   ``GLIBCXX_3.4.32' not found``: o conda carrega um `libstdc++` antigo antes
   do sistema. Atualize o do conda
   (`conda install -c conda-forge libstdcxx-ng`), use um ambiente com o Python
   do sistema (`/usr/bin/python3 -m venv ~/venv-midi`) ou rode o script com
   `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libstdc++.so.6 python3 tools/midi_bridge.py ...`.
3. Sem teclado, verifique a cadeia com a sequência de demonstração (escala e
   acorde em cada um dos quatro instrumentos). Com `--audio`, o som volta pela
   mesma porta e toca no PC, sem precisar do DAC; `--gravar` salva um WAV:

   ```bash
   python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --teste --audio
   python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --teste --gravar teste.wav
   ```

4. Com teclado:

   ```bash
   python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --midi CASIO --audio
   ```

   Com o DAC ligado, `--audio` é opcional: o áudio sai pelos pinos I²S e pela
   UART ao mesmo tempo.

**Taxa da UART**: MIDI (entrada) e áudio para o PC (saída) compartilham a
porta serial, então usam a mesma taxa, `UART_BAUD` = 115 200 baud por padrão
(o fluxo mu-law ocupa ~76 % dela). A ponte BL616 da dock não se mostrou
confiável a 921 600 baud: os dados chegam corrompidos e a ponte trava até a
placa ser reconectada. O script usa `--baud 115200 --formato ulaw` por padrão;
se o topo for sintetizado com outros parâmetros, passe os mesmos valores
(`--baud`, `--formato pcm14`, `--decim`). Para um receptor DIN-MIDI com
optoacoplador em um pino PMOD, use `UART_BAUD = 31250` e `PC_AUDIO = 0` em
`src/primer25k_nanospc_top.v`.

O áudio no PC é mono (pan não é audível) e, no formato padrão, tem qualidade
de telefone (mu-law, 7984 Hz): serve para conferir notas, acordes e timbres.
O som completo em estéreo de 16 bits a 31 937,5 Hz sai pelo DAC.

**Diagnóstico**: `--diag` pede à FPGA um pacote de estado (o byte `0xF9`,
ignorado pelo parser MIDI) e o mostra decodificado: checksum da A-RAM
calculado no boot (comparado com `src/data/midi_boot.spc.hex`, para confirmar
que a imagem foi carregada na BSRAM), PC de boot, atividade do SPC700 no
código do driver, escritas no DSP e em KON, contadores da caixa de correio e
pico do áudio. Com `--teste --diag`, o pacote vem depois da sequência:

```bash
python3 tools/midi_bridge.py --porta /dev/ttyUSB1 --teste --diag
```

**Keep-alive**: a ponte BL616 trava de vez (até reconectar a placa) se o
buffer de recepção dela (~7 KB) encher sem o PC estar lendo. Por isso a FPGA
só envia áudio enquanto recebe *Active Sensing* (`0xFE`, ignorado pelo
parser) em `midi_rx`: o script o manda a cada 100 ms apenas com `--audio` ou
`--gravar`, e a FPGA para o fluxo 300 ms depois do último `0xFE`
(`PC_AUDIO_KEEPALIVE_MS`). Notas MIDI comuns não ligam o fluxo.

**Se a ponte travar mesmo assim** (o script recebe 0 amostras), desconecte e
reconecte o USB-C da placa e carregue o driver de novo (ver abaixo).

**Driver serial depois de gravar**: o Gowin Programmer usa o chip FTDI da
placa e o `ftdi_sio` fica fora (a IDE costuma pedir que ele esteja na lista
negra, em `/etc/modprobe.d/ftdi_sio.conf`). Depois de gravar a placa:

```bash
sudo modprobe ftdi_sio     # cria /dev/ttyUSB0 (JTAG) e /dev/ttyUSB1 (UART da FPGA)
```

Use `/dev/ttyUSB1` no script. Antes de gravar de novo, se o Programmer não
achar o cabo, rode `sudo rmmod ftdi_sio`.

## Licença

O código RTL da APU do SNES é adaptado do projeto NanoSPC, sob a licença
GPL-3.0-only (ver `COPYING` e `THIRD_PARTY_NOTICES.md`).
