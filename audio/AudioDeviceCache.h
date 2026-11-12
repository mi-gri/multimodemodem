#pragma once

// Process-wide cache of the audio device lists (Qt 5).
//
// QAudioDeviceInfo::availableDevices() probes every device through the audio
// plugin and takes about a second or more. MultiModeModem used to call it on every
// RX start, every TX start (FT TX audio then began ~1.4 s into the slot) and
// whenever the Settings dialog opened. The list is now read once and reused;
// it is read again when the operator presses Refresh, or when a configured
// device is not in the cached list (device plugged in later).
// Thread-safe: RX/TX audio engines run in their own threads.

#include <QtGlobal>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QAudio>
#include <QAudioDeviceInfo>
#include <QList>
#include <QString>

namespace AudioDeviceCache {

QList<QAudioDeviceInfo> devices(QAudio::Mode mode, bool refresh = false);

// The device with this backend name; refreshes the list once when the name
// is not cached. Returns false (and leaves *device alone) when it is missing.
bool find(QAudio::Mode mode, const QString &deviceName, QAudioDeviceInfo *device);

void invalidate();

// Starts reading both device lists on a pool thread. Called at startup so the
// probe (about 1.5 s with PulseAudio) runs while the main window is built;
// devices() then waits for it under the cache mutex instead of probing again.
void prefetch();

} // namespace AudioDeviceCache
#endif
