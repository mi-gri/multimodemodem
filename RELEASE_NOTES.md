# MultiModeModem (m³) 0.6.5

MultiModeModem (m³) is a fork of MadModem 0.6.5 by Lucian-Ioan Papadopol, IZ6NNH,
maintained by Michael Grigutsch, DO3BOX. Version numbers follow MadModem; the m³
build number counts on top. Source: https://github.com/mi-gri/MultiModeModem

## New in MultiModeModem

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
