// SPDX-License-Identifier: GPL-3.0-or-later
// Message grouping after JS8Call 2.2.0 mainwindow.cpp (processCompoundActivity,
// processBufferedActivity), GPL-3.0-or-later.
#include "Js8CommandBuffer.h"

#include "Js8Message.h"

#include <cmath>

namespace {
const QString kUnknownCall = QStringLiteral("<....>");
constexpr int kCloseSeconds = 60;
constexpr int kDropSeconds = 90;
} // namespace

int Js8CommandBuffer::findBuffer(double offsetHz, Js8Mode::Submode submode) const
{
    const double tolerance = Js8Mode::parameters(submode).bandwidthHz() / 4.0;
    for (int i = 0; i < m_buffers.size(); ++i) {
        const Buffer &b = m_buffers.at(i);
        if (b.submode == submode && std::fabs(b.offsetHz - offsetHz) <= tolerance) return i;
    }
    return -1;
}

bool Js8CommandBuffer::resolveCompounds(Buffer &buffer) const
{
    int needed = 0;
    if (buffer.command.from == kUnknownCall) ++needed;
    if (buffer.command.to == kUnknownCall) ++needed;
    if (buffer.compounds.size() < needed) return false;
    if (buffer.command.from == kUnknownCall) {
        const Compound c = buffer.compounds.takeFirst();
        buffer.command.from = c.call;
        if (!c.grid.isEmpty()) buffer.command.grid = c.grid;
        if (c.last) buffer.commandLast = true;
    }
    if (buffer.command.to == kUnknownCall) {
        const Compound c = buffer.compounds.takeFirst();
        buffer.command.to = c.call;
        if (c.last) buffer.commandLast = true;
    }
    return true;
}

bool Js8CommandBuffer::finish(Buffer &buffer, Js8Command *out)
{
    if (!buffer.haveCommand || !resolveCompounds(buffer)) return false;
    Js8Command cmd = buffer.command;
    if (Js8Message::isCommandBuffered(cmd.command) && !buffer.commandLast) {
        if (!buffer.dataLast) return false;
        QString message = Js8Message::rstrip(buffer.parts.join(QString()));
        const int bits = Js8Message::commandChecksumBits(cmd.command);
        bool valid = true;
        if (bits == 32 || bits == 16) {
            const int size = bits == 32 ? 6 : 3;
            message = Js8Message::lstrip(message);
            if (message.size() < size + 1) {
                valid = false;
            } else {
                const QString checksum = message.right(size);
                message = message.left(message.size() - size - 1);
                valid = Js8Message::checksumValid(checksum, message, bits);
            }
        }
        if (!valid) {
            ++m_checksumFailures;
            buffer.haveCommand = false;   // discard; the caller removes the buffer
            return false;
        }
        cmd.text = message;
        cmd.buffered = true;
    }
    *out = cmd;
    return true;
}

QVector<Js8Command> Js8CommandBuffer::add(const Js8Decode &d)
{
    QVector<Js8Command> done;
    const QString from = d.from.trimmed();

    if (d.heartbeat) {
        Js8Command c;
        c.from = from;
        c.to = d.cq ? QStringLiteral("@ALLCALL") : QStringLiteral("@HB");
        c.command = d.cq ? QStringLiteral(" CQ") : QStringLiteral(" HEARTBEAT");
        c.grid = d.grid;
        c.offsetHz = d.frequencyHz;
        c.snrDb = d.snrDb;
        c.utc = d.utc;
        c.submode = d.submode;
        if (!c.from.isEmpty() && c.from != kUnknownCall) done << c;
        return done;
    }

    int index = findBuffer(d.frequencyHz, d.submode);
    if (d.compound || d.directed) {
        if (index < 0) {
            Buffer b;
            b.offsetHz = d.frequencyHz;
            b.submode = d.submode;
            m_buffers << b;
            index = m_buffers.size() - 1;
        }
    }
    if (index < 0) return done;   // free text without a header

    Buffer &b = m_buffers[index];
    b.updated = d.utc;

    if (d.compound) {
        b.compounds << Compound{from, d.grid, d.last};
    } else if (d.directed) {
        if (b.haveCommand) {
            // A header replaces an unfinished message; compound frames of the
            // new message may already be queued, so they stay.
            const QVector<Compound> compounds = b.compounds;
            b = Buffer();
            b.offsetHz = d.frequencyHz;
            b.submode = d.submode;
            b.updated = d.utc;
            b.compounds = compounds;
        }
        b.haveCommand = true;
        b.commandLast = d.last;
        b.command.from = from;
        b.command.to = d.to.trimmed();
        b.command.command = d.command;
        b.command.extra = d.extra.trimmed();
        b.command.grid = d.grid;
        b.command.offsetHz = d.frequencyHz;
        b.command.snrDb = d.snrDb;
        b.command.utc = d.utc;
        b.command.submode = d.submode;
    } else if (b.haveCommand) {
        b.parts << d.text;
        if (d.last) b.dataLast = true;
    } else {
        return done;
    }

    Js8Command cmd;
    if (finish(b, &cmd)) {
        done << cmd;
        m_buffers.removeAt(index);
    } else if (!b.haveCommand && b.compounds.isEmpty()) {
        m_buffers.removeAt(index);   // checksum failure
    }
    return done;
}

QVector<Js8Command> Js8CommandBuffer::expire(const QDateTime &now)
{
    QVector<Js8Command> done;
    for (int i = m_buffers.size() - 1; i >= 0; --i) {
        Buffer &b = m_buffers[i];
        const qint64 age = b.updated.isValid() ? b.updated.secsTo(now) : kDropSeconds + 1;
        if (age > kDropSeconds) {
            m_buffers.removeAt(i);
            continue;
        }
        if (age > kCloseSeconds && b.haveCommand && !b.parts.isEmpty() && !b.dataLast) {
            b.dataLast = true;
            Js8Command cmd;
            if (finish(b, &cmd)) done.prepend(cmd);
            m_buffers.removeAt(i);
        }
    }
    return done;
}
