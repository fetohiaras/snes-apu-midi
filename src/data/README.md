# Local SPC test image

`test_spc.spc.hex` is intentionally not version-controlled.  It is generated
from a locally obtained `.spc` snapshot, which may contain copyrighted game
music and samples.

Generate an image before simulation or synthesis:

```bash
cd sim
make spc-to-hex SPC="/absolute/path/to/song.spc"
```

The converter writes `test_spc.spc.hex` here.  The current Primer top uses
that exact file name as its BSRAM initialisation image.

For a public demo, use only an SPC snapshot whose audio/data rights permit
redistribution, or provide a separately documented local acquisition step.

## Imagens de diagnóstico e MIDI

- `core_smoke.spc.hex` é um artefato pequeno e redistribuível gerado por
  `tools/build_core_smoke_image.py`; ele é versionado para o teste de hardware
  do SPC700/mailbox.
- `midi_boot.spc.hex` é gerado por `tools/build_midi_image.py` a partir do
  driver e do banco sintético do próprio projeto. Ele também pode ser
  versionado para que todos os integrantes sintetizem a mesma imagem.
- `test_spc.spc.hex` continua local/não versionado porque pode conter música e
  amostras de jogos protegidos por direitos autorais.
