# MultiModeModem (m³) changelog

MultiModeModem (m³) is based on MadModem by IZ6NNH; the MadModem entries follow below.

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
