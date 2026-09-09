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
