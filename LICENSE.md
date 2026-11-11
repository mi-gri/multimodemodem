# MultiModeModem (m³) license

MultiModeModem (m³) is free software: you can redistribute it and/or modify it
under the terms of the **GNU General Public License as published by the Free
Software Foundation, version 3 of the License, or (at your option) any later
version** (SPDX: `GPL-3.0-or-later`). The full license text is in `COPYING`.

MultiModeModem is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

## Copyright

- **MadModem**, on which MultiModeModem is based (fork of MadModem 0.6.5,
  https://github.com/l-papadopol/MM):
  Copyright (C) 2026 Papadopol Lucian-Ioan, IZ6NNH <l.i.papadopol@gmail.com>,
  www.madexp.it. MadModem is distributed under the GNU GPL version 3.
- **MultiModeModem modifications and additions** (from November 2026; see the
  Git history and `CHANGELOG.md`):
  Copyright (C) 2026 Michael Grigutsch, DO3BOX <do3box-github@migri.de>.
- **Third-party code** keeps its own copyright and license notices; see
  `THIRD_PARTY_NOTICES.md` and the license files under `third_party/`.

This is a modified version of MadModem (GPLv3 section 5a). Where a source file
states its own license (SPDX identifier or header), that statement applies to it.

## Why the GPL

The combined program contains GPLv3 code (MSHV/WSJT-X FT8/FT4/MSK144/Q65
material, JS8Call, Decodium, MadModem itself), so the whole program can only be
distributed under the GPL version 3 or later. All other components are
compatible with it:

| Component | License |
|---|---|
| MadModem, MSHV / WSJT-X material, JS8Call, Decodium | GPL-3.0 (-or-later) |
| gMFSK and fldigi references, FFTW3 | GPL-2.0-or-later |
| Hamlib, MMSSTV-derived SSTV core | LGPL-2.1-or-later / LGPL-3.0-or-later |
| Qt (dynamically linked) | LGPL-3.0 |
| Khronos OpenCL headers, OpenSSL 3 (Windows package) | Apache-2.0 |
| CRC++ | BSD-3-Clause |
| zlib | Zlib |
| TweetNaCl (Ed25519 verification) | public domain |

Apache-2.0 is compatible with GPLv3 but not with GPLv2, so the program cannot
be distributed under GPLv2 alone. Some third-party trees are kept only as
reference material and are not compiled; `THIRD_PARTY_NOTICES.md` lists which.

## Binary packages

Whoever distributes a binary package must make the corresponding source
available (GPLv3 section 6). For the libraries bundled into the Windows, macOS
and portable Linux packages, see "Licenses of bundled libraries" in
`docs/DISTRIBUTION.md`.

This file is a summary, not legal advice.
