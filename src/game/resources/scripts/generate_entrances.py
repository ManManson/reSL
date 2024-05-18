from cpp_writer import CPPWriter

import struct

# 1d7d:01fc : 132 bytes
data = b'\x05\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x09\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0b\x0c\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x07\x08\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0d\x0e\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x04\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x03\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0f\x0a\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x0f\x0f\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'

colors = {
    0: 'Green',
    1: 'Black',
    2: 'Gray',
    3: 'DarkGray',
    4: 'White',
    5: 'Yellow',
    6: 'Brown',
    7: 'Blue',
    8: 'DarkBlue',
    9: 'Red',
    10: 'DarkRed',
    11: 'Cyan',
    12: 'DarkCyan',
    13: 'LightGreen',
    14: 'DarkGreen',
    15: 'BWBlinking'
}

EntranceStructSize = 0x16

with CPPWriter('entrance.cpp') as w:
    w.writeln('#include <game/types/entrance.h>')
    w.writeln()
    w.writeln('namespace resl {')
    w.writeln()
    w.writeln('/* 1d7d:01fc : 198 bytes */')
    w.writeln('EntranceInfo g_entrances[9] = {')
    with w.indent():
        for i in range(9):
            bgColor, fgColor = struct.unpack('<BB', data[:2])
            w.writeln('{ Color::%s, Color::%s },' % (colors[bgColor], colors[fgColor]))
            for i in range(2, EntranceStructSize):
                assert(data[i] == 0);
            data = data[EntranceStructSize:]
        assert(len(data) == 0)
    w.writeln('};')
    w.writeln()
    w.writeln('} // namespace resl')
