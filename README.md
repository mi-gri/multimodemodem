# MultiModeModem (m³) 0.6.6

**MultiModeModem (m³)** is a fork of **[MadModem](https://github.com/l-papadopol/MM)**
by **Lucian-Ioan Papadopol, IZ6NNH** ([madexp.it](https://www.madexp.it)), maintained by
DO3BOX at [mi-gri/MultiModeModem](https://github.com/mi-gri/MultiModeModem).
It keeps everything MadModem offers (digital modes, waterfall, CAT/PTT, rotator,
editable ADIF logbook, Auto QSO) and adds JTDX-style FT operation (decode colours,
waterfall, decode filters, late-final handling), JS8 in all four speeds, a frequency
list for all modes, PSK Reporter spots, Wavelog/Cloudlog upload and signed updates.

**MultiModeModem (m³)** ist ein Fork von MadModem (IZ6NNH). Er behält alle
MadModem-Funktionen und ergänzt einen FT-Betrieb im Stil von JTDX, JS8, eine
Frequenzliste für alle Modi, PSK-Reporter-Spots, Wavelog-Upload und signierte Updates.

### Origin and credits / Herkunft

- **MadModem** — Lucian-Ioan Papadopol, IZ6NNH: application, modems, logbook, UI.
  m³ started from MadModem 0.6.5 and numbers its own versions from 0.6.6 on.
- **WSJT-X** — Joe Taylor, K1JT, and the WSJT development group: FT8/FT4/MSK144/Q65 protocols.
- **JTDX** — JTDX team: model for the FT decode display, colours and filters; PSK Reporter
  client (Edson Pereira PY2SDR, with fixes by CE3TSK from jtdx_contest).
- **JS8Call** — Jordan Sherer, KN4CRD: JS8 protocol, message layer and receiver.
- License: GPL-3.0-or-later, see [LICENSE.md](LICENSE.md). Third-party notices:
  [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

The program file is `MultiModeModem`, and its settings and data folders are named
`MultiModeModem` as well. Settings and the default logbook are copied once from the
`MadModem` folders on first start; MadModem itself is left untouched.

---

The original MadModem description follows.

## MadModem 0.6.5

**MadModem (MM)** è un'applicazione libera per radioamatori: modi digitali, audio,
waterfall, controllo della radio e del rotore, logbook e mappe in un unico programma.
Sviluppato da **Lucian-Ioan Papadopol, IZ6NNH** — [madexp.it](https://www.madexp.it).

**MadModem** is free amateur-radio software combining digital modes, a waterfall,
CAT/PTT, antenna rotator control and an ADIF logbook in one Qt desktop application.

## Funzioni / Features

- FT8, FT4, MSK144, Q65, CW, RTTY, BPSK/QPSK, MFSK, Feld Hell, SSTV e WEFAX.
- FT8/FT4 QSO sequencer, CQ queues, Auto QSO and Doppler correction.
- CW native/neural receivers, CW/RTTY skimmers, audio CW and serial keying.
- Hamlib CAT/PTT, rotator tracking, station/DX information, QSO maps and UTC scheduler.
- ADIF logging, contest tools, macro messages, WAV recording and offline analysis.
- Help and interface in English, Italian, French, German, Norwegian and Czech.

## Download e installazione / Downloads

Scaricare il pacchetto dalla pagina [Releases](https://github.com/mi-gri/MultiModeModem/releases).

| Sistema / System | Download |
| --- | --- |
| Windows x64 | AutoCPU EXE installer; Compatible or AVX2/FMA portable ZIP |
| Ubuntu 24.04 / Mint 22.x x64 | RUN, DEB, portable TAR.GZ/ZIP |
| Fedora 43 x64 | RUN, RPM, portable TAR.GZ/ZIP |
| macOS 15+ Intel / Apple Silicon | PKG, app ZIP, DMG |

Select the package for your system and processor. Linux packages target the named
base distribution. Portable Linux: extract into a writable folder and run `AppRun`.
Installation details and user-data paths: [Installation](docs/INSTALLATION.md).

## Primo avvio / First run

1. Enter your callsign and locator in Settings → User/QTH.
2. Configure audio devices, CAT and PTT; check radio control and RX before TX.
3. Select a mode, start RX and adjust the input level without clipping.
4. For FT8/FT4, synchronize UTC. Check the selected correspondent and TX message.
5. Open the built-in Help for mode-specific instructions. Stop cancels TX and automation.

The title shows both version and build. Auto QSO decisions are recorded in
Runtime log; complete compressed session logging is available in Settings → Diagnostics.
See [Troubleshooting](docs/TROUBLESHOOTING.md).

## Sorgenti / Source

- [Build instructions](docs/BUILDING.md)
- [GitHub distribution workflow](docs/DISTRIBUTION.md)
- [Documentation](docs/README.md) · [Release notes](RELEASE_NOTES.md)

Licensed under GNU GPL-3.0-or-later; see [COPYING](COPYING), [LICENSE.md](LICENSE.md),
[authors](AUTHORS.md) and [third-party notices](THIRD_PARTY_NOTICES.md).
