// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8MODE_H
#define JS8MODE_H

#include <QString>
#include <QStringList>
#include <array>

/**
 * @brief JS8 (JS8Call 2.2) protocol constants.
 *
 * JS8 is 8-FSK with 79 symbols (three 7x7 Costas arrays and 58 data
 * symbols), an LDPC(174,87) code over 75 information bits plus CRC12, and a
 * tone spacing equal to the keying rate.  The values below are the
 * js8{a,b,c,e}_params.f90 / commons.h constants of JS8Call 2.2.0.  The
 * experimental "Ultra" submode (disabled in JS8Call releases) is left out.
 */
class Js8Mode
{
public:
    // Values match Varicode::SubmodeType of JS8Call.
    enum class Submode { Normal = 0, Fast = 1, Turbo = 2, Slow = 4 };

    struct Parameters
    {
        Submode submode;
        char letter;           // A, B, C, E as in JS8Call's decoder
        int nsps;              // samples per symbol at 12 kHz
        int periodSeconds;     // TX period
        int decodeSeconds;     // NTXDUR: decoded part of the period
        int startDelayMs;      // signal starts this long after the boundary
        int nDownSps;          // samples per symbol after downsampling
        int ndd;               // long-FFT length factor (NDFFT1 = nsps*ndd)
        int jz;                // +/- sync search range in quarter symbols
        double dedupeFactor;   // AZ = 12000/nsps * factor
        int costas;            // 1 original, 2 three symmetrical arrays

        double baud() const { return 12000.0 / nsps; }
        double bandwidthHz() const { return 8.0 * baud(); }
        int nmax() const { return decodeSeconds * 12000; }
        double startSeconds() const { return startDelayMs / 1000.0; }
        double durationSeconds() const { return 79.0 * nsps / 12000.0; }
    };

    static constexpr int kSymbols = 79;
    static constexpr int kDataSymbols = 58;

    static Parameters parameters(Submode s)
    {
        switch (s) {
        case Submode::Fast:  return {s, 'B', 1200, 10, 10, 200, 20, 100, 144, 0.8, 2};
        case Submode::Turbo: return {s, 'C', 600, 6, 6, 100, 12, 120, 172, 0.6, 2};
        case Submode::Slow:  return {s, 'E', 3840, 30, 28, 500, 32, 90, 32, 0.64, 2};
        case Submode::Normal:
        default:             return {Submode::Normal, 'A', 1920, 15, 15, 500, 32, 100, 62, 0.64, 1};
        }
    }

    using Costas = std::array<std::array<int, 7>, 3>;
    static Costas costas(int which)
    {
        if (which == 1) return {{{4, 2, 5, 6, 1, 3, 0}, {4, 2, 5, 6, 1, 3, 0}, {4, 2, 5, 6, 1, 3, 0}}};
        return {{{0, 6, 2, 3, 5, 4, 1}, {1, 5, 0, 2, 3, 6, 4}, {2, 5, 0, 6, 4, 1, 3}}};
    }

    static QString familyName() { return QStringLiteral("JS8"); }
    static QString adifMode() { return QStringLiteral("MFSK"); }
    static QString adifSubmode() { return QStringLiteral("JS8"); }
    static QString submodeName(Submode s)
    {
        switch (s) {
        case Submode::Fast: return QStringLiteral("Fast");
        case Submode::Turbo: return QStringLiteral("Turbo");
        case Submode::Slow: return QStringLiteral("Slow");
        case Submode::Normal:
        default: return QStringLiteral("Normal");
        }
    }
    static QList<Submode> allSubmodes() { return {Submode::Normal, Submode::Fast, Submode::Turbo, Submode::Slow}; }
    static Submode submodeFromInt(int v)
    {
        switch (v) {
        case 1: return Submode::Fast;
        case 2: return Submode::Turbo;
        case 4: return Submode::Slow;
        default: return Submode::Normal;
        }
    }
    static bool isMode(const QString &mode) { return mode.trimmed().compare(familyName(), Qt::CaseInsensitive) == 0; }
    static int defaultRxFrequencyHz() { return 1500; }
    static int defaultTxFrequencyHz() { return 1500; }
    static int minimumFrequencyHz() { return 200; }
    static int maximumFrequencyHz(Submode s) { return 2900 - int(parameters(s).bandwidthHz()); }
};

#endif // JS8MODE_H
