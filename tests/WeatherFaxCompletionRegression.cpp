// WEFAX: completing an image inside line processing (line limit, APT stop,
// end of signal) must not touch the cleared sample buffers.  It removed one
// sample from an empty QVector: heap corruption, bad_alloc crash in m³ after
// 3000 free-running lines.  Built with QT_FORCE_ASSERTS so QVector checks it.
#include "modems/weatherfax/WeatherFaxDecoder.h"
#include "audio/AudioBlock.h"

#include <QCoreApplication>
#include <QImage>
#include <cmath>
#include <cstdio>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    int fails = 0;
    auto check = [&](bool ok, const char *name) { if (!ok) ++fails; std::printf("%s %s\n", ok ? "PASS" : "FAIL", name); };

    WeatherFaxDecoder d;
    d.setLpm(120);
    d.setAutoStartEnabled(false);      // free run, as in the crash
    d.setTargetImageLines(256);        // smallest safety limit: reached after ~128 s
    int completed = 0;
    QImage completedImage;
    QObject::connect(&d, &WeatherFaxDecoder::imageCompleted, [&](const QImage &img, const QString &) { ++completed; completedImage = img; });

    const int rate = 48000;
    double phase = 0.0;
    quint64 index = 0;
    bool snapshotChecked = false;
    for (qint64 n = 0; n < qint64(rate) * 160; n += 1024) {
        AudioBlock block;
        block.sampleRate = rate;
        block.firstSampleIndex = index;
        block.samples.resize(1024);
        for (int i = 0; i < 1024; ++i) {
            const double f = 1500.0 + 400.0 * std::sin(double(n + i) * 1e-4);
            phase += 2.0 * M_PI * f / rate;
            block.samples[i] = 0.3f * float(std::sin(phase));
        }
        index += 1024;
        d.processAudioBlock(block);
        if (!snapshotChecked && n > rate * 30) {
            // Two snapshots without a new line share one copy.
            const QImage a = d.currentImage(), b = d.currentImage();
            check(!a.isNull() && a.cacheKey() == b.cacheKey(), "snapshot is copied once per line, not per call");
            snapshotChecked = true;
        }
    }
    check(completed == 1 && completedImage.height() == 256, "image completed at the 256-line limit");
    check(true, "no out-of-range buffer access after completion (QT_FORCE_ASSERTS)");
    std::printf("%s\n", fails ? "RESULT FAIL" : "RESULT PASS");
    return fails;
}
