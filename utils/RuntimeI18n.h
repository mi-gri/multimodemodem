#ifndef M3_RUNTIMEI18N_H
#define M3_RUNTIMEI18N_H

#include <QString>

namespace M3I18n {

void setLanguageCode(const QString &languageCode);
QString languageCode();
QString translate(const QString &key, const QString &fallback);
QString translateSource(const QString &prefix, const QString &source);
QString text(const QString &source);
QString placeholder(const QString &source);

} // namespace M3I18n

#endif // M3_RUNTIMEI18N_H
