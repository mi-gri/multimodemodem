#include "DspEngine.h"

#include "FftwPlannerLock.h"

#include <fftw3.h>

#include <mutex>

#include <QDateTime>
#include <QtMath>

#include <algorithm>
#include <utility>
#include <vector>

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------

// FFTW r2c plan of one length; the waterfall used an in-house radix-2 FFT
// that was 13-16 times slower (Celeron N5100: 65536 points 11.9 ms vs
// 0.76 ms).  Plans are made under the shared planner lock.
struct DspEngine::FftPlan {
    explicit FftPlan(int n)
        : size(n), input(fftw_alloc_real(static_cast<size_t>(n))),
          output(fftw_alloc_complex(static_cast<size_t>(n / 2 + 1)))
    {
        std::lock_guard<std::mutex> lock(M3Dsp::fftwPlannerMutex);
        plan = fftw_plan_dft_r2c_1d(n, input, output, FFTW_ESTIMATE);
    }
    ~FftPlan()
    {
        if (plan != nullptr) {
            std::lock_guard<std::mutex> lock(M3Dsp::fftwPlannerMutex);
            fftw_destroy_plan(plan);
        }
        fftw_free(input);
        fftw_free(output);
    }
    FftPlan(const FftPlan &) = delete;
    FftPlan &operator=(const FftPlan &) = delete;
    int size = 0;
    double *input = nullptr;
    fftw_complex *output = nullptr;
    fftw_plan plan = nullptr;
};

DspEngine::~DspEngine()
{
    delete m_fftPlan;
}

DspEngine::DspEngine(QObject *parent)
    : QObject(parent)
{
}

// -----------------------------------------------------------------------------
// Public slots
// -----------------------------------------------------------------------------

void DspEngine::processAudioBlock(const AudioBlock &block)
{
    if (block.samples.isEmpty() || block.sampleRate <= 0) {
        return;
    }

    if (m_continuity.accept(block)) {
        m_fifo.clear();
        m_waterfallLeveler.reset();
        resetRowAverage();
    }
    updateFftSize(block.sampleRate);
    if (m_fifo.isEmpty()) {
        m_fifoStartUtcNs = block.firstSampleUtcNs;   // 0: the producer has no capture time
    }
    m_fifo += block.samples;
    m_windowScratch.resize(m_fftSize);
    const double nsPerSample = 1.0e9 / static_cast<double>(block.sampleRate);
    const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
    int consumed = 0;
    while (m_fifo.size() - consumed >= m_fftSize) {
        std::copy_n(m_fifo.constBegin() + consumed, m_fftSize, m_windowScratch.begin());
        // UTC of the window centre: from the capture time where there is one,
        // else estimated from the clock and what is still buffered.
        const double centreOffset = static_cast<double>(consumed) + m_fftSize / 2.0;
        m_windowCentreUtcMs = m_fifoStartUtcNs > 0
            ? (m_fifoStartUtcNs + static_cast<qint64>(centreOffset * nsPerSample)) / 1000000
            : nowMs - static_cast<qint64>((static_cast<double>(m_fifo.size()) - centreOffset) * nsPerSample / 1.0e6);
        analyzeWindow(m_windowScratch, block.sampleRate);
        consumed += m_hopSize;
    }
    if (consumed > 0) {
        m_fifo.remove(0, consumed);
        if (m_fifoStartUtcNs > 0) m_fifoStartUtcNs += static_cast<qint64>(consumed * nsPerSample);
    }
}

void DspEngine::reset()
{
    m_fifo.clear();
    m_continuity.reset();
    m_waterfallLeveler.reset();
    resetRowAverage();
}

void DspEngine::setTargetRowSeconds(double seconds)
{
    m_targetRowSeconds = (seconds > 0.0 && qIsFinite(seconds)) ? seconds : 0.0;
    resetRowAverage();
}

void DspEngine::setColumns(int columns)
{
    columns = qBound(256, columns, 8192);
    if (columns == m_columns) {
        return;
    }
    m_columns = columns;
    m_waterfallLeveler.reset();
    resetRowAverage();
}

void DspEngine::setDisplayRange(double minHz, double maxHz)
{
    if (!qIsFinite(minHz) || !qIsFinite(maxHz)) return;
    minHz = qBound(10.0, minHz, 5000.0);
    maxHz = qBound(minHz + 100.0, maxHz, 6000.0);
    if (qAbs(minHz - m_minHz) < 1.0e-6 && qAbs(maxHz - m_maxHz) < 1.0e-6) return;
    m_minHz = minHz;
    m_maxHz = maxHz;
    resetRowAverage();
}

void DspEngine::setHopSamples(int samples)
{
    samples = qBound(512, samples, 16384);
    if (samples == m_hopSize) return;
    m_hopSize = samples;
    resetRowAverage();
}

void DspEngine::resetRowAverage()
{
    m_rowPowerSum.clear();
    m_rowWindows = 0;
}

void DspEngine::setTargetBinHz(double binHz)
{
    m_targetBinHz = (binHz > 0.0 && qIsFinite(binHz)) ? binHz : 0.0;
    m_fftSizeSampleRate = 0; // re-evaluated with the next audio block
}

void DspEngine::updateFftSize(int sampleRate)
{
    if (sampleRate == m_fftSizeSampleRate) {
        return;
    }
    m_fftSizeSampleRate = sampleRate;

    int size = kDefaultFftSize;
    if (m_targetBinHz > 0.0 && sampleRate > 0) {
        const double wanted = static_cast<double>(sampleRate) / m_targetBinHz;
        size = 4096;
        // closest power of two on a log scale
        while (size < 65536 && static_cast<double>(size) * qSqrt(2.0) < wanted) {
            size *= 2;
        }
    }
    if (size == m_fftSize) {
        return;
    }
    m_fftSize = size;
    // Keep only what one new window needs, so a shorter FFT does not
    // analyse a burst of stale overlapping windows at once.
    if (m_fifo.size() > m_fftSize) {
        const int dropped = m_fifo.size() - m_fftSize;
        m_fifo.remove(0, dropped);
        if (m_fifoStartUtcNs > 0 && sampleRate > 0) {
            m_fifoStartUtcNs += static_cast<qint64>(dropped * (1.0e9 / sampleRate));
        }
    }
    m_waterfallLeveler.reset();
    resetRowAverage();
}

void DspEngine::configureSignalPeakMetric(bool enabled, int lowHz, int highHz)
{
    m_signalPeakMetricEnabled = enabled;
    m_signalPeakLowHz = qMax(0, qMin(lowHz, highHz));
    m_signalPeakHighHz = qMax(m_signalPeakLowHz, qMax(lowHz, highHz));
}

void DspEngine::configureReceiverFilter(const QString &modeName, int widthHz, int source)
{
    Q_UNUSED(modeName);
    // A backend value is a hint only, never an audio-frequency centre or a
    // hard crop. Spectrum evidence remains necessary before correction.
    m_receiverWidthHz = source > 0 && widthHz >= 50 && widthHz <= 12000 ? widthHz : 0;
    m_receiverFilterAge.start();
}

// -----------------------------------------------------------------------------
// Analysis
// -----------------------------------------------------------------------------

void DspEngine::analyzeWindow(const QVector<float> &window, int sampleRate)
{
    ensureWindowTable();

    if (m_fftPlan == nullptr || m_fftPlan->size != m_fftSize) {
        delete m_fftPlan;
        m_fftPlan = new FftPlan(m_fftSize);
    }
    double *input = m_fftPlan->input;
    for (int i = 0; i < m_fftSize; ++i) {
        input[i] = static_cast<double>(window[i]) * m_windowTable[i];
    }
    computeMagnitudes();

    const int maxBin = (m_fftSize / 2) - 2;
    auto &magnitudes = m_magnitudeScratch;

    const double log10 = qLn(10.0);
    const double binsPerHz = static_cast<double>(m_fftSize) / static_cast<double>(sampleRate);
    const double columnHz = (m_maxHz - m_minHz) / static_cast<double>(m_columns - 1);
    const double binsPerColumn = columnHz * binsPerHz;

    // Magnitude shown in waterfall column x, from a spectrum of magnitudes.
    auto columnMagnitude = [&](const QVector<double> &mags, int x, double *freqOut) {
        const double ratio = static_cast<double>(x) / static_cast<double>(m_columns - 1);
        const double freq = m_minHz + ratio * (m_maxHz - m_minHz);
        if (freqOut != nullptr) *freqOut = freq;
        const double binPosition = freq * binsPerHz;
        double mag = 0.0;
        if (binsPerColumn > 1.0) {
            // A column spans several FFT bins: show the strongest of them.
            // Interpolating between two bins would skip the others, and a
            // narrow signal falling between them would vanish from the row.
            const int first = qBound(1, static_cast<int>(qCeil(binPosition - binsPerColumn / 2.0)), maxBin + 1);
            const int last = qBound(first, static_cast<int>(qFloor(binPosition + binsPerColumn / 2.0)), maxBin + 1);
            for (int bin = first; bin <= last; ++bin) {
                mag = qMax(mag, mags[bin]);
            }
        } else {
            const int bin0 = qBound(1, static_cast<int>(qFloor(binPosition)), maxBin);
            const int bin1 = qBound(1, bin0 + 1, maxBin + 1);
            const double frac = qBound(0.0, binPosition - static_cast<double>(bin0), 1.0);
            mag = ((1.0 - frac) * mags[bin0]) + (frac * mags[bin1]);
        }
        return mag;
    };

    // The strongest frequency of this window (not of the averaged row).
    double bestDb = -200.0;
    double bestFrequency = 0.0;
    for (int x = 0; x < m_columns; ++x) {
        double freq = 0.0;
        const double db = 20.0 * qLn(qMax(columnMagnitude(magnitudes, x, &freq), 1.0e-12)) / log10;
        if (db > bestDb) {
            bestDb = db;
            bestFrequency = freq;
        }
    }

    // A waterfall row is the power average of rowWindows consecutive windows,
    // as JTDX/WSJT-X average their spectra ("N Avg"): one row per ~0.3 s in
    // FT8, so several 15-s periods fit on the screen and the noise is calmer.
    const int rowWindows = m_targetRowSeconds > 0.0
        ? qBound(1, qRound(m_targetRowSeconds * sampleRate / static_cast<double>(m_hopSize)), 64)
        : 1;
    if (m_rowPowerSum.size() != magnitudes.size()) {
        m_rowPowerSum.fill(0.0, magnitudes.size());
        m_rowWindows = 0;
    }
    for (int bin = 0; bin < magnitudes.size(); ++bin) {
        m_rowPowerSum[bin] += magnitudes[bin] * magnitudes[bin];
    }
    ++m_rowWindows;
    if (m_rowWindows >= rowWindows) {
        const QVector<double> *rowMags = &magnitudes;
        QVector<double> averaged;
        if (m_rowWindows > 1) {
            averaged.resize(magnitudes.size());
            for (int bin = 0; bin < magnitudes.size(); ++bin) {
                averaged[bin] = qSqrt(m_rowPowerSum[bin] / m_rowWindows);
            }
            rowMags = &averaged;
        }
        const double lineSeconds = static_cast<double>(m_hopSize) * m_rowWindows /
                                   static_cast<double>(sampleRate);
        m_rowPowerSum.fill(0.0);
        m_rowWindows = 0;

        QVector<double> dbLine;
        dbLine.resize(m_columns);
        QVector<quint8> waterfallLine;
        waterfallLine.resize(m_columns);
        for (int x = 0; x < m_columns; ++x) {
            dbLine[x] = 20.0 * qLn(qMax(columnMagnitude(*rowMags, x, nullptr), 1.0e-12)) / log10;
        }

        /*
         * Display-only WSJT-X-style flattening.
         *
         * WSJT-X Wide Graph does not let a slow temporal AGC chase the absolute
         * receiver level.  Its default Flatten path converts each row to dB,
         * estimates a lower-envelope polynomial from the lowest 10% of each of
         * ten frequency segments, then subtracts that baseline before applying a
         * fixed gain/zero colour mapping.  This reacts immediately to receiver
         * AGC steps without leaving the whole passband orange for many seconds.
         *
         * Narrow receiver noise shelves receive a local lower-envelope correction.
         * No frequency bins are cropped, and decoder audio is untouched.
         */
        std::vector<double> levelInput;
        levelInput.reserve(static_cast<std::size_t>(dbLine.size()));
        for (double value : dbLine) levelInput.push_back(value);
        const int receiverWidth = m_receiverFilterAge.isValid() && m_receiverFilterAge.elapsed() < 45000 ? m_receiverWidthHz : 0;
        const WaterfallLevelResult levels =
            m_waterfallLeveler.update(levelInput, lineSeconds, columnHz,
                receiverWidth, m_narrowBandDisplay || (receiverWidth >= 50 && receiverWidth <= 1000));
        const bool haveBaseline = levels.baselineDb.size() ==
                                  static_cast<std::size_t>(dbLine.size());

        for (int x = 0; x < m_columns; ++x) {
            const double baselineDb = haveBaseline
                ? levels.baselineDb[static_cast<std::size_t>(x)]
                : levels.floorDb;
            const double flattenedDb = dbLine[x] - baselineDb;

            // Equivalent to the WSJT-X Wide Graph default transfer:
            //     y1 = 10 * gain * flattened_dB + zero
            // The user colour-scale control supplies the gain in WaterfallWidget;
            // zero remains 0 so the lower envelope maps to black/dark blue.
            const double rawValue = 10.0 * flattenedDb;
            const int value = qBound(0, static_cast<int>(qRound(rawValue)), 254);

            waterfallLine[x] = static_cast<quint8>(value);
        }

        emit waterfallRowTime(m_windowCentreUtcMs);
        emit waterfallLineReady(waterfallLine, m_minHz, m_maxHz);
    }

    // QSO signal peak metric. Reuse the FFT that was already required for the
    // waterfall instead of running Welch/FFT work in MainWindow. The metric is
    // intentionally small: average power in the selected signal band versus
    // equal-width adjacent noise bands, with a guard gap around the signal.
    if (m_signalPeakMetricEnabled && m_signalPeakHighHz > m_signalPeakLowHz) {
        const int nyquist = sampleRate / 2;
        const int lowHz = qBound(20, m_signalPeakLowHz, qMax(20, nyquist - 20));
        const int highHz = qBound(lowHz + 10, m_signalPeakHighHz, qMax(lowHz + 10, nyquist - 10));
        const int widthHz = qMax(20, highHz - lowHz);
        const int guardHz = qMax(30, widthHz / 3);

        auto meanBandPower = [&](int bandLowHz, int bandHighHz, bool *ok) -> double {
            if (ok != nullptr) *ok = false;
            bandLowHz = qBound(1, bandLowHz, nyquist - 1);
            bandHighHz = qBound(bandLowHz + 1, bandHighHz, nyquist);
            const int firstBin = qBound(1, static_cast<int>(qCeil(
                static_cast<double>(bandLowHz) * m_fftSize / sampleRate)), maxBin);
            const int lastBin = qBound(firstBin, static_cast<int>(qFloor(
                static_cast<double>(bandHighHz) * m_fftSize / sampleRate)), maxBin);
            if (lastBin < firstBin) return 0.0;
            double sum = 0.0;
            int count = 0;
            for (int bin = firstBin; bin <= lastBin; ++bin) {
                const double mag = magnitudes.at(bin);
                sum += mag * mag;
                ++count;
            }
            if (count <= 0 || !(sum > 0.0) || !qIsFinite(sum)) return 0.0;
            if (ok != nullptr) *ok = true;
            return sum / static_cast<double>(count);
        };

        bool signalOk = false;
        const double signalPower = meanBandPower(lowHz, highHz, &signalOk);
        QVector<double> noisePowers;
        bool noiseOk = false;
        const int leftHigh = lowHz - guardHz;
        const int leftLow = leftHigh - widthHz;
        if (leftLow >= 20) {
            const double p = meanBandPower(leftLow, leftHigh, &noiseOk);
            if (noiseOk) noisePowers.append(p);
        }
        const int rightLow = highHz + guardHz;
        const int rightHigh = rightLow + widthHz;
        if (rightHigh <= nyquist - 10) {
            const double p = meanBandPower(rightLow, rightHigh, &noiseOk);
            if (noiseOk) noisePowers.append(p);
        }

        if (signalOk && !noisePowers.isEmpty()) {
            double noisePower = 0.0;
            for (double p : std::as_const(noisePowers)) noisePower += p;
            noisePower /= static_cast<double>(noisePowers.size());
            if (noisePower > 0.0 && qIsFinite(noisePower)) {
                emit signalPeakMetricReady(10.0 * qLn(qMax(1.0e-12, signalPower / noisePower)) / qLn(10.0), true);
            } else {
                emit signalPeakMetricReady(-99.0, false);
            }
        } else {
            emit signalPeakMetricReady(-99.0, false);
        }
    }

    if (bestDb > -92.0) {
        emit dominantFrequencyChanged(bestFrequency, bestDb);
    } else {
        emit dominantFrequencyChanged(0.0, bestDb);
    }
}

void DspEngine::ensureWindowTable()
{
    if (m_windowTable.size() == m_fftSize) {
        return;
    }

    m_windowTable.resize(m_fftSize);

    for (int i = 0; i < m_fftSize; ++i) {
        const double phase = (2.0 * M_PI * i) /
                             static_cast<double>(m_fftSize - 1);

        /*
         * Four-term Blackman-Harris window.  It has a wider main lobe than
         * Hann, but far lower sidelobes, so strong WEFAX/SSTV tones leak much
         * less into nearby waterfall bins and weak signals are easier to see.
         */
        m_windowTable[i] =
            0.35875 -
            (0.48829 * qCos(phase)) +
            (0.14128 * qCos(2.0 * phase)) -
            (0.01168 * qCos(3.0 * phase));
    }
}

void DspEngine::computeMagnitudes()
{
    FftPlan &p = *m_fftPlan;
    if (p.plan != nullptr) {
        fftw_execute(p.plan);
    }
    const int bins = p.size / 2;
    m_magnitudeScratch.resize(bins);
    const double scale = 1.0 / static_cast<double>(p.size);
    for (int bin = 0; bin < bins; ++bin) {
        const double re = p.output[bin][0];
        const double im = p.output[bin][1];
        m_magnitudeScratch[bin] = qSqrt(re * re + im * im) * scale;
    }
}
