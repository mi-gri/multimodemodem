# Third-party notices and credits

MultiModeModem (m³) is a GPL-3.0-or-later combined work based on MadModem 0.6.5
(see `LICENSE.md`). This file summarizes the upstream material currently present
in the tree. The individual upstream license files under `third_party/` remain
authoritative. Sections that name a MadModem version describe material taken
over from MadModem.

## MultiModeModem (m³)

Modifications and additions since the fork (JTDX-style FT operation, JS8,
frequency list, Wavelog, PSK Reporter spots, signed updates and others):
**Michael Grigutsch / DO3BOX**, GPL-3.0-or-later. Third-party material added
by m³ is listed below (JS8Call, JTDX PSK Reporter client, TweetNaCl).

## Original MadModem project material

Project lead, original concept, UI/product direction, integration, testing and release packaging: **Papadopol Lucian-Ioan / IZ6NNH / MadExp**.

Original MadModem material not otherwise credited to a third-party project is part of this GPLv3 package.

The project uses an AI-assisted coding workflow under human direction. The author remains responsible for the product concept, requirements, testing and release decisions.

## Compiled or linked third-party material

### MSHV / WSJT-X-related FT material

MSHV Copyright (C) 2015 Hrisimir Hristov — LZ2HV.
Upstream project and source: https://sourceforge.net/projects/mshv/

Location: `third_party/mshv_gpl/port/`

Used for FT8/FT4 protocol support, including message packing/unpacking, generators, CRC/LDPC helpers and related FT support code.

License: GPL, as preserved in `third_party/mshv_gpl/`.

### Hamlib

Location: `third_party/hamlib_lgpl/source/`

Used for radio CAT/PTT and rotator control. The bundled build scripts build Hamlib for Linux and Windows/MXE.

License: Hamlib library LGPL-2.1-or-later; some upstream tools/examples are GPL-2.0-or-later.



Local portability adjustments in source revision R3: `lib/precise_time.c` includes
`assert.h` and selects the appropriate Windows constructor mechanism;
`configure.ac` uses literal whitespace-separated feature-probe lists. Original
copyright and license headers are retained.

### MMSSTV-derived SSTV core

Location: `third_party/mmsstv_lgpl/MmsstvRxCore.*`

Used as the compiled SSTV RX helper in MadModem. The file itself states that it is derived from concepts and constants in MMSSTV's LGPL `sstv.cpp` / `sstv.h`.

License: LGPLv3-or-later, as preserved in `third_party/mmsstv_lgpl/`.

Credit: Makoto Mori and Nobuyuki Oba.

### Decodium / Raptor adapted NTP client

Location: `third_party/decodium_gpl/port/NtpClient.*`

Used as an adapted Qt NTP client.

License: GPL, as preserved in `third_party/decodium_gpl/`.



### AD1C/K1EA country file

Location: `cty.csv`

Used for DXCC/country/prefix lookup. Preserve upstream attribution when updating the file.

### Qt

MadModem is a Qt application. Build and distribution must respect the license of the Qt version used.

## Bundled reference material not compiled into the current MadModem target

The following directories are kept for attribution, traceability, study or future comparison, but are not listed in the current `MadModem` CMake target source list:

- `third_party/qsstv_gpl/reference/`;
- `third_party/mmsstv_lgpl/reference/`;
- `third_party/mshv_gpl/reference/`;
- `third_party/mshv_gpl/upstream_2765/`;
- `third_party/decodium_gpl/reference/`.

QSSTV is important as the reference for the sound-card calibration workflow, but the current MadModem executable does not compile or link QSSTV source files.

## Reimplemented / inspired-by work

The following projects influenced MadModem design, but their source code is not directly copied into the current MadModem-native files unless separately listed above:

- WSJT-X: FT slot timing, sequencing, parser/QSO-state behaviour.
- MSHV / WSJT-X / JTDX family: FT receiver architecture ideas and live-decode budgeting.
- fldigi / gmfsk family: PSK/QPSK/MFSK receiver design ideas.
- QSSTV: sound-card calibration workflow idea.
- CatRotator-style programs: rotator workflow and UX ideas.
- Meeus/NOAA-style formulae: Moon / EME local ephemeris calculation.


## gMFSK MFSK16 reference material

MadModem's standard MFSK16 Varicode/FEC implementation was written for MadModem using the public gMFSK source as protocol reference for the IZ8BLY MFSK Varicode table, R=1/2 K=7 convolutional FEC polynomials, and the 10-stage diagonal interleaver/deinterleaver behaviour. gMFSK is GPL-2.0-or-later; MadModem is GPL-3.0, which is compatible with GPL-2.0-or-later code and protocol-table reuse.

Referenced components include gMFSK `mfsk.h`, `mfskrx.c`, `mfsktx.c`, `interleave.c`, and `varicode.c`.


## fldigi GPL reference core

MadModem 0.5.27 uses fldigi source code supplied by the user as a GPL reference for text-mode DSP behaviour. The integrated changes are Qt/C++ MadModem code, but PSK symbol sampling/Varicode handling, MFSK16 softdecode/FEC/interleaver structure and Feld Hell raster orientation were compared against fldigi's GPL sources under `src/psk`, `src/mfsk` and `src/feld`.

## MSHV / WSJT-X MSK144 GPL components

MadModem 0.5.73 integrates GPL-compatible MSK144 support derived from user-supplied
MSHV source. The assimilated components are used for MSK144 message generation,
LDPC decode helpers, hashing, and message pack/unpack support.

Integrated locations include:

- `third_party/mshv_gpl/port/HvGenMsk/`
- `third_party/mshv_gpl/port/HvPackUnpackMsg/`
- `third_party/mshv_gpl/nhash.*`
- `third_party/mshv_gpl/HvTxW/hvqthloc.*`
- `modems/msk144/`

Original algorithms, protocol specifications, and related WSJT-family source are
copyright the WSJT Development Group and MSHV contributors; the MSHV C++ rewrite
and modifications are attributed to Hrisimir Hristov, LZ2HV, where present in the
source headers. MadModem remains GPLv3-compatible.


## MSHV Q65 protocol-codec source assimilation

Q65 message packing, QRA coding/decoding and waveform-symbol generation use the
GPL-derived `HvGenQ65`, `q65_subs` and `pack_unpack_msg77` components. MadModem's
active synchronization, spectral analysis, candidate search, drift refinement
and averaging code is under `modems/q65/`.

The GPL-derived 77-bit packer and `nhash` implementation are compiled once in
MadModem's shared weak-signal codec target and used by FT4/FT8, MSK144 and Q65.
This changes target ownership and synchronization only; it does not relicense or
claim original authorship of those protocol primitives.

The imported `DecoderQ65`/`decoderpom` and FFTW header remain under
`third_party/mshv_gpl/` as attributed reference material. They are not compiled
or linked into the active Q65 target.

## MM Neural CTC (source R5)

Original MadModem temporal-CNN/CTC runtime, synthetic audio generator and independently
trained weights: Copyright (C) 2026 Papadopol Lucian-Ioan; GPL-3.0-or-later.
No DeepCW code or parameter tensors are included. DeepCW repositories were studied
as architectural references. General CTC and gated recurrent unit techniques are
identified in `tools/cw_neural/README.md`. Training-only PyTorch/NumPy/SciPy are
not linked into or required by the MadModem application.

## Khronos OpenCL C headers

Location: `third_party/opencl_headers/`. Unmodified cl.h, cl_platform.h and cl_version.h from Khronos OpenCL-Headers, packaged as Ubuntu opencl-c-headers 3.0~2023.12.14-1. Copyright The Khronos Group Inc.; Apache License 2.0. Full LICENSE and package COPYRIGHT are included there. These declarations are compiled locally; the optional OpenCL runtime is loaded dynamically and is not bundled. Upstream: https://github.com/KhronosGroup/OpenCL-Headers

## MSHV 2.76.7 FT8/FT4 recovery (R30)

Location: `third_party/mshv_gpl/ft_reference/`. GPLv3, original WSJT Development Group algorithms and MSHV C++ rewrite/modifications by Hrisimir Hristov LZ2HV. Exact upstream archive, source hashes and modifications are recorded in `ft_reference/PORT_NOTES.md` and `docs/provenance/ft-reference.json`. This separate headless FT target compiles its own adapted decoderpom/FFT implementation and links FFTW3 double precision. The Q65 target remains independent of FFTW.

FFTW3 (Matteo Frigo and Steven G. Johnson): external GPL-2.0-or-later library, compatible with this GPLv3 application; upstream https://www.fftw.org/. Distributors must preserve its license and meet the corresponding-source obligations for the version they redistribute. JTDX and WSJT-Z are source comparison references; their applications and Fortran runtimes are not linked into this recovery target.

## Native FT DSP reimplementation, source R31

The native C++ spectral downconversion and complex Costas refinement were reimplemented with reference to the WSJT Development Group's WSJT-X 3.0.2 FT8 algorithms. Their pinned source inventory is in docs/provenance/ft-native-reference.json. FFTW3 remains the existing external numerical library. No WSJT-X decoder engine or Fortran runtime is linked by this change. Existing original WSJT/MSHV attributions and GPL obligations remain in effect.

## JS8Call 2.2.0 (JS8)

The JS8 mode uses material from JS8Call 2.2.0 by Jordan Sherer KN4CRD,
GPL-3.0-or-later: the Varicode/JSC message layer, the JSC word tables and the
JS8 frame unpacking (`third_party/js8call_gpl`), and C++ ports of its Fortran
JS8 receiver, LDPC(174,87) code and transmitter (`modems/js8`), and ports
of its message grouping, directed-command replies, relay and inbox rules and
its standard frequency list (`Js8CommandBuffer`, `Js8AutoReply`, `Js8Inbox`,
`utils/StandardFrequencies.h`).  The other default working frequencies in
`utils/StandardFrequencies.h` are those of WSJT-X 3.0 (`models/FrequencyList.cpp`,
GPL-3.0-or-later); the other modes use public band-plan and activity
frequencies (IARU Region 1, Feld Hell Club, DWD radiofax schedule).  JS8Call is
based on WSJT-X (Joe Taylor K1JT and the WSJT development group).  CRC++ by
Daniel Bahr is BSD 3-clause.  Details: `third_party/js8call_gpl/NOTICE.md`.

## JTDX — PSK Reporter client and FT display

`network/PskReporter.*` is a C++/Qt port of the PSK Reporter client of JTDX
(Edson Pereira PY2SDR and the JTDX team, GPL-3.0), with the 1200-byte datagram
and 5-byte frequency fixes from jtdx_contest (CE3TSK). The FT decode colours,
waterfall and decode filters follow JTDX's behaviour; that code is written for
m³. Upstream: https://sourceforge.net/projects/jtdx/

## zlib — compressed session logs

Session diagnostics use the platform zlib library (zlib license) for standard gzip compression. Project and license: https://zlib.net/. No zlib source is embedded by this change.

## TweetNaCl — update signature verification

`updates/Ed25519Verify.cpp` ports the Ed25519 verification of TweetNaCl
20140427 by Daniel J. Bernstein, Bernard van Gastel, Wesley Janssen, Tanja
Lange, Peter Schwabe and Sjaak Smetsers (public domain, https://tweetnacl.cr.yp.to/),
adapted to C++ with SHA-512 from Qt and a canonical-S check per RFC 8032.
