# Introdução

Projeto para a disciplina ENGG54 - Laboratório Integrado III, consistindo na
implementação de um sintetizador MIDI baseado no subsistema de 
áudio (SPC700 e S-DSP) do SNES para a placa Sipeed Tang Primer
25K (`GW5A-LV25MG121NC1/I0`).

## Alvo de síntese

O alvo de síntese atual carrega um arquivo .SPC na memória da APU, roda o 
núcleo a 24.528MHz, e usa o LED READY como indicador de funcionamento.
- Dispositivo: GW5A-LV25MG121NC1/I0, device version A


## Organização

- `src/`: RTL, blocos IP Gowin e imagens para a memória dos processadores.
- `sim/`: Testbenches em Verilator.
- `tools/`: Scripts de conversão .SPC para .HEX.
- `primer25k_nanospc.cst`, `.sdc`, `.gprj`: Arquivos de configuração da placa-alvo.
- `PROPOSAL.md`: Proposta e planejamento do projeto.
- `THIRD_PARTY_NOTICES.md`: Licensa do projeto NanoSPC.


## Simulação

O sistema atual contém um testbench que permite a geração de um arquivo de áudio .WAV
a partir de um arquivo .SPC para SNES convertido em .HEX.

```bash
cd sim
make run
```

O arquivo .WAV é escrito em `sim/output/`.

Outros arquivos `.spc` podem ser usados após conversão:

```bash
# conversão
make spc-to-hex SPC="/caminho/do/arquivo.spc"
make run
```

## Compilação e programação

Open `primer25k_nanospc.gprj` with Gowin FPGA Designer Education, select the
recorded target device, run Synthesis and Place & Route, then program the
generated `.fs` image in external-flash mode.  The clock input is E2 (50 MHz)
and the READY LED is E8.

Abra o projeto `primer25k_nanospc.gprj` na Gowin IDE, selecione o alvo,
execute Synthesis e Place & Route. Certifique-se de que estas opções estão ativadas:

- Project -> Configuration -> Place & Route -> Dual-Purpose Pin : Use READY as regular IO, use CPU as regular IO

Após isso,  grave o bitstream .fs resultante no Gowin Programmer com as seguintes configurações: 
 
- Access Mode: External Flash Mode 5A
- Operation: exFlash Background Erase,Program,Verify 5A 
- Programming Options -> File name: "arquivo.fs"
- External Flash Options -> Device: Generic Flash
- External Flash Options -> Start Address: 0x000000

Entrada de clock: E2 (50MHz), LED READY: E8

## Licensa

O código RTL da APU do SNES é adaptado do projeto NanoSPC, sob a licença
GPL-3.0-only; veja `COPYING` e `THIRD_PARTY_NOTICES.md`.

