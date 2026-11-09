// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QRegularExpression>
#include <QString>
#include <QStringList>

/**
 * @brief Finds the transmitting station in decoded keyboard-mode text.
 *
 * For PSK Reporter spots from RTTY, PSK, MFSK and CW.  A callsign counts only
 * when it follows "CQ" or "DE" and is sent twice in a row ("CQ CQ DE DL1ABC
 * DL1ABC K", "K1ABC DE DL1ABC DL1ABC KN"), so a single garbled character does
 * not create a false spot.  The rules extend SkimmerCqTracker (the CW/RTTY
 * skimmer CQ detector) by the "DE" identification.  Text is fed incrementally.
 */
class TextCallSpotter
{
public:
    void reset() { *this = TextCallSpotter(); }

    /// Feeds decoded text; returns the stations confirmed by it.
    QStringList append(const QString &text)
    {
        QStringList found;
        for (QChar c : text.toUpper()) {
            if (!c.isSpace() && c != QLatin1Char(',') && c != QLatin1Char(':')) {
                m_token += c;
                if (m_token.size() > 20) m_token = QStringLiteral("?");
                continue;
            }
            if (m_token.isEmpty()) continue;
            const QString word = m_token;
            m_token.clear();
            const QString call = takeWord(word);
            if (!call.isEmpty()) found << call;
        }
        return found;
    }

private:
    QString takeWord(const QString &word)
    {
        if (word == QLatin1String("CQ")) { open(16); return QString(); }
        if (word == QLatin1String("DE")) { open(6); return QString(); }
        if (word == QLatin1String("QRZ") || word == QLatin1String("QRZ?") || word == QLatin1String("73") ||
            word == QLatin1String("SK")) {
            open(0);
            return QString();
        }
        if (m_window <= 0) return QString();
        --m_window;
        static const QRegularExpression callPattern(
            QStringLiteral("^(?:[A-Z0-9]{1,4}/)?[A-Z0-9]{1,3}[0-9][A-Z]{1,4}(?:/[A-Z0-9]{1,4})?$"));
        static const QRegularExpression gridPattern(QStringLiteral("^[A-R]{2}[0-9]{2}(?:[A-X]{2})?$"));
        if (!callPattern.match(word).hasMatch() || gridPattern.match(word).hasMatch()) return QString();
        if (m_candidate.isEmpty() || m_candidate != word) {
            if (!m_candidate.isEmpty()) { open(0); return QString(); }   // two different calls: not an ID
            m_candidate = word;
            return QString();
        }
        // Second time in this window: confirmed, report once.
        open(0);
        return word;
    }

    void open(int tokens)
    {
        m_window = tokens;
        m_candidate.clear();
    }

    QString m_token;
    QString m_candidate;
    int m_window = 0;
};
