# Diagnósticos de hardware — Tang Primer 25K

Estes testes foram adicionados para separar os três caminhos que antes estavam
misturados: USB-UART, SPC700/APU e áudio/DAC.

## 0. Pré-checagem

No diretório do projeto:

```powershell
py tools/preflight.py
```

`core_smoke.spc.hex` deve aparecer como OK. `midi_boot.spc.hex` só é necessário
para o sintetizador completo.

## 1. Teste USB-UART, sem APU e sem DAC

Abra `primer25k_uart_smoketest.gprj` no Gowin.

Em `Project -> Configuration -> Place & Route -> Dual-Purpose Pin`, habilite:

- Use CPU as regular IO
- Use READY as regular IO

Rode Synthesize + Place & Route e carregue o `.fs` em **SRAM Program**.

No Windows instale pyserial e descubra qual das duas COM do debugger é a UART:

```powershell
py -m pip install pyserial
py -m serial.tools.list_ports
py tools/uart_smoke.py COM5
```

O script envia um byte devagar. A cada byte válido o READY muda de estado. Se
isso funciona, o caminho PC -> BL616 -> B3 -> `uart_rx` está bom.

## 2. Teste do SPC700/APU, sem UART e sem DAC

Abra `primer25k_core_smoketest.gprj`.

Habilite novamente CPU e READY como GPIO, compile e carregue em SRAM.

Esse projeto usa `src/data/core_smoke.spc.hex`, gerado pelo próprio repositório.
Ele contém um programa SPC700 de seis bytes que lê a porta $F7 e ecoa o valor
na mesma porta. O FPGA gera eventos internamente e `midi_mailbox` exige o eco
como ACK.

Resultado esperado: depois da inicialização, o READY muda de estado
repetidamente. Isso comprova em hardware:

- imagem SPC lida pela A-RAM;
- `spc_parser` restaurando PC/registradores;
- SPC700 executando instruções;
- portas $F4-$F7 funcionando;
- `midi_mailbox` enviando e recebendo ACK.

Se esse teste falhar, o problema está no caminho APU/SPC700, não no USB-MIDI.
Se passar e o teste UART também passar, o próximo suspeito é o driver completo
`midi_driver.s` / configuração do S-DSP / imagem `midi_boot.spc.hex`.

Para regenerar a imagem mínima a qualquer momento:

```powershell
py tools/build_core_smoke_image.py
```

## 3. Sintetizador completo

O topo `primer25k_nanospc.gprj` precisa da imagem:

`src/data/midi_boot.spc.hex`

Ela **não deve ser substituída por um arquivo vazio**. Gere com:

```bash
cd sim
make midi-image
```

O alvo requer `spcasm`. A imagem é construída a partir de código e formas de
onda do próprio projeto; por isso agora ela pode ser versionada e compartilhada
entre os integrantes depois de gerada.

Não carregue o topo de áudio na placa até os três `IO_LOC` de I2S serem
escolhidos no `primer25k_nanospc.cst`. Enquanto o DAC não chegou, use os dois
projetos de diagnóstico acima.
