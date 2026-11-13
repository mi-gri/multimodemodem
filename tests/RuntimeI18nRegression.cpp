#include "utils/RuntimeI18n.h"
#include <QCoreApplication>
#include <iostream>
int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    M3I18n::setLanguageCode("it");
    bool ok = M3I18n::text("Track Moon / EME") == QStringLiteral("Segui Luna / EME");
    ok &= M3I18n::text("User / QTH / Macros") == QStringLiteral("Utente / QTH / Macro");
    ok &= M3I18n::text("Display / FT colours") == QStringLiteral("Aspetto / colori FT");
    std::cout << (ok ? "PASS" : "FAIL") << " translated labels containing slash\n";
    // Live language switch: a caption shown in German leads back to its
    // English source, so it can be translated into the next language.
    M3I18n::setLanguageCode("de");
    const bool back = M3I18n::sourceText(QStringLiteral("Dekodieren")) == QStringLiteral("Decode") &&
                      M3I18n::sourceText(QStringLiteral("DO3BOX")) == QStringLiteral("DO3BOX");
    std::cout << (back ? "PASS" : "FAIL") << " source text of a translated caption\n";
    // ... and an English source known only as a keyed value finds its way back.
    // (An English text used by one key only, so the mapping is unambiguous.)
    const bool forth = M3I18n::fromEnglish(QStringLiteral("JS8 automatic operation")) == QStringLiteral("JS8-Automatik") &&
                       M3I18n::text(QStringLiteral("JS8 automatic operation")) == QStringLiteral("JS8-Automatik");
    std::cout << (forth ? "PASS" : "FAIL") << " keyed caption from its English source\n";
    ok &= forth;
    ok &= back;
    return ok ? 0 : 1;
}
