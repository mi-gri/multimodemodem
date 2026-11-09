// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8COMMANDBUFFER_H
#define JS8COMMANDBUFFER_H

#include "Js8Decoder.h"
#include "Js8Mode.h"

#include <QDateTime>
#include <QString>
#include <QVector>

/// One complete directed message (or heartbeat/CQ) heard on the band.
struct Js8Command
{
    QString from;          // sender (compound calls resolved)
    QString to;            // call or @GROUP
    QString command;       // " SNR?", " MSG", ">", " HEARTBEAT", " CQ" ...
    QString extra;         // directed number, e.g. SNR in " SNR -12"
    QString text;          // data after the command, checksum removed
    QString grid;
    double offsetHz = 0.0;
    int snrDb = -28;
    QDateTime utc;
    Js8Mode::Submode submode = Js8Mode::Submode::Normal;
    bool buffered = false; // text came from data frames after the header
};

/**
 * @brief Collects directed JS8 frames into complete commands.
 *
 * Follows JS8Call 2.2.0 (processCompoundActivity / processBufferedActivity):
 * per offset, a directed header opens a buffer; compound frames supply
 * portable calls written as "<....>"; buffered commands (MSG, MSG TO:,
 * QUERY ..., relay ">", CMD, GRID) collect the data frames up to the frame
 * marked last and must pass their CRC-16/32 checksum.  Unbuffered commands
 * (SNR?, INFO?, HEARING?, ACK ...) complete with their header frame.
 * A buffer without a last frame is closed after 60 s and dropped after 90 s.
 */
class Js8CommandBuffer
{
public:
    /// Feeds one decoded frame; returns the commands it completed.
    QVector<Js8Command> add(const Js8Decode &decode);
    /// Closes or drops stale buffers; returns commands closed by the timeout.
    QVector<Js8Command> expire(const QDateTime &now);
    void clear() { m_buffers.clear(); }
    /// Buffered messages discarded because the checksum did not match.
    int checksumFailures() const { return m_checksumFailures; }
    int openBuffers() const { return m_buffers.size(); }

private:
    struct Compound
    {
        QString call;
        QString grid;
        bool last = false;
    };
    struct Buffer
    {
        double offsetHz = 0.0;
        Js8Mode::Submode submode = Js8Mode::Submode::Normal;
        bool haveCommand = false;
        bool commandLast = false;   // header frame was also the last frame
        Js8Command command;
        QVector<Compound> compounds;
        QStringList parts;
        bool dataLast = false;
        QDateTime updated;
    };

    int findBuffer(double offsetHz, Js8Mode::Submode submode) const;
    bool resolveCompounds(Buffer &buffer) const;
    bool finish(Buffer &buffer, Js8Command *out);

    QVector<Buffer> m_buffers;
    int m_checksumFailures = 0;
};

#endif // JS8COMMANDBUFFER_H
