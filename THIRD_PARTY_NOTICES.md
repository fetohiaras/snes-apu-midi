# Avisos de terceiros e proveniência

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

A cópia exata do núcleo é versionada neste repositório, em vez de ser obtida
por meio de um submódulo. Assim, um clone é autocontido e reproduzível. O
NanoSPC é uma referência upstream, e não um checkout exigido por este build.

Qualquer distribuição deste repositório, ou de bitstreams de FPGA derivados
dele, deve obedecer à GPL-3.0-only e incluir o código-fonte correspondente
completo e o texto da licença GPL-3.0. Este repositório inclui o texto da
licença em `COPYING`.
