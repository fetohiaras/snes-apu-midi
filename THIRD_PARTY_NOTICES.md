# Avisos de terceiros e proveniência

## Núcleos USB da ultraembedded

Os arquivos em `src/usb/` (exceto `usb_host_probe.v` e `usb_midi_fifo.v`,
escritos para este projeto) são cópias sem modificação de:

- [ultraembedded/core_usb_fs_phy](https://github.com/ultraembedded/core_usb_fs_phy),
  commit `68a63ff937e58e90f4f534b9c6705c649d545136`: `usb_fs_phy.v`
  (LGPL-2.1 ou posterior). O `usb_transceiver.v` do mesmo projeto não é
  usado; o equivalente ao seu modo 1 (acionamento direto de D+/D−) está no
  topo.
- [ultraembedded/core_usb_host](https://github.com/ultraembedded/core_usb_host),
  commit `81eb9f131dbb434a9047ae074fea5c31ef46ce5d`: `usbh_sie.v`,
  `usbh_crc5.v`, `usbh_crc16.v` (GPL-3.0). O restante do controlador
  (`usbh_host.v`, registradores AXI e FIFOs) não é usado.

## Núcleo APU do NanoSPC

Os arquivos em `src/spc_core/` são derivados de
[nand2mario/nanospc](https://github.com/nand2mario/nanospc), no commit
upstream `498ba9013abda85e152e7accba309d5e09aa6e8e` (`fix verilator build`).
O NanoSPC é licenciado sob a GNU General Public License, versão 3.0.

Os componentes copiados do projeto upstream incluem o S-DSP, o S-SMP/SPC700,
o analisador de SPC, a A-RAM de teste e a implementação completa em
`spc700/`. O projeto para a Tang Primer adiciona seu próprio módulo de topo,
a integração de clock/PLL, as restrições da placa, o testbench de captura com
Verilator e o conversor de SPC para hex.

As modificações locais em `test_aram.v` são documentadas propositalmente no
código-fonte:

- o nome do arquivo de inicialização da memória é um parâmetro, permitindo que
  a simulação e o projeto Primer usem caminhos relativos próprios;
- uma variável temporária procedural foi transformada em `wire` para ser
  aceita pelas versões atuais do Verilator.

Modificações para síntese no GW5A-25 (que não tem RAM distribuída):

- `spc700/mcode.v`: a ROM de microcódigo `M_TAB` é inferida como BSRAM (pROM),
  cujo registrador de saída só pode ser zerado no reset. O valor de reset do
  micro-instrução (passo 0 do NOP, `PC++`) não é zero; ele passou a ser
  aplicado por um multiplexador fora da ROM (`MI_RESET`/`MI_VALID`), com o
  mesmo comportamento ciclo a ciclo. Sem isso, no hardware, a primeira busca
  de opcode depois do reset não incrementa o PC e a CPU executa código errado.
- `dsp.v`: `REGRAM` e `BRR_BUF` pediam `distributed_ram`; agora são marcados
  explicitamente como registradores.
- `test_aram.v`: a A-RAM é marcada explicitamente como BSRAM.

A cópia exata do núcleo é versionada neste repositório, em vez de ser obtida
por meio de um submódulo. Assim, um clone é autocontido e reproduzível. O
NanoSPC é uma referência upstream, e não um checkout exigido por este build.

Qualquer distribuição deste repositório, ou de bitstreams de FPGA derivados
dele, deve obedecer à GPL-3.0-only e incluir o código-fonte correspondente
completo e o texto da licença GPL-3.0. Este repositório inclui o texto da
licença em `COPYING`.
