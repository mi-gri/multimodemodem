# WSJT-X derived code

`wspr/` contains a C++ port of the WSPR decoder `wsprd` from WSJT-X
(`lib/wsprd`, WSJT-X 3.0.0 source tree):

- `WsprCodec.*`: Fano sequential decoder, convolutional encoder, interleaver
  and the WSPR 50-bit message unpacking (from `fano.c`, `tab.c`,
  `wsprd_utils.c`, `wsprsim_utils.c`); `WsprMetricTable.inc` is table 2 of
  `metric_tables.c`.
- `modems/wspr/WsprEngine.*` (outside this directory) ports the decoding
  passes of `wsprd.c`.

Copyright 1994 Phil Karn, KA9Q; 2001-2018 Joe Taylor, K1JT;
2014-2018 Steven Franke, K9AN; the WSJT-X development group.

License: GNU General Public License version 3 or later, the same as m³.
File I/O, global state and fixed-size string handling were replaced; the
signal processing and coding follow the original.
