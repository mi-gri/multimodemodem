#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Build a busy simulated FT8 band (12 kHz, 16 bit, 15 s WAV) for decoder
speed and sensitivity measurements.

Each signal comes from WSJT-X ft8sim (must be on PATH) without noise, is
scaled to a random SNR (-20 .. +5 dB in 2500 Hz) and summed; white noise is
added once.  Frequencies are 55 Hz apart from 200 Hz, DT is -0.3 .. 1.2 s and
the messages are CQs, grids, reports, RR73 and 73 between random calls.
Next to OUT.wav, OUT.wav.truth lists "f0 DT SNR message" per signal.

Example (m3 receiver, with and without the R30 recovery):
  tools/ft8_sim_band.py 50 build/decode-bench/band50.wav --seed 50
  python3 tools/run_ft_replay_probe.py --build build --compile-only
  python3 tools/run_ft_replay_probe.py --build build --native-only --compile-only
  PROBE_OFFLINE=1 QT_QPA_PLATFORM=offscreen build/ft-replay-r30/probe build/decode-bench/band50.wav
  PROBE_OFFLINE=1 QT_QPA_PLATFORM=offscreen build/ft-replay-native/probe build/decode-bench/band50.wav
WSJT-X for comparison: jt9 -8 -d 3 band50.wav
"""
import argparse
import array
import math
import os
import random
import shutil
import subprocess
import tempfile
import wave

SAMPLES = 180000          # 15 s at 12 kHz
TX_SECONDS = 12.64        # FT8 transmission length


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('signals', type=int, help='number of signals (at most 50)')
    parser.add_argument('out', help='output WAV file')
    parser.add_argument('--seed', type=int, default=1, help='random seed (same seed, same band)')
    parser.add_argument('--snr-min', type=float, default=-20.0)
    parser.add_argument('--snr-max', type=float, default=5.0)
    args = parser.parse_args()
    if shutil.which('ft8sim') is None:
        raise SystemExit('ft8sim (WSJT-X) not found on PATH')
    slots = list(range(200, 2900, 55))
    if not 1 <= args.signals <= len(slots):
        raise SystemExit(f'signals must be 1..{len(slots)}')

    rng = random.Random(args.seed)
    letters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

    def call() -> str:
        prefix = rng.choice(['DL', 'DK', 'G', 'F', 'I', 'OH', 'SP', 'EA', 'K', 'W', 'VK', 'JA',
                             'LZ', 'YO', 'OK', 'PA', 'ON', 'HB9', 'OE'])
        return prefix + str(rng.randint(1, 9)) + ''.join(rng.choice(letters) for _ in range(rng.randint(2, 3)))

    def grid() -> str:
        return rng.choice('IJKLMNOEF') + rng.choice('KLMNOP') + str(rng.randint(0, 9)) + str(rng.randint(0, 9))

    def message() -> str:
        a, b = call(), call()
        return rng.choice([f'CQ {a} {grid()}', f'{a} {b} {grid()}', f'{a} {b} -{rng.randint(1, 24):02d}',
                           f'{a} {b} R-{rng.randint(1, 24):02d}', f'{a} {b} RR73', f'{a} {b} 73'])

    mix = [0.0] * SAMPLES
    truth = []
    with tempfile.TemporaryDirectory(prefix='ft8-sim-band-') as tmp:
        for index, f0 in enumerate(sorted(rng.sample(slots, args.signals))):
            text = message()
            snr = rng.uniform(args.snr_min, args.snr_max)
            dt = rng.uniform(-0.3, 1.2)
            work = os.path.join(tmp, str(index))
            os.makedirs(work)
            # snr 99 = no noise; the level is set below.
            subprocess.run(['ft8sim', text, str(f0), f'{dt:.2f}', '0', '0', '1', '99'], cwd=work,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, check=True)
            with wave.open(os.path.join(work, '000000_000001.wav')) as w:
                x = array.array('h', w.readframes(w.getnframes()))
            power = sum(v * v for v in x) / (TX_SECONDS * 12000)
            # Unit-variance noise has 2500/6000 of its power in 2500 Hz.
            scale = math.sqrt(10 ** (snr / 10) * 2500 / 6000 / power) if power > 0 else 0.0
            for k in range(min(SAMPLES, len(x))):
                mix[k] += x[k] * scale
            truth.append(f'{f0} {dt:.2f} {snr:.1f} {text}')
    noise = random.Random(args.seed + 1000)
    for k in range(SAMPLES):
        mix[k] += noise.gauss(0.0, 1.0)
    gain = 0.5 * 32767 / max(abs(v) for v in mix)
    with wave.open(args.out, 'wb') as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(12000)
        w.writeframes(array.array('h', [int(v * gain) for v in mix]).tobytes())
    with open(args.out + '.truth', 'w') as f:
        f.write('\n'.join(truth) + '\n')
    print(f'{args.out}: {args.signals} signals')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
