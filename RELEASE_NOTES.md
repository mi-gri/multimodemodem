# MultiModeModem (m³) 0.6.6

MultiModeModem (m³) is a fork of MadModem 0.6.5 by Lucian-Ioan Papadopol, IZ6NNH,
maintained by Michael Grigutsch, DO3BOX. From 0.6.6 on m³ numbers its own versions;
the m³ build number counts on top. Source: https://github.com/mi-gri/MultiModeModem
License: GPL-3.0-or-later (see LICENSE.md).

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
