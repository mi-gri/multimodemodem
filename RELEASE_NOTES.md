# MultiModeModem (m³) 0.26.12

MultiModeModem (m³) is a fork of MadModem 0.6.5 by Lucian-Ioan Papadopol, IZ6NNH,
maintained by Michael Grigutsch, DO3BOX. From 0.6.6 on m³ numbers its own versions;
the m³ build number counts on top. Source: https://github.com/mi-gri/MultiModeModem
License: GPL-3.0-or-later (see LICENSE.md).

## New in MultiModeModem 0.26.12

### Before you update

- Version numbers now follow 0.YY.MM (year, month): 0.26.11 follows 0.6.7.
- Configuration files are now `settings.m3cfg` and `configs/NAME.m3cfg`; old files are
  renamed at the first start. Older program versions do not find the renamed files.
- MadModem's settings are no longer imported (its ADIF logbook still is).
- Until Settings → Bands is saved once, every amateur band counts as TX allowed.
- Help → "Include beta versions in update checks" is off by default: only releases are
  offered.

### Transmit safety

- PTT is released first when the program is closed, and also when the system ends it
  (shutdown, log-off, UPS shutdown, end of the desktop session). Nothing keys the
  transmitter afterwards; the end of the desktop session switches Auto mode off.
- The TX band permission checks the whole signal (dial plus audio range, sideband from
  CAT) and also covers serial CW keying. No transmission without a known frequency.
- Bands can be set to receive only. A band change during a transmission or a running
  sequence stops it.
- A transmission (not Tune) is stopped when the radio reports a high SWR twice in a row
  (on by default, limit 2.5).
- No automatic transmission starts while the QSY plan is running, except a WSPR beacon
  entry marked for it. Unsolicited calls are answered only when automatic TX is allowed.
- A new QSO is never started automatically from an a-priori (AP) decode or from an
  implausible callsign.

### Operation

- WSPR-2 receive and beacon: a C++ port of the WSJT-X wsprd decoder with spot table and
  PSK Reporter upload; the beacon transmits in even minutes with an adjustable TX share,
  only in Auto mode.
- JS8: messages always have a target, TX progress display, CMD menu for common queries,
  heartbeat and CQ intervals of 5-60 minutes.
- FT8/FT4: new "Clear DX" button, CQ series continues after an unanswered QSO, RR73 is
  repeated when the partner missed it, more deep decodes shown after the reply deadline.
- New "Auto" switch in the menu bar with a notice; new "Filter" menu with one list of
  excluded countries and continents for all automatic modes and the views.
- Frequency area for every mode, quick band buttons, preferred bands, Tune in every mode
  with adjustable length, entry of the dial frequency without CAT.
- QSY plan for every mode, with one-time entries and JSON import/export.
- RTTY signal browser, CW noise text suppression, WEFAX line start correction, Feld Hell
  paper wrapping to the window width, SSTV on the lower sideband.
- Wavelog/Cloudlog: failed uploads are kept and sent again. Country file update from the
  Tools menu. Optional operator callsign for club and special-event stations.
- New start parameter `--startupmenu` to choose a configuration.
- Menu bar and side panel reorganised; default theme Classic Dark for new installations.

### Performance

- Waterfall based on FFTW with far lower CPU load, upper edge selectable 3000-5000 Hz,
  zoom with real resolution, speed per mode; the receiver passband follows via CAT.
- QSO map no longer freezes with large logbooks; faster program start; faster first answer
  to a CQ with TX audio route Data/Pkt.

### Fixes

- macOS/Windows: crash at start ("Thread stack size exceeded") fixed.
- No FT decodes after a mode change, or no JS8/Q65 decodes after a mode switch with RX
  running; WEFAX crash after 3000 free-running lines.
- Timing recovery after lost audio samples; a short audio underrun no longer aborts a
  transmission.
- Fake It split is retried when the radio rejects the command; Auto mode no longer stays
  blocked by an empty QSO from the caller queue.
- Yaesu FTX-1: receive width and DATA SHIFT are set directly (Hamlib releases send a
  malformed width command). Bundled Hamlib updated.
- Security: crafted JS8 frames could read past a table; received JS8 text in tooltips is
  shown as plain text; project images in the SSTV editor are limited before decoding.

Details: CHANGELOG.md.

## New in MultiModeModem 0.6.6

- JS8 (JS8Call 2.2 protocol) in all four speeds Normal, Fast, Turbo and Slow: native C++
  port of the JS8Call receiver, LDPC decoder and modulator; band activity, heard stations,
  messages, macro bar, decoding of all speeds at once, directed commands with automatic
  replies (AUTO), relay, inbox, heartbeat/CQ timers and idle watchdog.
- Frequency list for all modes (Settings → Frequencies, as in WSJT-X/JTDX) with defaults
  from WSJT-X 3.0, JS8Call 2.2.0 and the IARU band plans; QSY on mode change; the band
  scheduler uses the list.
- New side tab Frequency for every mode: band, the stored frequencies of the band and a
  free frequency entry (FT and JS8 keep their band selector in the Mode tab). The QRZ tab
  is hidden for weather fax and the radio telescope. Frequencies outside the amateur bands (weather RTTY and fax) form
  their own group; a mode change leaves the rig on such a station. DWD weather RTTY
  stations and a matching RTTY preset (50 Bd, 450 Hz) are included.
- Frequency list: "Add missing defaults" adds the default entries the list lacks and keeps
  own entries. Saved lists get the defaults of modes they did not know yet, and an
  unchanged list is recognised in any order (it was stored as a fixed list before).
- Waterfall marker labels (RTTY Mark/Space, RX/TX) readable: text in black or white on a
  tag in the marker colour.
- PSK Reporter spots also for JS8, MSK144, Q65, RTTY, PSK, MFSK and CW.
- Wavelog/Cloudlog: logged QSOs and optionally the radio status are uploaded directly.
- Transceiver readout: CAT frequency (mouse wheel and double-click tune), TX power, SWR
  and ALC during transmission.
- Auto QSO keeps the TX marker with Hold TX, skips directed CQs the own station does not
  fit and answers only callers from the area of an own directed CQ.
- Settings are stored centrally; `--config NAME` selects separate configurations.
- One instance per configuration: a second start on the same settings file shows a message
  and exits; other configurations (`--config NAME`) run side by side.
- QRZ.com moved into the Logbook settings; new tab Display / FT colours; F2 opens Settings.
- Faster logbook (QSOs appended instead of rewriting the file), cty lookup and parsing;
  session diagnostics limited to 2 GB.
- Security: signed updates (Ed25519 signature on SHA256SUMS.txt, checked before an update
  is installed); settings file owner-only (0600); Wavelog refuses plain http to internet
  hosts including IPv6 addresses; ADIF field names are checked; JS8 relay off by default
  and automatic directed answers rate-limited.
- New m³ logo and program icon.
- German: tooltips and help of the text and image modes, the CAT and sound-card
  calibration pages of Settings, the logbook contest export and the About dialog are
  translated (they were English); About lists JS8 and the GPL-3.0-or-later licence.
  `--i18n-report` lists texts that are still English in the configured language.
- Help viewer uses the dark colours of the help pages in every theme; links were blue on
  grey before. About keeps the MadModem credits (the product renaming had replaced them).
- Source identifiers, files and build targets renamed from MadModem to M3/MultiModeModem;
  builds with Qt 5 and Qt 6 (macOS) again.
- License stated as GPL-3.0-or-later with the m³ copyright; notices for JTDX and the
  libraries bundled in the binary packages.

Updating from 0.6.5: the updater of 0.6.5 does not check signatures yet; from 0.6.6 on,
updates are only installed when their signature verifies.

## New in MultiModeModem 0.6.5

- New name MultiModeModem (m³): program file, desktop entry, title, About, help and
  proposed file names. Settings and the default logbook are copied once from the
  MadModem folders on first start; MadModem itself is left untouched.
- FT8/FT4 operation in the style of JTDX:
  - Waterfall with fine resolution (FT8 tones stand apart), adjustable row time and
    a UTC line at every T/R period.
  - CQ calls shown on the waterfall; RX/TX markers as bands of the signal width.
  - Decode table colours as in JTDX (new DXCC, grid, call per band), with a reset to
    the default colours and a clearer line between decode periods.
  - Decode filters: only CQ/RR73/73, hide worked stations (optionally only those
    worked within N days), hide countries (cty.dat prefixes) and hide continents.
    Watched stations and lines to your call always stay visible.
- Auto QSO is always available without a password.
- A late RR73/RRR/73 after a given-up QSO (both reports exchanged) is logged and
  answered with 73.
- PSK Reporter: received FT8/FT4 stations are reported to pskreporter.info
  (client taken over from JTDX). Off by default; Settings → Logbook.
- Antenna per band in Settings → User / station / QTH, used by PSK Reporter and {ANT}.
- TX level in dB, remembered per band and mode, with its own level for Tune.
- Mode, RX state and window size and position are remembered across restarts.
- Faster: the Settings dialog opens at once and FT TX audio starts on time (audio
  device list cached). The waterfall keeps its history across TX cycles.

## MadModem 0.6.5 release notes

- Evil Auto QSO now records CQ acceptance/rejection, priority selection, logbook
  waits, queue waits, received replies, sequencer progress, TX preparation and
  actual audio events, retries, timeouts, completion and ADIF save results.
- Automation records include session/QSO identifiers, UTC slots, messages,
  queue sizes and TX plan tokens. Repeated unchanged waits are coalesced.
- The application title displays version and build, including after language changes.
- Documentation is organized around installation, operation, building and distribution.
- Existing decoder, Doppler, manual TX takeover, CQ priority policies and compressed
  complete logging remain available. Linux RUN/DEB/RPM and portable downloads continue.
