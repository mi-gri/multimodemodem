#ifndef DSPENGINE_H
#define DSPENGINE_H

#include "../audio/AudioBlock.h"
#include "WaterfallLeveler.h"
#include "../audio/AudioContinuity.h"

#include <QObject>
#include <QVector>
#include <QElapsedTimer>

/**
 * @brief Performs first-stage DSP analysis for the receiver.
 *
 * Purpose:
 * - Receive normalized mono audio blocks from AudioEngine.
 * - Compute FFT spectra for the diagnostic waterfall.
 * - Estimate the strongest audio frequency in the visible passband.
 * - Keep waterfall processing intentionally lighter than modem decoding.
 *
 * Performance note:
 * - The engine is designed to run in a worker QThread owned by MainWindow.
 * - The decoder path never depends on waterfall FFT density; this keeps
 *   WAV/offline analysis and live decoding responsive on modest PCs.
 */
class DspEngine : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Creates the DSP engine.
     */
    explicit DspEngine(QObject *parent = nullptr);
    ~DspEngine() override;

public slots:
    /**
     * @brief Processes one audio block from the audio engine.
     */
    void processAudioBlock(const AudioBlock &block);

    /**
     * @brief Clears the internal DSP buffers.
     */
    void reset();

    /**
     * @brief Enables/disables the low-cost QSO peak metric extracted from the
     *        already-computed waterfall FFT.
     *
     * This never performs a second FFT and therefore cannot add a heavy DSP
     * operation to the modem/UI receive path. The slot runs in the DSP worker
     * thread together with waterfall processing.
     */
    void configureSignalPeakMetric(bool enabled, int lowHz, int highHz);
    void configureReceiverFilter(const QString &modeName, int widthHz, int source);
    void setNarrowBandDisplay(bool enabled) { m_narrowBandDisplay = enabled; }

    /**
     * @brief Sets the frequency resolution the waterfall FFT aims at.
     *
     * The FFT length becomes the power of two closest to sampleRate / binHz
     * (4096 .. 65536), so the resolution does not depend on the capture rate.
     * 0 restores the default 8192-point FFT. A long FFT resolves the FT8
     * tones (6.25 Hz apart) the way JTDX shows them, but it also averages
     * over a long window, which would smear CW and RTTY characters, so it is
     * meant for the FT modes. The row rate (hop) is unchanged.
     */
    void setTargetBinHz(double binHz);

    /**
     * @brief Averages the power spectra of consecutive windows into one
     *        waterfall row of about this many seconds (JTDX "N Avg").
     *
     * 0 emits one row per window (hop), the default. The dominant-frequency
     * and peak metrics still follow every window.
     */
    void setTargetRowSeconds(double seconds);

    /**
     * @brief Number of waterfall columns (256 .. 8192); the widget asks for
     *        one per screen pixel, so no column is stretched or skipped.
     */
    void setColumns(int columns);

    /**
     * @brief Audio range of the waterfall row (default 100 .. 3000 Hz).
     *        WSPR shows 1200 .. 1800 Hz around its 1400 .. 1600 Hz band.
     */
    void setDisplayRange(double minHz, double maxHz);

    /**
     * @brief Step between analysis windows (default 2048 samples).  Slow
     *        displays with long FFTs (WSPR: 65536 points, one row per second)
     *        need far fewer windows.
     */
    void setHopSamples(int samples);

signals:
    /**
     * @brief Sends one normalized waterfall intensity line.
     */
    void waterfallLineReady(const QVector<quint8> &line, double minHz, double maxHz);

    /**
     * @brief UTC (ms) of the waterfall row that follows next; emitted just
     *        before waterfallLineReady, so period lines can be drawn on it.
     */
    void waterfallRowTime(qint64 utcMs);

    /**
     * @brief Sends the strongest detected frequency in the visible band.
     */
    void dominantFrequencyChanged(double frequencyHz, double levelDb);

    /**
     * @brief Sends a signal-to-adjacent-noise metric for mechanical QSO peak
     *        tracking. Computed from the existing DSP FFT in the worker thread.
     */
    void signalPeakMetricReady(double metricDb, bool valid);

private:
    /**
     * @brief Runs one FFT analysis window.
     */
    void analyzeWindow(const QVector<float> &window, int sampleRate);

    /**
     * @brief Prepares the reusable Hann analysis window.
     */
    void ensureWindowTable();

    /**
     * @brief Real FFT of m_fftInput (FFTW plan cached per length): fills
     *        m_magnitudeScratch with |X[k]| / N for k = 0 .. N/2 - 1.
     */
    void computeMagnitudes();
    struct FftPlan;
    FftPlan *m_fftPlan = nullptr;

private:
    AudioContinuity m_continuity;
    QVector<float> m_windowScratch;
    QVector<double> m_magnitudeScratch;
    QVector<float> m_fifo;
    QVector<double> m_windowTable;

    /**
     * @brief Chooses the FFT length for the sample rate and target resolution.
     */
    void updateFftSize(int sampleRate);

    static constexpr int kDefaultFftSize = 8192;
    int m_fftSize = kDefaultFftSize;
    int m_hopSize = 2048;
    double m_targetBinHz = 0.0;
    int m_fftSizeSampleRate = 0;

    void resetRowAverage();
    double m_targetRowSeconds = 0.0;
    QVector<double> m_rowPowerSum;
    int m_rowWindows = 0;
    qint64 m_fifoStartUtcNs = 0;      // capture UTC of m_fifo[0], 0 = unknown
    qint64 m_windowCentreUtcMs = 0;   // UTC of the centre of the window being analysed
    int m_columns = 1024;

    double m_minHz = 100.0;
    double m_maxHz = 3000.0;

    WaterfallLeveler m_waterfallLeveler;
    int m_receiverWidthHz = 0;
    bool m_narrowBandDisplay = false;
    QElapsedTimer m_receiverFilterAge;

    bool m_signalPeakMetricEnabled = false;
    int m_signalPeakLowHz = 0;
    int m_signalPeakHighHz = 0;
};

#endif // DSPENGINE_H
