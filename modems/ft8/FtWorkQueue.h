// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

// One queue for everything the FT automation may work next: stations that called
// me, CQs of Auto mode and CQs the operator added. Policy order (what the automation
// takes next):
//   1. the entry the operator marked "Next" (pinned), if it is ready
//   2. stations that called me, first come first served
//   3. all others by value (new country, grid, band, mode, distance, SNR)
//   4. entries that gave no reply, at the end (grey, until their pause is over)
// The table the operator sees may be sorted any way; that never changes this order.
#include "Ft8RxDecoder.h"
#include "FtDecodedText.h"

#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QVector>

#include <algorithm>
#include <utility>
/**
 * @brief One decoded CQ waiting for Auto QSO priority selection.
 *
 * Auto QSO must not answer the first CQ that happens to be emitted by the
 * decoder.  A full FT pass can contain many simultaneous CQs; MM buffers them
 * briefly, ranks them by worked-needed value and distance, then starts exactly
 * one QSO.
 */
struct Ft8FullAutoCqCandidate
{
    Ft8RxDecoder::Decode decode;
    QString call;
    QString grid;
    QString distanceGrid;
    QString country;
    QString dxcc;
    QString band;
    QString mode;
    bool valid = false;
    bool cq = true;
    bool workedCall = false;
    bool newCountry = false;
    bool newGrid = false;
    bool newBand = false;
    bool newMode = false;
    double distanceKm = -1.0;
    QDateTime queuedUtc;
    QString priorityText;
    static bool higherPriority(const Ft8FullAutoCqCandidate &a, const Ft8FullAutoCqCandidate &b) {
        if (a.newCountry != b.newCountry) return a.newCountry;
        if (a.newGrid != b.newGrid) return a.newGrid;
        if (a.newBand != b.newBand) return a.newBand;
        if (a.newMode != b.newMode) return a.newMode;
        if (a.workedCall != b.workedCall) return !a.workedCall;
        if (qAbs(a.distanceKm - b.distanceKm) > 1.0) return a.distanceKm > b.distanceKm;
        if (a.decode.snrDb != b.decode.snrDb) return a.decode.snrDb > b.decode.snrDb;
        return a.decode.frequencyHz < b.decode.frequencyHz;
    }
};


namespace M3 {

enum class QueueSource { Caller, AutoCq, Manual };

struct FtQueueEntry : Ft8FullAutoCqCandidate
{
    QueueSource source = QueueSource::AutoCq;
    QDateTime firstSeenUtc;
    QDateTime lastSeenUtc;
    bool pinned = false;            // "Next"
    bool calledDuringActivity = false;   // a caller who called while my QSO or CQ ran: not a call out of the blue
    QDateTime noReplyUntilUtc;      // valid and in the future: gave no reply, at the end of the list

    bool noReply(const QDateTime &now) const { return noReplyUntilUtc.isValid() && noReplyUntilUtc > now; }
};

class FtWorkQueue
{
public:
    static constexpr int MaxEntries = 50;

    // A call to me may end the no-reply pause of a station only this often within the window: a station that does
    // not hear me must not be worked again and again.
    static constexpr int MaxNoReplyLifts = 2;
    static constexpr qint64 NoReplyLiftWindowSecs = 15 * 60;

    QVector<FtQueueEntry> entries;

    int indexOf(const QString &call) const
    {
        for (int i = 0; i < entries.size(); ++i)
            if (FtDecodedText::callMatches(entries.at(i).call, call)) return i;
        return -1;
    }
    bool contains(const QString &call) const { return indexOf(call) >= 0; }
    int size() const { return entries.size(); }
    bool isEmpty() const { return entries.isEmpty(); }

    int count(QueueSource source) const
    {
        int n = 0;
        for (const auto &e : entries) n += e.source == source ? 1 : 0;
        return n;
    }

    // A call to me heard after the stored decode is a reply: it ends the no-reply pause of that station.
    static bool endsNoReplyPause(const FtQueueEntry &old, const FtQueueEntry &heard)
    {
        return heard.source == QueueSource::Caller && old.noReplyUntilUtc.isValid() &&
               heard.decode.slotStartUtcMs > old.decode.slotStartUtcMs;
    }

    // True when a decode of @p call heard in the slot @p slotUtcMs may end its no-reply pause (the same decode
    // asks more than once on its way through the program and counts once).
    bool canLiftNoReply(const QString &call, qint64 slotUtcMs, const QDateTime &now) const
    {
        int used = 0;
        for (const Lift &lift : m_lifts) {
            if (lift.utc.secsTo(now) > NoReplyLiftWindowSecs || !FtDecodedText::callMatches(lift.call, call)) continue;
            if (lift.slotUtcMs == slotUtcMs) return true;
            ++used;
        }
        return used < MaxNoReplyLifts;
    }
    // Like canLiftNoReply, and counts the lift.
    bool allowNoReplyLift(const QString &call, qint64 slotUtcMs, const QDateTime &now)
    {
        for (int i = m_lifts.size() - 1; i >= 0; --i)
            if (m_lifts.at(i).utc.secsTo(now) > NoReplyLiftWindowSecs) m_lifts.removeAt(i);
        if (!canLiftNoReply(call, slotUtcMs, now)) return false;
        for (const Lift &lift : std::as_const(m_lifts))
            if (lift.slotUtcMs == slotUtcMs && FtDecodedText::callMatches(lift.call, call)) return true;
        m_lifts.append({call.trimmed().toUpper(), slotUtcMs, now});
        return true;
    }

    // Calls of the stations that may be served now: callers without a no-reply pause and not too old. Only these
    // have priority over a new CQ series; any other caller entry must not hold the automation back.
    QStringList readyCallers(const QDateTime &now, qint64 maxAgeSecs) const
    {
        QStringList calls;
        for (const auto &e : entries) {
            if (e.source != QueueSource::Caller || e.noReply(now)) continue;
            if (!e.pinned && e.lastSeenUtc.isValid() && e.lastSeenUtc.secsTo(now) > maxAgeSecs) continue;
            calls << e.call;
        }
        return calls;
    }

    // Adds or refreshes an entry. A newer entry keeps what the operator did (pinned, no-reply pause - unless the
    // station now calls me, see endsNoReplyPause) and the first-seen time; a caller stays a caller; a manual entry stays manual. Returns false when the queue is
    // full and nothing could be dropped for it.
    bool upsert(FtQueueEntry entry, const QDateTime &now)
    {
        if (entry.call.trimmed().isEmpty()) return false;
        if (!entry.firstSeenUtc.isValid()) entry.firstSeenUtc = now;
        entry.lastSeenUtc = now;
        const int idx = indexOf(entry.call);
        if (idx >= 0) {
            const FtQueueEntry &old = entries.at(idx);
            if (entry.decode.slotStartUtcMs > 0 && old.decode.slotStartUtcMs > entry.decode.slotStartUtcMs && old.source == entry.source)
                return true;                                  // never replace by an older decode
            entry.pinned = old.pinned || entry.pinned;
            entry.calledDuringActivity = old.calledDuringActivity || entry.calledDuringActivity;
            if (endsNoReplyPause(old, entry) && allowNoReplyLift(entry.call, entry.decode.slotStartUtcMs, now)) {
                entry.noReplyUntilUtc = QDateTime();
                entry.firstSeenUtc = now;                     // first come first served counts from this call
            } else {
                entry.noReplyUntilUtc = old.noReplyUntilUtc;
                entry.firstSeenUtc = old.firstSeenUtc.isValid() ? old.firstSeenUtc : entry.firstSeenUtc;
            }
            if (old.source == QueueSource::Caller) entry.source = QueueSource::Caller;
            else if (old.source == QueueSource::Manual && entry.source == QueueSource::AutoCq) entry.source = QueueSource::Manual;
            entries[idx] = entry;
            return true;
        }
        if (entries.size() >= MaxEntries && !dropOne(now)) return false;
        entries.append(entry);
        return true;
    }

    void removeCall(const QString &call)
    {
        for (int i = entries.size() - 1; i >= 0; --i)
            if (FtDecodedText::callMatches(entries.at(i).call, call)) entries.removeAt(i);
    }
    void clear() { entries.clear(); }
    void removeSource(QueueSource source)
    {
        for (int i = entries.size() - 1; i >= 0; --i)
            if (entries.at(i).source == source) entries.removeAt(i);
    }

    // Only one entry can be "Next"; pinning it again lifts the mark.
    void togglePinned(const QString &call)
    {
        const int idx = indexOf(call);
        if (idx < 0) return;
        const bool wasPinned = entries.at(idx).pinned;
        for (auto &e : entries) e.pinned = false;
        entries[idx].pinned = !wasPinned;
    }

    // Gave no reply: grey, at the end, for @p until.
    void markNoReply(FtQueueEntry entry, const QDateTime &until, const QDateTime &now)
    {
        entry.noReplyUntilUtc = until;
        entry.pinned = false;
        const int idx = indexOf(entry.call);
        if (idx >= 0) {
            entries[idx].noReplyUntilUtc = until;
            entries[idx].pinned = false;
            return;
        }
        entry.firstSeenUtc = entry.firstSeenUtc.isValid() ? entry.firstSeenUtc : now;
        entry.lastSeenUtc = now;
        if (entries.size() >= MaxEntries && !dropOne(now)) return;
        entries.append(entry);
    }

    // Removes entries that are too old; the no-reply pause also ends an entry once it is over.
    // maxAgeSecs applies to callers and CQs alike (a CQ or call older than that is not interesting any more).
    void expire(const QDateTime &now, qint64 maxAgeSecs)
    {
        for (int i = entries.size() - 1; i >= 0; --i) {
            const FtQueueEntry &e = entries.at(i);
            if (e.noReplyUntilUtc.isValid()) {
                if (e.noReplyUntilUtc <= now) entries.removeAt(i);
                continue;
            }
            if (e.pinned) continue;
            if (e.lastSeenUtc.isValid() && e.lastSeenUtc.secsTo(now) > maxAgeSecs) entries.removeAt(i);
        }
    }

    // Indices in policy order (see the top of the file).
    QVector<int> order(const QDateTime &now) const
    {
        QVector<int> idx;
        idx.reserve(entries.size());
        for (int i = 0; i < entries.size(); ++i) idx.append(i);
        const auto group = [&](const FtQueueEntry &e) {
            if (e.noReply(now)) return 3;
            if (e.pinned) return 0;
            if (e.source == QueueSource::Caller) return 1;
            return 2;
        };
        std::stable_sort(idx.begin(), idx.end(), [&](int a, int b) {
            const FtQueueEntry &ea = entries.at(a);
            const FtQueueEntry &eb = entries.at(b);
            const int ga = group(ea), gb = group(eb);
            if (ga != gb) return ga < gb;
            if (ga == 1) return ea.firstSeenUtc < eb.firstSeenUtc;                       // callers: first come first served
            if (ga == 3) return ea.noReplyUntilUtc < eb.noReplyUntilUtc;                 // longest ago first
            return Ft8FullAutoCqCandidate::higherPriority(ea, eb);
        });
        return idx;
    }

private:
    struct Lift {
        QString call;
        qint64 slotUtcMs = 0;
        QDateTime utc;
    };
    QVector<Lift> m_lifts;

    // Makes room: the least valuable entry that is neither pinned nor a caller (no-reply entries first).
    bool dropOne(const QDateTime &now)
    {
        const QVector<int> o = order(now);
        for (int k = o.size() - 1; k >= 0; --k) {
            const FtQueueEntry &e = entries.at(o.at(k));
            if (e.pinned || e.source == QueueSource::Caller) continue;
            entries.removeAt(o.at(k));
            return true;
        }
        return false;
    }
};

} // namespace M3
