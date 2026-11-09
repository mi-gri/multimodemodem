# JS8Call 2.2.0 material

JS8 support in MultiModeModem follows JS8Call 2.2.0 by Jordan Sherer KN4CRD
(GPL-3.0-or-later, see `COPYING`), which builds on WSJT-X by Joe Taylor K1JT
and the WSJT development group.

Files in this directory:

- `Varicode.h/.cpp`: JS8Call's `varicode.*` (message frames, directed
  commands, heartbeats, compound calls, Huffman and JSC data frames).
  Wrapped in namespace `js8`; debug output compiled out; the frame-preview
  thread that needs WSJT-X's DecodedText is left out.
- `Jsc.h/.cpp`: JS8Call's `jsc.*` (dense word coding).  The tables are read
  from `jsc_tables.txt` instead of the 12 MB `jsc_map.cpp`/`jsc_list.cpp`
  initialisers; a locked lookup cache was added.
- `jsc_tables.txt`: the data of `jsc_map.cpp`, `jsc_list.cpp` and the prefix
  table, converted unchanged by `tools/js8_generate_jsc_tables.py`.
- `DecodedFrame.h/.cpp`: the JS8 part of JS8Call's `decodedtext.*`
  (unpacking of received frames).
- `Js8LdpcTables.h`: LDPC(174,87) generator, column order and parity-check
  tables of `lib/ft8/ldpc_174_87_params.f90` and `bpdecode174.f90`
  (WSJT-X, Joe Taylor K1JT and Steve Franke K9AN).
- `crc.h`: CRC++ 0.2.0.6 by Daniel Bahr (BSD 3-clause, notice in the file).

The receiver, LDPC decoder and transmitter in `modems/js8/` are C++ ports of
the Fortran routines `js8?_decode.f90`, `syncjs8.f90`, `js8_downsample.f90`,
`syncjs8d.f90`, `js8dec.f90`, `subtractjs8.f90`, `baselinejs8.f90`,
`genjs8.f90`, `encode174.f90`, `bpdecode174.f90`, `osd174.f90` and
`chkcrc12a.f90` of JS8Call 2.2.0.
