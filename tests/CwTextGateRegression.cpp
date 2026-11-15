// CW text gate: noise-like decoder output is held back, real traffic passes.
#include "modems/cw/CwTextGate.h"

#include <cstdio>

static int g_fails = 0;
static void check(bool ok, const char *name) { if (!ok) ++g_fails; std::printf("%s %s\n", ok ? "PASS" : "FAIL", name); }

static QString run(const QString &text, int chunk = 3)
{
    CwTextGate gate;
    QString out;
    for (int i = 0; i < text.size(); i += chunk) out += gate.feed(text.mid(i, chunk));
    return out;
}

int main()
{
    const QString qso = QStringLiteral("CQ CQ DE DL1ABC DL1ABC K DL1ABC DE DO3BOX DO3BOX K GM UR RST 579 579 NAME MICHAEL QTH BERLIN HW? AR R R TNX FER QSO 73 SK");
    check(run(qso) == qso, "ragchew passes unchanged (also NAME MICHAEL QTH)");
    const QString contest = QStringLiteral("TEST DL1ABC TEST DL1ABC 5NN 14 TU DO3BOX 5NN 14 TU QRZ TEST K3LR TEST");
    check(run(contest) == contest, "contest exchange passes unchanged");
    // Decoded from a fading 80 m carrier (2026-11-15 recording) and the live log.
    check(run(QStringLiteral("T NEI5 I IKK IEE EATTAEEE T EL I TTEM TT TIN ATTE")).isEmpty(), "decoded carrier noise is held back");
    check(run(QStringLiteral("E T TEA TE EET E E/AAEE IEE EATTAEEE T EL I TTEM TT TIN ")).trimmed().isEmpty(), "live-log noise is held back");
    // Noise, then a real signal: the signal is shown, the noise before it is not.
    const QString after = run(QStringLiteral("E T TEA TE EET E E/AAEE IEE EATTAEEE T EL I TTEM TT TIN CQ TEST DL1ABC DL1ABC TEST"));
    check(after.contains(QStringLiteral("CQ TEST DL1ABC DL1ABC TEST")) && !after.contains(QStringLiteral("EATTAEEE")), "signal after noise shown without the noise");
    // A real QSO with fades (decoded from a recording) keeps its content.
    const QString faded = run(QStringLiteral("GLESDX GB MM EAM D F PN8MEE E IMRS TUEE E TE E E E T E EDX1KL GE TU CALL S7N 57N O R EEIG EEIG EIWSEE LX1KL E3RS K"));
    check(faded.contains(QStringLiteral("CALL")) && faded.contains(QStringLiteral("LX1KL E3RS K")), "faded real QSO keeps its calls");
    CwTextGate gate;
    gate.feed(QStringLiteral("CQ CQ DE DL1ABC DL1ABC K"));
    check(gate.isOpen(), "gate opens on traffic");
    gate.reset();
    check(!gate.isOpen() && gate.feed(QStringLiteral("E")).isEmpty(), "reset closes and judges afresh");
    std::printf("%s\n", g_fails ? "RESULT FAIL" : "RESULT PASS");
    return g_fails;
}
