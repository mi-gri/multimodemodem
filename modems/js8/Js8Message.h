// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8MESSAGE_H
#define JS8MESSAGE_H

#include "Js8Mode.h"

#include <QString>
#include <QStringList>
#include <QVector>

/**
 * @brief JS8 message layer: text <-> 12-character frames.
 *
 * Thin, thread-safe wrapper around the Varicode/JSC code of JS8Call 2.2.0
 * (third_party/js8call_gpl).  All calls are serialised because the original
 * keeps shared lookup state.
 */
class Js8Message
{
public:
    // Transmission-type bits carried next to each frame (i3).
    enum Bits { Normal = 0, First = 1, Last = 2, Data = 4 };

    struct Frame
    {
        QString frame;   // 12 characters of the 64-symbol alphabet
        int bits = 0;
    };

    struct Unpacked
    {
        QString text;            // human readable text of this frame
        int frameType = 255;     // Varicode::FrameType
        bool first = false;
        bool last = false;
        bool heartbeat = false;
        bool cq = false;         // heartbeat "alt" = CQ
        bool directed = false;
        bool compound = false;
        QString from;            // sender, when the frame names it
        QString to;              // directed target
        QString command;         // directed command, e.g. " SNR?"
        QString extra;           // directed number, e.g. "-12" after " SNR"
        QString grid;
    };

    /// Splits text into frames as JS8Call does (directed, heartbeat, CQ,
    /// compound and free-text data frames).  selectedCall is prefixed
    /// unless the text already starts with a call, @group, CQ or HB.
    static QVector<Frame> buildFrames(const QString &myCall, const QString &myGrid,
                                      const QString &selectedCall, const QString &text,
                                      Js8Mode::Submode submode, bool forceIdentify = false,
                                      bool forceData = false);
    /// Text the receiver will display for the given frames.
    static QString preview(const QVector<Frame> &frames, Js8Mode::Submode submode);
    static Unpacked unpack(const QString &frame, int bits, Js8Mode::Submode submode);

    static QString heartbeatText(const QString &myGrid);
    static QString cqText(const QString &myGrid);
    static bool isValidCallsign(const QString &call);
    /// False when the JSC word tables are missing from the build.
    static bool dictionaryAvailable();
    /// Characters accepted in free text (upper case letters, digits, punctuation).
    static QString normalizeText(const QString &text);

    // Directed-command helpers of JS8Call's Varicode.
    static QString formatSnr(int snrDb);                 // "+05", "-12"
    static bool isCommandBuffered(const QString &command);
    static int commandChecksumBits(const QString &command); // 0, 16 or 32
    static bool checksumValid(const QString &checksum, const QString &text, int bits);
    static QString checksum(const QString &text, int bits);
    static QString rstrip(const QString &text);
    static QString lstrip(const QString &text);
    static QStringList parseCallsigns(const QString &text);
    /// Base call of a portable call (DL/K1ABC/P -> K1ABC), as Radio::base_callsign.
    static QString baseCallsign(const QString &call);
};

#endif // JS8MESSAGE_H
