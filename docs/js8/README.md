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
speed and hands each period to `Js8NativeEngine` on its own thread.  The
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

- **Band activity** shows the running text per audio offset; ✓ marks a
  completed message.  Double-click moves RX (and TX) to that offset.
- **Heard stations** lists calls from heartbeats, CQs and directed messages
  with SNR, grid and offset.  A click selects the station (field *To*) and
  fills the QSO form.
- **Messages** shows complete messages at the RX offset, messages to you
  (orange) and @ALLCALL traffic, plus your own transmissions (red).
- *Send* transmits the text directed to the selected station unless the text
  already starts with a call, @group, CQ or HEARTBEAT.  *CQ*, *HB* and *SNR?*
  send the standard messages.  STOP cancels an armed transmission.
- Left click in the waterfall sets the RX offset (and TX when "TX offset
  follows RX offset" is on); right click sets only TX.
- QSOs are logged with mode `JS8`.

Not implemented yet: automatic replies (SNR?, HEARING?, INFO?, message
store-and-forward), relays, inbox, decoding several speeds at once and
heartbeat/CQ repeat timers.
