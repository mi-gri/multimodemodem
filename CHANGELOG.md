# MultiModeModem (m³) changelog

MultiModeModem (m³) is based on MadModem by IZ6NNH; the MadModem entries follow below.

## Unreleased

- Faster start (Qt 5): the audio device probe (about 1.5 s with ALSA and PulseAudio)
  and the JS8 word tables load on background threads while the window is built; the
  window appears after about 1.8 s instead of 3.5 s (same machine and load). The
  compatibility device lists fill in a moment later.
- M3_STARTUP_TRACE=1 prints the startup stages with their times to stderr.
- Security: JS8 text decompression read past a table for crafted data frames (a run of
  eight or more 4-bit groups); such frames are now refused. Found with UBSan.
- Security: received JS8 text in the band-activity and inbox tooltips is shown as plain
  text; a station could insert HTML (formatting, local images) before.
- MSK40 short messages: the last tone read one byte past its bit array (Fortran wrap
  ported off by one); it now wraps to the first bit as in WSJT-X.
- New test m3_unpack_fuzz_regression: random payloads through the FT/MSK/Q65 and JS8
  unpackers and the JS8 reply rules.
- SSTV archive: the file name is reserved atomically, so two images saved in the same
  second can no longer replace each other.

## m³ 0.6.6

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

## m³ 0.6.5

- Rename to MultiModeModem (m³): program file, application name, data folders (settings
  and default logbook copied once from MadModem), proposed file names, ADIF PROGRAMID,
  desktop entry, title, About and help; README and AUTHORS state origin and credits.
- FT8/FT4 waterfall as in JTDX: fine FFT resolution, adjustable row time, UTC line per
  T/R period, CQ calls on the waterfall, RX/TX markers as signal-width bands.
- JTDX decode colours with "Default colours" reset; clearer line between periods.
- Decode filters: CQ/RR73/73 only, worked stations (last QSO within N days), countries,
  continents; rows stay hidden while the logbook answer is pending.
- Auto QSO always available (password-gated mode removed).
- Late RR73/RRR/73 after a given-up QSO is logged and answered with 73.
- PSK Reporter client for received FT8/FT4 stations (from JTDX, with the 1200-byte and
  5-byte frequency fixes of jtdx_contest); antenna per band.
- TX level in dB per band and mode, separate Tune level.
- Mode, RX state and window geometry remembered across restarts.
- Audio device list cached (fast Settings dialog, FT TX audio on time); waterfall
  history kept across TX cycles; local build number counts up automatically.

## MadModem 0.6.5

- Add correlated operational traces for Evil Auto QSO in Runtime log and compressed session logs.
- Show the distribution build number beside the version in the window title.
- Consolidate current documentation and remove obsolete development reports.

Earlier release information is available in [GitHub Releases](https://github.com/l-papadopol/MM/releases).
