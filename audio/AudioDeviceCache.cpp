#include "AudioDeviceCache.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QMutex>
#include <QMutexLocker>
#include <QThreadPool>

namespace AudioDeviceCache {
namespace {

struct Cache {
    QMutex mutex;
    bool inputsValid = false;
    bool outputsValid = false;
    QList<QAudioDeviceInfo> inputs;
    QList<QAudioDeviceInfo> outputs;
};

Cache &cache()
{
    static Cache instance;
    return instance;
}

QList<QAudioDeviceInfo> &listFor(Cache &c, QAudio::Mode mode, bool **valid)
{
    if (mode == QAudio::AudioInput) {
        *valid = &c.inputsValid;
        return c.inputs;
    }
    *valid = &c.outputsValid;
    return c.outputs;
}

} // namespace

QList<QAudioDeviceInfo> devices(QAudio::Mode mode, bool refresh)
{
    Cache &c = cache();
    QMutexLocker lock(&c.mutex);
    bool *valid = nullptr;
    QList<QAudioDeviceInfo> &list = listFor(c, mode, &valid);
    if (refresh || !*valid) {
        list = QAudioDeviceInfo::availableDevices(mode);
        *valid = true;
    }
    return list;
}

bool find(QAudio::Mode mode, const QString &deviceName, QAudioDeviceInfo *device)
{
    for (int pass = 0; pass < 2; ++pass) {
        const QList<QAudioDeviceInfo> list = devices(mode, pass == 1);
        for (const QAudioDeviceInfo &candidate : list) {
            if (candidate.deviceName() == deviceName) {
                if (device != nullptr) *device = candidate;
                return true;
            }
        }
    }
    return false;
}

void prefetch()
{
    QThreadPool::globalInstance()->start([] {
        devices(QAudio::AudioInput);
        devices(QAudio::AudioOutput);
    });
}

void invalidate()
{
    Cache &c = cache();
    QMutexLocker lock(&c.mutex);
    c.inputsValid = false;
    c.outputsValid = false;
}

} // namespace AudioDeviceCache
#endif
