// SPDX-License-Identifier: GPL-3.0-or-later
// Decodes WSPR-2 WAV files (12 kHz mono, from the even minute) with the m³
// engine and prints wsprd-style lines; one hash table across all files.
#include "audio/WavFileReader.h"
#include "modems/wspr/WsprEngine.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QFile>
#include <QFileInfo>

#include <cstdio>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    M3Wspr::Engine engine;
    M3Wspr::DecodeOptions options;
    QElapsedTimer total;
    total.start();
    int files = 0, decodes = 0;
    for (int a = 1; a < argc; ++a) {
        QFile file(QString::fromLocal8Bit(argv[a]));
        if (!file.open(QIODevice::ReadOnly)) continue;
        M3Audio::WavFileFormat format;
        QString error;
        if (!M3Audio::parseWavHeader(file, format, error) || format.sampleRate != 12000) {
            std::fprintf(stderr, "%s: %s\n", argv[a], qPrintable(error.isEmpty() ? QStringLiteral("not 12 kHz") : error));
            continue;
        }
        file.seek(format.dataOffset);
        const QVector<float> samples = M3Audio::convertWavBytesToMono(file.read(format.dataSize), format);
        if (samples.isEmpty()) continue;
        ++files;
        const QString stamp = QFileInfo(file).completeBaseName().right(4);
        for (const M3Wspr::Decode &d : engine.decode(samples.constData(), static_cast<size_t>(samples.size()), options)) {
            std::printf("%4s %3.0f %4.1f %10.6f %2d  %-s \n", qPrintable(stamp), d.snrDb, d.dtSeconds,
                        d.audioHz / 1e6, static_cast<int>(d.drift), d.message.text.c_str());
            ++decodes;
        }
        std::printf("<DecodeFinished>\n");
    }
    std::fprintf(stderr, "%d file(s), %d decode(s), %.1f s\n", files, decodes, total.elapsed() / 1000.0);
    return 0;
}
