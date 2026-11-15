// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QString>

#include <deque>

/**
 * @brief Holds back CW text until it reads like real Morse traffic.
 *
 * A Morse decoder fed with noise, a fading carrier or a birdie produces a
 * stream of one- and two-element characters ("E T TEA TE EET").  Real traffic
 * - ragchew, contest exchange, calls, numbers - has far fewer of them:
 * measured 15-37 % E/T/I/A/N/M in real text, 60-89 % in decoded noise
 * (synthetic -10/-13 dB, a recorded 80 m carrier, the live log).
 *
 * New characters are kept back until the last kWindow letters look like
 * text; then everything held is released at once and further characters pass
 * immediately while the window stays plausible.  When the window turns into
 * noise the gate closes again and holds; held text beyond kMaxHeld is dropped.
 * Signal processing is untouched: this only decides what is shown.
 */
class CwTextGate
{
public:
    static constexpr int kWindow = 16;       // letters judged
    static constexpr int kMinJudged = 8;     // before this, hold
    static constexpr double kOpenAt = 0.48;  // short-letter share to open
    static constexpr double kCloseAt = 0.55; // ... and to close again
    static constexpr int kMaxHeld = 48;      // held text kept while closed

    /// Feeds decoded text; returns the part to show now (may be empty).
    QString feed(const QString &text)
    {
        QString out;
        for (const QChar c : text) {
            const bool letter = !c.isSpace();
            if (letter) {
                m_window.push_back(isShort(c));
                if (int(m_window.size()) > kWindow) m_window.pop_front();
            }
            m_held += c;
            if (letter) updateState();
            if (m_open) {
                out += m_held;
                m_held.clear();
            } else if (m_held.size() > kMaxHeld) {
                m_held.remove(0, m_held.size() - kMaxHeld);
            }
        }
        return out;
    }

    void reset()
    {
        m_window.clear();
        m_held.clear();
        m_open = false;
    }

    bool isOpen() const { return m_open; }
    double shortShare() const
    {
        if (m_window.empty()) return 0.0;
        int n = 0;
        for (bool s : m_window) n += s ? 1 : 0;
        return double(n) / double(m_window.size());
    }

private:
    static bool isShort(QChar c)
    {
        switch (c.toUpper().unicode()) {
        case 'E': case 'T': case 'I': case 'A': case 'N': case 'M': return true;
        default: return false;
        }
    }

    void updateState()
    {
        if (int(m_window.size()) < kMinJudged) return;
        const double share = shortShare();
        if (!m_open && share <= kOpenAt) {
            m_open = true;
            // Release only what was judged (the last kWindow letters and the
            // spaces between them); older held text was noise.
            int letters = 0;
            int start = m_held.size();
            while (start > 0) {
                if (!m_held.at(start - 1).isSpace() && ++letters > kWindow) break;
                --start;
            }
            m_held.remove(0, start);
        }
        else if (m_open && share >= kCloseAt) {
            // Keep holding: a short run of E/T/A/M inside real text (NAME
            // MICHAEL) reopens soon and then nothing is lost; real noise
            // stays closed and is dropped beyond kMaxHeld.
            m_open = false;
        }
    }

    std::deque<bool> m_window;
    QString m_held;
    bool m_open = false;
};
