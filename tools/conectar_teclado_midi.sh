#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

PADRAO_MIDI="${1:-}"
if [ "$#" -gt 0 ]; then shift; fi

LISTA=$(python3 tools/midi_bridge.py --listar)

if [ -n "$PADRAO_MIDI" ]; then
    ENTRADA=$(echo "$LISTA" | sed -n '/^Entradas MIDI:/,/^Portas seriais:/p' \
        | grep -i -- "$PADRAO_MIDI" | head -n1 | sed 's/^  //')
else
    ENTRADA=$(echo "$LISTA" | sed -n '/^Entradas MIDI:/,/^Portas seriais:/p' \
        | grep -v '^Entradas MIDI:' | grep -v '^Portas seriais:' \
        | grep -vi 'midi through' | grep -v '(nenhuma)' | head -n1 | sed 's/^  //')
fi

if [ -z "${ENTRADA:-}" ]; then
    echo "Nenhuma entrada MIDI de hardware encontrada. Saida de 'midi_bridge.py --listar':" >&2
    echo "$LISTA" >&2
    exit 1
fi

PORTA=$(echo "$LISTA" | sed -n '/^Portas seriais:/,$p' \
    | grep -v '^Portas seriais:' | grep -v '(nenhuma' | grep -v 'ttyUSB0' \
    | head -n1 | awk '{print $1}')
if [ -z "${PORTA:-}" ]; then
    PORTA=$(echo "$LISTA" | sed -n '/^Portas seriais:/,$p' \
        | grep -v '^Portas seriais:' | grep -v '(nenhuma' | head -n1 | awk '{print $1}')
fi

if [ -z "${PORTA:-}" ]; then
    echo "Nenhuma porta serial encontrada. Saida de 'midi_bridge.py --listar':" >&2
    echo "$LISTA" >&2
    exit 1
fi

echo "Entrada MIDI: $ENTRADA"
echo "Porta serial: $PORTA"
exec python3 tools/midi_bridge.py --porta "$PORTA" --midi "$ENTRADA" --audio "$@"
