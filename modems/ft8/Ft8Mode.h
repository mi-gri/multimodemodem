#ifndef FT8MODE_H
#define FT8MODE_H

#include "../../dsp/FrequencyMarker.h"

#include <QString>
#include <QStringList>
#include <QVector>

/**
 * @brief Metadata for the interoperable Franke-Taylor digital modes supported by MultiModeModem.
 *
 * v1.12 intentionally focuses on real on-air modes FT8 and FT4.  FT4 is not an
 * FT8 timer tweak: it has a different 4-FSK symbol structure, four 4-symbol
 * Costas sync blocks, shorter symbol timing and its own candidate/metric path.
 */
class Ft8Mode
{
public:
    struct Profile
    {
        QString modeName;
        QString shortLabel;
        QString adifMode;
        int slotMs = 15000;
        int cycleMs = 30000;
        int signalMs = 12640;
        bool interoperableCoreAvailable = false;
        bool experimental = false;
        QString note;
    };

    static QString modeName();
    static QStringList allModeNames();
    static bool isFamilyMode(const QString &modeName);
    static Profile profileForMode(const QString &modeName);
    static QVector<FrequencyMarker> frequencyMarkers(int rxHz, int txHz,
                                                     const QString &modeName = QString());

    /**
     * @brief Bandwidth a signal occupies from its base tone, in Hz:
     *        FT8 8 tones x 6.25 Hz = 50 Hz, FT4 4 tones x 20.83 Hz = 83.3 Hz.
     */
    static double signalBandwidthHz(const QString &modeName);
};

#endif // FT8MODE_H
