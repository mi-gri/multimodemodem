# MultiModeModem (m³) changelog

MultiModeModem (m³) is based on MadModem by IZ6NNH; the MadModem entries follow below.

## Unreleased

- Right-hand panel reorganised for a 1000 px window without scrolling (all 13 modes,
  measured): fixed header with the Frequency area and RX · TX · TX level; tabs
  Operating (controls used during a QSO) and Parameters (mode settings set once), QRZ,
  Rotor; fixed footer with the FT slot clock, band scheduler and runtime log. Operating
  and parameter controls were split for FT8/FT4, JS8, Q65, MSK144, RTTY, CW, WEFAX and
  the radio telescope; PSK, MFSK, Hell and SSTV fit on one tab. Q65/MSK144 message
  tables are as high as their rows; the sequencer line moved into the mode group.
- Translations: old word-by-word artefacts such as "+ Add bis log" or "wartet for CQ"
  are recognised in all languages (English function words in a translated text) and
  replaced by reviewed text or the English source. German keeps amateur-radio terms:
  "Slot" instead of "Periode/Zeitschlitz", "Tune" instead of "Abst."; French and
  Norwegian use "Tune" as WSJT-X and JTDX do.
- FT TX slot button names the slot and its start: "TX slot 1 · :00/:30" / "TX slot 2 ·
  :15/:45" (FT8), "even"/"odd" (FT4); the tooltip explains the switch. German "Tune"
  instead of "Abst." on the Tune button and the TX level prefix.
- Hidden side tabs (Status, Contest) could be reached with Ctrl+Tab and then showed
  their page without a tab; keyboard navigation now skips them.
- One Frequency area above the side tabs for every mode: CAT readout (frequency, PWR,
  SWR, ALC), band, stored frequencies, free entry and the band status. The separate
  Frequency tab and the frequency/band rows in the FT and JS8 Mode tabs are gone.
- Fix: since the live-language fix, captions set from translation keys (JS8 group,
  frequency fields and others) appeared in English on a non-English start. An English
  source now finds its translation through its key; a fresh German start shows 10
  English-looking texts instead of 102 (mostly user macro text and status values).
- FT8/FT4 "Clear DX" button between TX and STOP: ends the current QSO (a running
  transmission for it stops), clears DX call and grid, resets Tx1-Tx6 and answers the
  next station in the caller queue. Auto QSO and CQ repeat stay on; the cleared station
  is not answered automatically for 15 minutes. STOP is unchanged.
- FT8/FT4: the two radio buttons TX I / TX II are one toggle button "TX I" / "TX II" as
  wide as RX and TX; a click switches the period. It follows automatic period changes when answering a
  station and is disabled while transmitting, as the radio buttons were.
- Auto mode (FT8/FT4 Auto QSO and JS8 automatic operation: AUTO replies, relays,
  heartbeat/CQ timers) is a setting again (Settings -> FT8 / FT4, off by default). Switching it on shows a notice: automatic operation may not be permitted in
  every country, can disqualify a contest entry, the station must not run unattended for
  long, and the operator is responsible for any damage. It is enabled only after "I have
  read and understood your notice and want to continue" is ticked and confirmed.
  Switching it off ends a running Auto QSO.
- Live language change: captions set from translation keys (for example the Decode menu)
  stayed in the old language until a restart. The generic translator now leads a shown
  text back to its English source; help tooltips, tooltips of fixed protocol buttons
  (SNR?, HB ...), the frequency-list status, JS8 macro tooltips, CW status, FT sequencer
  hints and the continent filter are updated too. --i18n-report with M3_I18N_SWITCH=en
  lists texts that are still German after a live change.
- FT deep decodes (OSD repair and the R30 reference recovery) are shown only as standard
  messages whose callsigns belong to a known DXCC entity; plain decodes are not filtered.
  Overnight 40 m log: 24 of 25 noise decodes and two false calls to the own station
  removed, no real decode lost on 23 recorded 20 m periods.
- FT messages with an unresolved hash as sender ("RA6ABC <...> -08") and DXpedition double
  messages ("<...> RR73; KN5XZO <NA2J> -02") are read as directed messages.
- The "FT live decode exceeded 1 second" log line appeared every period; the log now warns
  only when a live decode takes a fifth of the period (FT8 3 s, FT4 1.5 s), at most every
  10 minutes.
- FT callsign recognition accepts special event calls with several digits (DL75WAU, SX602N,
  TM100A), with or without hash brackets; "<SX602N> IW9HPG JM78" now fills the locator column.
  The pattern came from MadModem. Decodes with an unresolved hash such as
  "<...> DL7UKT JO62" fill the callsign and locator columns too and are never treated as
  addressed to the own station.
- FT8/FT4 late decodes: after the reply deadline the deep reference recovery (R30) goes
  on for up to 9 s (FT4 4.5 s) and shows what it finds as display-only rows; they never
  answer, call or arm TX. On 23 recorded 20 m periods: 348 instead of 317 decodes under
  live deadlines (WSJT-X 351, JTDX 344). Settings → FT8 / FT4, on by default.
- Faster start (Qt 5): the audio device probe (about 1.5 s with ALSA and PulseAudio)
  and the JS8 word tables load on background threads while the window is built; the
  window appears after about 1.8 s instead of 3.5 s (same machine and load). The
  compatibility device lists fill in a moment later.
- M3_STARTUP_TRACE=1 prints the startup stages with their times to stderr.
- tools/ft8_sim_band.py builds a busy simulated FT8 band from WSJT-X ft8sim signals for
  decoder speed and sensitivity measurements with the FT replay probe.
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
