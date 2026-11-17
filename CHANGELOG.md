# MultiModeModem (m³) changelog

MultiModeModem (m³) is based on MadModem by IZ6NNH; the MadModem entries follow below.

## m³ 0.6.7

- Start in a mode with a band selector (MFSK, PSK, Q65, MSK144 ...) no longer QSYs to its first band (160m) while the rig
  frequency is still unknown; the rig stays where it is.
- RTTY skimmer: a signal appears in the list only after it has decoded 4 characters; steady carriers and noise peaks
  170 Hz apart no longer fill the list with rows that have a frequency but no text.
- Safety (second audit): after a single manual CQ the sequencer no longer counts as "engaged" for hours; an unsolicited
  call is only answered within three slots after your own transmission (or with a CQ series/QSO), else it needs Auto mode.
- PSK Reporter: after a QSY of 1 kHz or more no spots are reported for one mode period + 3 s (FT8 18 s,
  FT4 10.5 s, text modes 5 s): the decodes of the last old-band slot arrived with the new band's frequency.
  A band change sends the queued spots at once.
- FT CQ retry in Auto mode: when a reply to a running CQ series becomes a QSO, the series restarts with a fresh
  count after the QSO (not after STOP, not without Auto mode). The decode table scrolls once per pass instead of
  once per late row (it seemed to jump up and down), and the own TX line follows the Auto-scroll setting.
- A stale instance lock of the old config name (NAME.mad.instance) is removed at start.
- Safety (audit of the TX gates): FT no longer answers an unsolicited call (no CQ or QSO of yours) unless
  automatic TX is allowed (Auto mode, no running QSY plan, TX band); the caller queue starts a QSO only
  under the same condition. A changed QSY plan revokes the WSPR-beacon permission of the old one. One
  confirmation for TX outside the amateur bands covers only its 100 kHz area; a QSY there stops TX.
  The plan-import dialog shows file text as plain text.
- MadModem's settings are no longer imported at the first start (only its ADIF logbook still is), so format
  changes of MadModem never reach m³.
- Configuration files are now `settings.m3cfg` and `configs/NAME.m3cfg` (was `.mad`, MadModem's name). Old files
  are converted at start by renaming; an existing `.m3cfg` is never replaced. `--config` also takes an old
  `.mad` path (continues as `.m3cfg`). Older program versions do not find the renamed file.
- WSPR: a callsign WSPR cannot send (special-event/club calls such as DL75WAU) is reported on the WSPR page
  and in the log with the reason - before and independent of the Auto-mode hint.
- JS8 relay/reply parsing accepts special-event calls with two digits (DL75WAU).
- Settings → User/QTH: optional Operator callsign for club and special-event stations; logged as
  OPERATOR (ADIF) with the station call as STATION_CALLSIGN; empty = the station call as before.
  Macro variable {OPCALL}.
- Settings → Frequencies: a note shows that changes only take effect with OK, and cancelling asks
  before dropping a changed list.
- Settings → Frequencies: a row whose frequency you change moves to its place; the list stays sorted by frequency.
- JS8: directed queries that JS8 answers automatically (SNR? GRID? INFO? STATUS? HEARING? QUERY ...
  AGN? MSG) are refused when the target is an @group or @ALLCALL ("select a station first"); a new
  CMD menu next to SNR? sends the common queries to the station in To.
- JS8: text never goes out without a target. "To" defaults to @ALLCALL (x resets to it, an empty
  field means it); any callsign or @group (e.g. @EMCOMM) can be typed, anything else is refused with a
  message. @groups you use are remembered (menu next to To; "Forget group" removes one). Only a
  station in To - not @ALLCALL or another group - counts as a QSO that pauses heartbeat/CQ repeats.
- JS8 TX progress: the status line shows "slot 2 of 3 · 0:22 / 0:45" and the message with the part
  already sent (green, struck through), the frame on air (highlighted) and the rest; at the end it
  says whether the message went out completely or was stopped in which slot.
- JS8: {RST} in macros is the SNR report of the station in To ("SNR +05"; from the RST-sent field
  of the QSO form when it holds one) - "SNR 599" went out as "SNR +05" plus a stray "99". Without a
  known report the macro sends nothing and says why. Any station set in To (heard list, inbox
  reply, typed) now fills the QSO form, so the QRZ tab follows.
- JS8: frames naming a station that cannot be an amateur callsign are dropped (and logged) -
  false decodes from noise such as "7ZWEZV/2RH: @HB HEARTBEAT QF89" at -28 dB.
  Lone text frames with no message start on their offset are dropped when they lie far out of time
  (|DT| >= 1.8 s) or at the passband edge (< 400 or > 2900 Hz), e.g. "A76EYEZ S IS I" at DT 2.1 s,
  270 Hz; continuations of a heard message are kept.
- JS8: heartbeat and CQ repeat intervals 5–60 min in 5-min steps; labels "Heartbeat interval" / "CQ interval" in all languages (de: Heartbeat-Takt, CQ-Takt).
- Runtime log: a line whose number happened to be "1" could pick up its own translation template
  and show "%1" ("CQ-Retry gestartet: %1"); the CQ retry line names FT4/FT8 correctly. Switching on
  Auto QSO while the QSY plan runs (or on a receive-only band) says that it stays paused.
- The Rotator tab on the right is shown only with rotator control on (Settings → Rotator).
- QSY plan: after a restart inside a WSPR entry marked "Send WSPR beacon", the beacon may run
  again (the permission was only set when the plan switched into the entry). A refused beacon
  switch now says why (plan entry not marked, rig on another band, band receive only, no frequency).
- Audio timeline after lost capture samples: a short audio stall (seen when Settings opened)
  shifted all FT8 timing by ~1.7 s and the clock servo needed ~15 minutes to catch up (no FT8
  decodes meanwhile). Lost frames are now detected within a few seconds and the timeline starts a
  new epoch; ordinary GUI stalls without loss do not trigger it.
- FT4/FT8 receive follows mode changes with RX running (QSY plan, operator), like JS8/Q65/WSPR:
  after the WSPR beacon had restarted RX, later plan switches to FT8/FT4 left the FT decoder deaf
  (no FT decodes on 10m/2m/70cm in the 2026-11-16 plan run); before that, FT kept decoding in the
  background in every other mode.
- Tune only beside the frequency display (all modes); the extra Tune button in the FT controls
  and the hidden one of MSK144 are gone.
- Bands (Settings → Bands): per band "TX allowed" or receive only, and preferred bands.
  - Receive-only band: a manual transmission asks first (the answer holds until the band
    changes); automatic transmissions (JS8 heartbeat/CQ/replies, FT Auto QSO, WSPR beacon)
    never start there. Until the page is saved once, every amateur band counts as TX allowed.
  - No TX without a known frequency: with CAT the rig must report its dial frequency; without
    CAT the dial frequency is entered above the tabs (Enter confirms; not kept across a restart).
    Bands, QSO form and log follow it.
  - A band change during a transmission or a running sequence (FT CQ/QSO, JS8 queue, WSPR,
    MSK144/Q65 periods, Tune) stops it; nothing continues on the new band, allowed or not.
  - Quick band buttons for the preferred bands below the frequency display: QSY to the current
    mode's frequency; receive-only bands in italics with "RX", bands without a frequency for the
    mode struck through and disabled, the current band framed. Band lists show preferred bands first.
- QSY plan: a WSPR entry can be marked "Send WSPR beacon" - the only automatic TX a running plan
  starts, on that entry's band once CAT confirms it, with Auto mode and TX allowed on the band.
  Imported plans with such entries ask before keeping them.
- Safety: while the QSY plan is on, no automatic transmission starts (JS8 heartbeat, CQ and
  replies, FT Auto QSO, WSPR beacon); manual TX stays possible. A plan switch to JS8 had sent an
  overdue heartbeat at once (test run 2026-11-16 08:40 UTC). Entering JS8 now restarts the
  heartbeat/CQ intervals instead of catching up.
- Own TX signal in the waterfall, clean and per mode (checkbox "TX" next to the waterfall speed;
  on by default, off in FT8/FT4). Shown like a strong received signal: only the top 18 dB, peak
  at ~70 % of the scale, referenced to the held peak of the transmission so keying pauses stay
  black (the wide orange stripes in Feld Hell/CW were whole key-up rows lit by residue). TX rows
  no longer touch the RX leveler, so RX continues without relearning after TX. Measured: Hell
  ~30 Hz, carrier ~26 Hz, RTTY ~73 Hz lit width, peak 180/254, RX level before/after equal.
- QSO forms of the text modes: the band field follows the rig's band (CAT) instead of showing a
  fixed "20m" hint; a band typed by the operator is kept, and a QSO logged with an empty field
  takes the CAT band (it was logged without band before).
- Tune button in every mode, between the rig frequency and PWR/SWR/ALC: FT8/FT4 use their
  tune; the other modes send a steady carrier on their TX audio frequency (RTTY between mark
  and space, CW TX tone, PSK/MFSK/Hell tone, MSK144/Q65/JS8/WSPR TX frequency, 1500 Hz for
  WEFAX/SSTV) through the normal TX path at the Tune level. Click again to stop; 60 s limit.
- CW: text plausibility gate for RX A/B. Decoded noise, fading carriers and birdies come out as
  runs of E/T/I/A/N/M; real traffic has far fewer (15-37 % against 60-89 %). Text is held back
  until the last 16 letters read like traffic, then released; it closes again on noise.
  Measured: a recorded 80 m band with two carriers 198 -> 11 false characters, synthetic
  -10/-13 dB from 150-330 % to 80-100 % character error (no invented text), clean signals
  unchanged, real recorded QSOs keep calls and exchange. Regression test m3_cw_text_gate_regression.
  Switch "Suppress noise text" in the CW settings (on by default) shows the raw output when off;
  a single short first word on a new tone appears once more text follows.
- QSY plan: a plan switch no longer clears the setting "Tune the rig to the mode's frequency
  when the mode changes": it was switched off for the moment of the switch and the mode change
  saved it so (RTTY -> FT8 then kept the frequency). A runtime flag is used instead.
- JS8 and Q65 decoded nothing after a mode switch with RX running (QSY plan or operator): their
  audio input was only switched on at RX start. It now follows every mode change (also WSPR).
- JS8 in the runtime log: one line per decoded slot ("JS8 Normal 17:36:00 UTC: 2 frame(s)")
  and one per decode, as for FT8.
- TX audio: a short sink underrun (all cores busy with a decode, 85 ms buffer) no longer aborts
  the transmission; it is logged and playback continues. Only a sink or source that stops
  advancing (watchdog, 1.5 s) or a real backend error ends it. In the test run this had cut a
  final 73 after 1.1 s at 97 % CPU.
- SSTV on the lower sideband: below 10 MHz (or where the frequency list marks the entry "LSB",
  e.g. 3.735 and 7.165 MHz) m³ switches an SSB/DATA rig mode to PKTLSB and back to PKTUSB
  elsewhere and in the other modes; the Data/USB TX route then keys PKTLSB. CW, FM and AM
  rig modes are left alone. The test run had received 80 m SSTV in USB. Rig independent: the
  mode is chosen from what the rig's Hamlib backend declares (PKTLSB, LSB-D1..D3, else plain
  LSB); a rig without any LSB mode is reported once and left as it is.
- WEFAX crash fixed (bad_alloc after 3000 free-running lines): completing an image inside the
  line loop (line limit, APT stop, end of signal) cleared the sample buffers, then one sample
  was removed from the empty buffer, corrupting the heap. Regression test
  m3_wefax_completion_regression (with Qt container asserts).
- WEFAX: the receive snapshot is copied once per new line instead of on every audio block
  (up to ~450 MB/s of copying near the end of a 3000-line image).
- RTTY signal browser (like fldigi's) under the RTTY receive text, in a resizable split:
  one row per signal the skimmer decodes in parallel (Hz, CQ call, country, text with the
  newest characters visible), sorted by frequency; CQ calls marked new DXCC / new on band /
  worked from the logbook; the country filter applies; vanished signals grey out and go after
  60 s. Double-click tunes the main decoder to that signal. Skimmer range up to 4900 Hz.
- CW: Ctrl+click on the waterfall sets the TX tone (as left+right together did; the help
  texts name both).
- WSPR beacon: unticking "Transmit as beacon" works like WSJT-X "Enable Tx": a transmission
  already on air ends normally and nothing new starts; the TX button still stops at once.
- Waterfall click and tone settings: RTTY, CW (RX A/B, skimmer) and Feld Hell reach up to 4900 Hz (the waterfall
  goes up to 5000 Hz); clicks above 3500 Hz were ignored. PSK31 stays below 3500 Hz (8 kHz
  internal rate); the CW TX tone stays below 3000 Hz. Selecting RTTY tones above 3000 Hz logs that
  the rig's TX filter would cut them.
- Country data: operating suffixes (/R rover, /P, /M, /QRP, /digit) are never taken as a
  prefix. "/R" was read as Russia: a US rover (W1ABC/R) counted as Russia and the AP noise
  decode "DO3BOX 0W2QAL/R FC14" passed the call check; such decodes are now dropped.
- FT country filter: entries may be any prefix of a DXCC entity (R9, U9, RA9 -> UA9), not
  only the primary one; the filter menu shows what each entry hides and marks entries that
  match no entity, and the runtime log reports them at start.
- FT slot separator spans the whole row; the slot clock in JS8/WSPR no longer flickers
  between RX and TX (the FT scheduler drew the same clock).
- Slot clock (side panel, "<Mode>-Slots") now in every slot mode: JS8 (6/10/15/30 s by submode), Q65
  (selected period, TX first/second), MSK144 (15/30 s, TX first/second) and WSPR (120 s), with
  the UTC clock. JS8 and WSPR have no first/second choice: one bar, red while transmitting.
- MSK144: the waterfall no longer shows the local TX monitor (as already for FT and CW).
  The noise-free local signal lit up even its -40 dB sidelobes, so the MSK looked far wider
  than on air; the generated signal itself is unchanged and measures as MSK (99 % bandwidth
  293-2754 Hz at 1500 Hz centre, as in WSJT-X).
- FT decode list: the separator before an own TX row shows the current band (CAT, else
  the selected FT band) instead of repeating the time that every row already has.
- WEFAX: new "Line start" (Operating tab) for a missed phasing. It rotates every row
  horizontally, also of a completed image, without copying it for display; PNG save and
  auto-save store the corrected image. Shift+click in the image makes that column the left
  edge (e.g. the edge of the phasing bar); 0 restores the received phase.
- WEFAX/SSTV image: the zoom hint no longer claims dragging in "fit" mode; the hand cursor
  appears exactly when the zoomed image can be dragged.
- Feld Hell: the paper wraps at the window width (columns = visible width / zoom) instead
  of 1120 columns, so no horizontal scrolling; on a resize the tape is re-wrapped without
  losing pixels. TX text is printed column by column as the audio is actually played
  (not all at once at TX start); after an abort only what was sent stays on the paper.
- Side tabs: hiding a mode-specific tab (Parameter, Contest) no longer leaves a gap, e.g.
  between "Betrieb" and "QRZ" after switching from FT8 to SSTV.
- QSY plan (band scheduler): works in every mode (WSPR added) and switches between
  modes: each entry sets mode, band and rig frequency at its UTC time, without the extra
  QSY of the mode change. One switch "Run QSY plan" replaces the per-mode switch (an
  enabled mode migrates to on). Entries can run daily or only on one UTC date (one-time
  plans such as a test run); import/export of plans as JSON and "Remove all". The plan
  never starts TX and still waits for TX (also its preparation) and an FT QSO to end.
  The mode tab shows the next entry.
- Wavelog/Cloudlog: a QSO whose upload fails is kept in a second ADIF file
  (~/.local/share/MultiModeModem/wavelog_pending.adi, owner-only) from its first failed
  attempt on, also when Settings lack URL/API key. It is sent again 1 min after start and
  every 30 min, and removed as soon as an upload succeeds or Wavelog reports it as a
  duplicate (already online). The file can be imported by hand at any time.
- Country file: Help -> "Update country file (cty.csv)" downloads the current big CTY
  from country-files.com over verified HTTPS; it replaces the data in use only when it
  parses completely (at least 300 entities, a VER date) and is not older. The copy is
  kept in the user data folder; at start the newer of it and the bundled file is used.
  Settings -> Logbook shows the version in use and offers a weekly check (off by
  default). Bundled cty.csv updated to VER20261114.
- WSPR-2: new mode. Receiver is a C++ port of the WSJT-X wsprd decoder
  (third_party/wsjtx_gpl/wspr, GPL-3.0), measured against wsprd on the same recordings;
  spot table, PSK Reporter upload of received spots, WSPR frequency list including
  2200 m and 630 m, waterfall 1200-1800 Hz at 0.73 Hz resolution.
- WSPR beacon: transmits in even minutes with an adjustable TX share; gated by the Auto
  mode switch; tones start at :01 after the even minute; every period decision
  (receive/transmit and why) is in the runtime log. The TX button stops the beacon.
- Waterfall: FFT with FFTW (CPU for FT8 124 -> 19 ms per second); upper edge as a setting
  (3000-5000 Hz); zoom with real resolution; time per row per mode, speed control at the
  bottom of the side panel; during TX the top 22 dB of the TX signal is shown.
- CAT: the receive passband follows the waterfall's upper edge for USB/LSB/PKTUSB/PKTLSB
  (on connect, on a width change and when the rig enters such a mode).
- Yaesu FTX-1: receive width set and read directly with the SH command (CAT Table 5),
  because Hamlib 4.7 releases send a malformed command; DATA SHIFT is set to half the
  width so the DATA passband runs from 0 Hz to the waterfall edge (4000 Hz: shift
  2000 Hz).
- CAT: with TX audio route Data/Pkt the mode is written before PTT only when it differs;
  this saves about 0.2 s before and after every transmission (the first answer to a CQ
  started ~0.85 s into the slot) and keeps the receive filter unchanged.
- CAT: the frequency readback always comes from the radio (Hamlib frequency cache off),
  so a lost CAT link no longer looks like a successful QSY.
- Settings: the mouse wheel changes combo boxes and number fields only when they have
  focus; otherwise it scrolls the page (a wheel turn had changed the CAT port).
- Auto QSO honours the decode filter's hidden countries and continents (watched stations
  exempt); before, the filter only hid rows and Auto QSO still answered a hidden country.
- FT: a double-click on an own TX line ("DX MYCALL ...") selects the addressee; a
  selection that would make the own call the correspondent is ignored (one
  "DO3BOX DO3BOX JO30" had been transmitted).
- FT controls: the band selector is released when an armed TX is stopped.
- WEFAX is a service mode: its frequencies are not sorted into amateur bands.
- Radio Telescope mode needs a rotator; a notice offers a bench test without rotator.
- RTTY: soft-integrated bit decisions and fade-tolerant slicer gates.
- Runtime log moved to the Help menu.
- Bundled Hamlib updated to upstream master 1b3fb4a6 (FTX-1 fixes); built without
  libsamplerate. CMake finds fftw3.h next to the FFTW library (Homebrew on Apple
  Silicon); RIG_MODEL_FTX1 is defined for older distribution Hamlib headers.

- QSO map: every repaint and every mouse move (tooltip) searched the whole logbook once
  per marker for the best known locator; with 13,500 QSOs and 1,000 markers the GUI froze
  for about 30 s per map setting change, and clicks made meanwhile toggled options back
  ("Current mode only" seemed impossible to switch off). A callsign->locator index and
  caches for the filtered markers make a filter change take about 0.6 s, a repaint 0.2 s
  and a hover lookup 0.3 ms.
- JS8: the RX button in "JS8 RX/TX settings" is gone (the RX button in the fixed header
  does the same); STOP spans the row and explains that it also cancels queued and
  automatic JS8 transmissions.
- WEFAX zoom buttons show "-" and "+" again: captions made only of symbols are never
  translated (their key was "empty", shown as "..."). German "WEFAX-RX jetzt erzwingen".
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
