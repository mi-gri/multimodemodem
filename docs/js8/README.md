# JS8 (JS8Call 2.2 protocol)

MultiModeModem receives and transmits JS8 in the four JS8Call speeds:

| Speed  | Period | Bandwidth | Start delay | 50 % decodes (AWGN, 2500 Hz) |
|--------|--------|-----------|-------------|------------------------------|
| Normal | 15 s   | 50 Hz     | 0.5 s       | −21 dB                       |
| Fast   | 10 s   | 80 Hz     | 0.2 s       | −18.5 dB                     |
| Turbo  | 6 s    | 160 Hz    | 0.1 s       | −15 dB                       |
| Slow   | 30 s   | 25 Hz     | 0.5 s       | −23 dB                       |

Measured with `m3_js8_regression --sweep N` (one heartbeat frame, ten noise
seeds per level, decode depth Deep).

## Protocol

A frame is 79 symbols of 8-FSK (three 7×7 Costas arrays, 58 data symbols)
with tone spacing equal to the keying rate.  It carries 12 characters of a
64-symbol alphabet (72 bits) plus three transmission-type bits
(first/last/data frame), protected by CRC12 and an LDPC(174,87) code.  The
message layer (directed commands, heartbeats, CQ, compound calls, Huffman and
JSC-compressed free text) is JS8Call's own `Varicode`/`JSC` code in
`third_party/js8call_gpl`.

## Receive path

`Js8Decoder` cuts the 12 kHz audio at UTC period boundaries of the selected
speed (with *Decode all speeds*: of all four speeds, each with its own period
grid) and hands each period to `Js8NativeEngine` on its own thread.  The
engine is a C++ port of JS8Call's Fortran receiver: Costas sync search over
the whole band, per-candidate downsampling, ±¼ symbol and ±2.5 Hz fine sync,
soft bit metrics, belief propagation, optional ordered-statistics decoding,
CRC check and signal subtraction for further passes.  Decode depth Fast is one
pass, Normal adds subtraction passes, Deep adds OSD.

On identical test WAVs the port decodes at least as weak signals as the
`js8` decoder of JS8Call 2.2.0 (Normal at −20 dB: 9/10 against 1/10; Turbo at
−14 dB: 9/10 against 6/10), and that decoder reads the frames
MultiModeModem transmits (checked for all four speeds).

## Transmit path

`Js8Message::buildFrames` splits the text exactly as JS8Call does.
`Js8Transmitter` produces one continuous waveform with frame n starting at
n × period + start delay after the first UTC boundary, so a message of n
frames occupies n consecutive periods.  It uses the shared weak-signal UTC
scheduler; there is no even/odd sequence in JS8.

## Operating

- **Frequency**: the band selector tunes a CAT-connected rig to the
  preferred JS8 frequency of the band from *Settings → Frequencies*; the CAT
  readout above it shows the dial frequency (mouse wheel over a digit tunes,
  double-click enters a frequency).  A CAT QSY to another band selects the
  band.
- **Band activity** shows the running text per audio offset and speed
  (A Normal, B Fast, C Turbo, E Slow; other speeds in grey); ✓ marks a
  completed message.  Double-click moves RX (and TX) to that offset and
  selects its speed.
- **Heard stations** lists calls from heartbeats, CQs and directed messages
  with SNR, grid and offset.  A click selects the station (field *To*) and
  fills the QSO form.
- **Messages** shows complete messages at the RX offset, messages to you
  (orange) and @ALLCALL traffic, plus your own transmissions (red, automatic
  ones marked AUTO).
- *Send* transmits the text directed to the selected station unless the text
  already starts with a call, @group, CQ or HEARTBEAT.  *CQ*, *HB* and *SNR?*
  send the standard messages.  STOP cancels an armed transmission and empties
  the automatic queue.
- **Macros**: the bar below the message line holds nine JS8 macros
  (Settings → User / QTH / Macros, section *JS8 macros*; defaults HW CPY?,
  SNR {RST}, QSL?, NAME/QTH, GRID?, INFO?, HEARING?, QUERY MSGS, 73).  A click
  sends the macro to the station in *To*, Shift+click only puts it into the
  message line.  Tokens as for the other modes; {CALL} is the station in
  *To*, {MYGRID} the four-character locator.
- Left click in the waterfall sets the RX offset (and TX when "TX offset
  follows RX offset" is on); right click sets only TX.  The waterfall marks
  the period starts of the selected speed.
- QSOs are logged with mode `JS8`.
- With *Report received stations to PSK Reporter* (Settings → Logbook) every
  station heard is spotted as mode `JS8`, with its grid when known, as JS8Call
  does.  The same option also spots MSK144/Q65 (stations with locator) and
  RTTY, PSK, MFSK and CW (a call sent twice after CQ or DE, without SNR).

## Frequencies

*Settings → Frequencies* holds the working-frequency list of all modes, as
the Frequencies tab of WSJT-X/JTDX: mode, dial frequency, IARU region,
description and a *preferred* mark per band.  The preferred entry of a band
(else the first one) is used by

- the band selectors of FT8/FT4, JS8, MSK144 and Q65,
- a mode change (option, on by default): the rig is tuned to the new mode's
  frequency on the band it is on; weather fax goes to its preferred station
  unless the rig already sits on a listed station,
- the band scheduler (*Use the frequency list entry for the band*).

FT4 falls back to FT8 on bands without an FT4 entry.  Defaults:

| Mode | Source |
|------|--------|
| FT8, FT4, MSK144, Q65 | WSJT-X 3.0 default list |
| JS8 | JS8Call 2.2.0 (1.842, 3.578, 7.078, 10.130, 14.078, 18.104, 21.078, 24.922, 28.078, 50.318, 144.178 MHz) |
| CW | IARU Region 1 HF band plan 2020, QRP centres of activity (30 m: customary 10.116), 6/2 m calling |
| Feld Hell | Feld Hell Club frequency list |
| PSK31, RTTY, MFSK, SSTV | customary calling/activity frequencies (40 m PSK and SSTV per region; SSTV 80/40 m in LSB) |
| Weather fax | DWD Hamburg/Pinneberg DDH3 3855.0, DDK3 7880.0, DDK6 13882.5 kHz (dial USB 1.9 kHz lower) |

*Restore defaults* reloads them.  An unchanged list is stored as "defaults",
so corrections of the built-in list reach existing installations.

## Directed commands and automatic operation

Frames of one transmission are collected per offset (`Js8CommandBuffer`, as
JS8Call's message buffer): compound calls are resolved and the buffered
commands (MSG, MSG TO:, QUERY, relay `>`, GRID, CMD) are checked against their
CRC-16 before they count.  `Js8AutoReply` then applies JS8Call's rules:

| Received (to you)         | Answer                                  |
|---------------------------|-----------------------------------------|
| `SNR?`                    | `CALL SNR -12`                          |
| `GRID?`                   | `CALL GRID JO31LK`                      |
| `INFO?` / `STATUS?`       | `CALL INFO …` / `CALL STATUS …` (if set)|
| `HEARING?`                | up to four stations heard in the last hour |
| `AGN?`                    | your last transmission again            |
| `MSG text`                | stored in the inbox, `CALL ACK`         |
| `MSG TO:CALL2 text`       | kept for CALL2, `CALL ACK`              |
| `QUERY MSGS`              | `CALL YES MSG ID n` or `CALL NO`        |
| `QUERY MSG n`             | `CALL MSG text FROM ORIGIN` (only to the addressee) |
| `QUERY CALL CALL2`        | `CALL YES +05 (12m)` if CALL2 was heard |
| `CALL2>text` (relay)      | forwarded as `CALL2>text *DE* SENDER`   |
| heartbeat (with HB ACK)   | `CALL HEARTBEAT SNR -12 [MSG ID n]`     |

- **AUTO** sends these answers by itself in the next free period.  Without
  AUTO nothing is sent automatically: a received MSG is still stored, and the
  answer to a question addressed to you is put into the message field for
  *Send*.  @ALLCALL questions and heartbeats are answered only with AUTO, at
  most once per station in 15 minutes.  Directed questions get at most four
  automatic answers per station in 10 minutes; further ones are only logged,
  since callsigns are not authenticated.
- **Relay and store messages** is off by default: relays (`>`) and
  `MSG TO:` messages for other stations are then ignored.
- **Heartbeat every / CQ every** repeat a heartbeat (on a free offset between
  500 and 1000 Hz) or CQ (on the TX offset); they pause while a station is
  selected.
- The **idle watchdog** stops all automatic transmissions after the set time
  without operator action; any action (send, select, settings) resumes them.
- The **inbox** (button under the messages) lists received messages and the
  ones kept for others (file `js8_inbox.json` next to the settings).  Opening
  it marks new messages read; *Reply* addresses the sender.

Not implemented: @group membership (only @ALLCALL and @HB are recognised),
APRS/JS8NET spotting and the JS8Call TCP/UDP API.
