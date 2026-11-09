// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8NATIVEENGINE_H
#define JS8NATIVEENGINE_H

#include "Js8Mode.h"

#include <QString>
#include <QVector>
#include <memory>

/**
 * @brief Native JS8 demodulator/decoder for one UTC period.
 *
 * C++ port of the JS8Call 2.2.0 receive chain (js8?_decode.f90,
 * syncjs8.f90, js8_downsample.f90, syncjs8d.f90, js8dec.f90,
 * subtractjs8.f90, baselinejs8.f90).  Input is the period audio at
 * 12000 Hz, starting at the UTC boundary.  Deviations from the Fortran:
 * the fine-sync bounds use the real downsampled length (the original
 * hard-codes the Normal-mode size), the third Costas reference keeps its
 * own phase and the subtraction filter is zero-phase.  FT8 a-priori passes
 * are not used, as in JS8Call.
 */
class Js8NativeEngine
{
public:
    struct Configuration
    {
        Js8Mode::Submode submode = Js8Mode::Submode::Normal;
        int depth = 3;            // 1 BP only, 2 + subtraction, 3 + OSD
        int lowHz = 200;
        int highHz = 2900;
        int rxFrequencyHz = 1500; // decoded first, deeper OSD nearby
        int txFrequencyHz = 1500;
        int apWidthHz = 50;
        double syncMin = 1.5;
        int maxCandidates = 300;
    };

    struct Result
    {
        QString frame;            // 12 characters
        int bits = 0;             // transmission type bits (first/last/data)
        double frequencyHz = 0.0;
        double dtSeconds = 0.0;   // relative to the nominal start delay
        int snrDb = -28;
        double quality = 0.0;     // 1 - (hard errors + dmin)/60
        int hardErrors = 0;
        bool osd = false;
    };

    Js8NativeEngine();
    ~Js8NativeEngine();
    Js8NativeEngine(const Js8NativeEngine &) = delete;
    Js8NativeEngine &operator=(const Js8NativeEngine &) = delete;

    /// samples: period audio at 12 kHz, nominally scaled to [-1, 1].
    QVector<Result> decode(const QVector<float> &samples, const Configuration &config);

private:
    struct Impl;
    std::unique_ptr<Impl> d;
};

#endif // JS8NATIVEENGINE_H
