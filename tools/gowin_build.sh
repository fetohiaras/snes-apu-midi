#!/usr/bin/env bash
# Headless Gowin build (synthesis, place & route, bitstream) of the project,
# with the files, constraints and options of primer25k_nanospc.gprj /
# impl/primer25k_nanospc_process_config.json.
#
#   tools/gowin_build.sh [OUTDIR]          default OUTDIR: build/gowin
#
# GOWIN_IDE points to the IDE folder (the one with bin/gw_sh). Output:
# OUTDIR/impl/pnr/primer25k_nanospc.fs and the reports next to it.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$(realpath -m "${1:-$ROOT/build/gowin}")"
GOWIN_IDE="${GOWIN_IDE:-/mnt/01DAF1EB02C192E0/Software/Linux/GOWIN IDE/IDE}"
GW_SH="$GOWIN_IDE/bin/gw_sh"
[ -x "$GW_SH" ] || { echo "gw_sh not found: set GOWIN_IDE to the Gowin IDE folder" >&2; exit 1; }

mkdir -p "$OUT"
TCL="$OUT/build.tcl"
{
    echo "set_device -name GW5A-25A -device_version A GW5A-LV25MG121NC1/I0"
    # every enabled file of the IDE project, in order
    sed -n 's/.*<File path="\([^"]*\)" type="[^"]*" enable="1".*/\1/p' "$ROOT/primer25k_nanospc.gprj" |
        while read -r f; do echo "add_file {$ROOT/$f}"; done
    cat <<EOF
set_option -output_base_name primer25k_nanospc
set_option -top_module primer25k_nanospc_top
set_option -verilog_std sysv2017
set_option -include_path {$ROOT}
set_option -use_ready_as_gpio 1
set_option -use_cpu_as_gpio 1
set_option -use_sspi_as_gpio 1
set_option -gen_text_timing_rpt 1
set_option -timing_driven 1
set_option -place_option 2
set_option -route_option 2
run all
EOF
} > "$TCL"

cd "$OUT"
echo "building in $OUT (log: $OUT/gw.log)"
if ! LD_LIBRARY_PATH="$GOWIN_IDE/lib" "$GW_SH" "$TCL" > gw.log 2>&1; then
    grep -E "ERROR" gw.log | head -20 >&2
    exit 1
fi

# summary: resources and the clocks' Fmax
RPT="$OUT/impl/pnr/primer25k_nanospc.rpt.txt"
sed -n '/Resource Usage Summary/,/I\/O Bank/p' "$RPT" | grep -E 'Logic  |Register  |CLS|BSRAM  |DSP  ' || true
grep -A8 'Max Frequency Summary' "$OUT/impl/pnr/primer25k_nanospc.tr" | grep -E 'MHz' || true
echo "bitstream: $OUT/impl/pnr/primer25k_nanospc.fs"
