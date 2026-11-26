#include "../modems/ft8/FtWorkQueue.h"
#include <cstdio>
#include <cstdlib>

using namespace M3;
static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); ++failures; } } while (0)

static FtQueueEntry entry(const char *call, QueueSource src, int snr = -10, bool newCountry = false, double km = 500)
{
    FtQueueEntry e;
    e.call = QString::fromLatin1(call);
    e.source = src;
    e.valid = true;
    e.cq = src != QueueSource::Caller;
    e.decode.snrDb = snr;
    e.decode.slotStartUtcMs = 1000;
    e.newCountry = newCountry;
    e.distanceKm = km;
    return e;
}

int main()
{
    const QDateTime t0 = QDateTime::fromMSecsSinceEpoch(1000000, Qt::UTC);
    auto at = [&](int s) { return t0.addSecs(s); };
    {   // order: pinned, callers FIFO, value, no-reply last
        FtWorkQueue q;
        q.upsert(entry("DL1AAA", QueueSource::AutoCq, -5, false, 100), at(0));
        q.upsert(entry("DL2BBB", QueueSource::AutoCq, -15, true, 100), at(1));
        q.upsert(entry("DL3CCC", QueueSource::Caller), at(5));
        q.upsert(entry("DL4DDD", QueueSource::Caller), at(2));
        q.upsert(entry("DL5EEE", QueueSource::Manual, -2, false, 3000), at(3));
        auto o = q.order(at(10));
        CHECK(q.entries[o[0]].call == "DL4DDD");   // callers first, earlier first
        CHECK(q.entries[o[1]].call == "DL3CCC");
        CHECK(q.entries[o[2]].call == "DL2BBB");   // new country wins
        CHECK(q.entries[o[3]].call == "DL5EEE");   // farther than DL1AAA
        CHECK(q.entries[o[4]].call == "DL1AAA");
        q.togglePinned("DL1AAA");
        o = q.order(at(10));
        CHECK(q.entries[o[0]].call == "DL1AAA");
        q.togglePinned("DL5EEE");                  // only one pin
        CHECK(!q.entries[q.indexOf("DL1AAA")].pinned && q.entries[q.indexOf("DL5EEE")].pinned);
        q.togglePinned("DL5EEE");
        CHECK(!q.entries[q.indexOf("DL5EEE")].pinned);
        q.markNoReply(q.entries[q.indexOf("DL4DDD")], at(900), at(10));
        o = q.order(at(10));
        CHECK(q.entries[o.last()].call == "DL4DDD");   // no reply: at the end
        CHECK(q.entries[o[0]].call == "DL3CCC");
        q.expire(at(1000), 300);                       // the pause is over: gone; the others are too old
        CHECK(q.isEmpty());
    }
    {   // upsert keeps the caller source and the pin; older decodes never replace newer ones
        FtWorkQueue q;
        q.upsert(entry("DL1AAA", QueueSource::Caller), at(0));
        q.togglePinned("DL1AAA");
        FtQueueEntry cq = entry("DL1AAA", QueueSource::AutoCq, -3);
        cq.decode.slotStartUtcMs = 2000;
        q.upsert(cq, at(20));
        CHECK(q.size() == 1 && q.entries[0].source == QueueSource::Caller && q.entries[0].pinned);
        CHECK(q.entries[0].firstSeenUtc == at(0));
        FtQueueEntry old = entry("DL1AAA", QueueSource::Caller, -20);
        old.decode.slotStartUtcMs = 500;
        q.upsert(old, at(30));
        CHECK(q.entries[0].decode.snrDb == -3);
    }
    {   // a call to me ends the no-reply pause; only ready callers hold back a new CQ series
        FtWorkQueue q;
        q.upsert(entry("ZS4AW", QueueSource::AutoCq), at(0));
        q.markNoReply(q.entries[0], at(900), at(10));
        q.upsert(entry("ZS4AW", QueueSource::Caller), at(20));          // same slot as stored: nothing new
        CHECK(q.entries[0].noReply(at(20)) && q.readyCallers(at(20), 300).isEmpty());
        FtQueueEntry cq = entry("ZS4AW", QueueSource::AutoCq);
        cq.decode.slotStartUtcMs = 2000;
        q.upsert(cq, at(25));                                           // a new CQ is no reply to me
        CHECK(q.entries[0].noReply(at(25)));
        FtQueueEntry call = entry("ZS4AW", QueueSource::Caller);
        call.decode.slotStartUtcMs = 3000;
        q.upsert(call, at(40));
        CHECK(!q.entries[0].noReply(at(40)) && q.entries[0].firstSeenUtc == at(40));
        CHECK(q.readyCallers(at(40), 300) == QStringList{QStringLiteral("ZS4AW")});
        CHECK(q.readyCallers(at(400), 300).isEmpty());                  // too old
        q.markNoReply(q.entries[0], at(900), at(50));
        CHECK(q.readyCallers(at(50), 300).isEmpty());                   // paused caller blocks nothing
    }
    {   // at most two lifts per station within 15 minutes; one decode counts once
        FtWorkQueue q;
        auto call = [&](qint64 slot, int s) {
            FtQueueEntry c = entry("DL7XYZ", QueueSource::Caller);
            c.decode.slotStartUtcMs = slot;
            q.upsert(c, at(s));
            return !q.entries[q.indexOf("DL7XYZ")].noReply(at(s));
        };
        q.upsert(entry("DL7XYZ", QueueSource::Caller), at(0));
        q.markNoReply(q.entries[0], at(900), at(10));
        CHECK(call(2000, 20));
        CHECK(q.allowNoReplyLift("DL7XYZ", 2000, at(21)));          // same decode again: not counted
        q.markNoReply(q.entries[0], at(930), at(30));
        CHECK(call(3000, 40));
        q.markNoReply(q.entries[0], at(950), at(50));
        CHECK(!call(4000, 60));                                     // third lift within the window: stays paused
        CHECK(!q.canLiftNoReply("DL7XYZ", 5000, at(70)));
        CHECK(q.canLiftNoReply("DL8OTHER", 5000, at(70)));          // another station is not affected
        CHECK(q.allowNoReplyLift("DL7XYZ", 6000, at(1000)));        // the window has passed
    }
    {   // limit 50: CQs are dropped for new entries, callers and pins never
        FtWorkQueue q;
        for (int i = 0; i < 50; ++i) q.upsert(entry(("DL" + std::to_string(100 + i) + "X").c_str(), QueueSource::AutoCq, -30 + i % 5, false, 100 + i), at(i));
        CHECK(q.size() == 50);
        CHECK(q.upsert(entry("DK9NEW", QueueSource::Caller), at(60)));
        CHECK(q.size() == 50 && q.contains("DK9NEW"));
        FtWorkQueue full;
        for (int i = 0; i < 50; ++i) full.upsert(entry(("DL" + std::to_string(200 + i) + "X").c_str(), QueueSource::Caller), at(i));
        CHECK(!full.upsert(entry("DK9NO", QueueSource::AutoCq), at(60)));   // only callers: no room
        CHECK(full.size() == 50);
    }
    if (failures == 0) std::puts("FtWorkQueue regression OK");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
