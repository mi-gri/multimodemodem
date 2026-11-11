#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""A second instance on the same configuration exits; other configurations
and a lock left by a killed instance do not block the start."""
from __future__ import annotations

import os
import signal
import subprocess
import sys
import tempfile
import time
from pathlib import Path

ALREADY_RUNNING = 4


def wait_for(path: Path, proc: subprocess.Popen, seconds: float = 60.0) -> None:
    deadline = time.monotonic() + seconds
    while time.monotonic() < deadline:
        if path.exists():
            return
        if proc.poll() is not None:
            raise SystemExit(f"FAIL instance exited early ({proc.returncode}) before creating {path}")
        time.sleep(0.1)
    raise SystemExit(f"FAIL {path} not created within {seconds:.0f} s")


def main() -> int:
    binary = Path(sys.argv[1])
    with tempfile.TemporaryDirectory(prefix="m3-instance-lock-") as tmp:
        env = dict(os.environ, M3_CONFIG_DIR=tmp, QT_QPA_PLATFORM="offscreen")
        env.pop("DISPLAY", None)
        env.pop("WAYLAND_DISPLAY", None)

        def start(*args: str) -> subprocess.Popen:
            return subprocess.Popen([str(binary), *args], env=env, stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE, text=True)

        default_lock = Path(tmp) / "settings.mad.instance"
        other_lock = Path(tmp) / "configs" / "second.mad.instance"
        running = []
        try:
            first = start()
            running.append(first)
            wait_for(default_lock, first)
            # The instance must get past loading its settings (QSettings uses
            # its own settings.mad.lock; a clash once made it wait forever).
            wait_for(Path(tmp) / "settings.mad", first)

            second = start()
            try:
                out, err = second.communicate(timeout=30)
            except subprocess.TimeoutExpired:
                second.kill()
                raise SystemExit("FAIL second instance on the same configuration did not exit")
            if second.returncode != ALREADY_RUNNING or "already running" not in err:
                raise SystemExit(f"FAIL second instance: exit {second.returncode}, stderr {err!r}")
            if str(first.pid) not in err:
                raise SystemExit(f"FAIL message does not name the running process {first.pid}: {err!r}")
            print("PASS second instance on the same configuration exits with a message")

            other = start("--config", "second")
            running.append(other)
            wait_for(other_lock, other)
            time.sleep(1.0)
            if other.poll() is not None:
                raise SystemExit(f"FAIL instance with another configuration exited ({other.returncode})")
            print("PASS another configuration starts beside it")

            first.send_signal(signal.SIGKILL)
            first.wait(timeout=10)
            if not default_lock.exists():
                raise SystemExit("FAIL fixture: killed instance removed its lock")
            restarted = start()
            running.append(restarted)
            deadline = time.monotonic() + 30
            while time.monotonic() < deadline and restarted.poll() is None:
                time.sleep(0.2)
                if Path(f"/proc/{restarted.pid}").exists() and default_lock.read_text(errors="replace").startswith(str(restarted.pid)):
                    break
            if restarted.poll() is not None:
                raise SystemExit(f"FAIL lock of a killed instance blocked the start ({restarted.returncode})")
            print("PASS lock left by a killed instance is taken over")
            for proc in running:
                if proc.poll() is None:
                    proc.terminate()
                    proc.wait(timeout=10)
            running.clear()

            # The same folder reached through a symlink, before the settings
            # file exists: both starts must use the same lock.
            real = Path(tmp) / "real"
            real.mkdir()
            link = Path(tmp) / "link"
            link.symlink_to(real, target_is_directory=True)
            via_link = subprocess.Popen([str(binary)], env=dict(env, M3_CONFIG_DIR=str(link)),
                                        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            running.append(via_link)
            wait_for(real / "settings.mad.instance", via_link)
            direct = subprocess.Popen([str(binary)], env=dict(env, M3_CONFIG_DIR=str(real)),
                                      stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            try:
                direct.communicate(timeout=30)
            except subprocess.TimeoutExpired:
                direct.kill()
                raise SystemExit("FAIL second instance through the real path of a symlinked folder kept running")
            if direct.returncode != ALREADY_RUNNING:
                raise SystemExit(f"FAIL symlinked folder: exit {direct.returncode}")
            print("PASS a symlinked settings folder shares the lock")
        finally:
            for proc in running:
                if proc.poll() is None:
                    proc.terminate()
                    try:
                        proc.wait(timeout=10)
                    except subprocess.TimeoutExpired:
                        proc.kill()
                        proc.wait()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
