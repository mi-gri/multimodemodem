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
    return ok ? 0 : 1;
}
