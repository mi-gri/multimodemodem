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
    ok &= back;
    return ok ? 0 : 1;
}
