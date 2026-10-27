#!/usr/bin/env python3


def bytes_to_bits_lsb_first(data: bytes):
    bits = []
    for byte in data:
        for i in range(8):
            bits.append((byte >> i) & 1)
    return bits


def int_to_bits_lsb_first(value: int, width: int):
    return [(value >> i) & 1 for i in range(width)]


def crc_raw(data_bits, poly_refl, init, width):
    reg = init
    mask = (1 << width) - 1
    for bit in data_bits:
        t = (reg & 1) ^ bit
        reg >>= 1
        if t:
            reg ^= poly_refl
        reg &= mask
    return reg


def analyze(name, poly_refl, init, xorout, width, check_value):
    check_bytes = b"123456789"
    bits = bytes_to_bits_lsb_first(check_bytes)
    computed = crc_raw(bits, poly_refl, init, width) ^ xorout
    status = "OK" if computed == check_value else "DIVERGIU"
    print(f"[{name}] CRC(\"123456789\") = 0x{computed:0{width//4}X} "
          f"(catalogo 0x{check_value:0{width//4}X}) {status}")
    assert computed == check_value, f"{name}: poly/init/xorout nao batem com o catalogo"

    residuals = set()
    for msg in (b"123456789", b"\x00", b"\xff\xff\xff", b"A note-on msg!",
                b"", b"\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a"):
        msg_bits = bytes_to_bits_lsb_first(msg)
        tx_crc = crc_raw(msg_bits, poly_refl, init, width) ^ xorout
        full_bits = msg_bits + int_to_bits_lsb_first(tx_crc, width)
        resid = crc_raw(full_bits, poly_refl, init, width)
        residuals.add(resid)

    assert len(residuals) == 1, f"{name}: residuo nao e constante entre mensagens: {residuals}"
    good_resid = residuals.pop()
    print(f"[{name}] residuo do receptor (constante p/ qualquer quadro valido) = "
          f"0x{good_resid:0{width//4}X}")
    return good_resid


if __name__ == "__main__":
    r5 = analyze("CRC5/USB", poly_refl=0x14, init=0x1F, xorout=0x1F,
                 width=5, check_value=0x19)

    r16 = analyze("CRC16/USB", poly_refl=0xA001, init=0xFFFF, xorout=0xFFFF,
                  width=16, check_value=0xB4C8)

    print()
    print(f"Usar em usb_midi_defs.vh: CRC5_GOOD_RESID  = 5'h{r5:02X}")
    print(f"Usar em usb_midi_defs.vh: CRC16_GOOD_RESID = 16'h{r16:04X}")
