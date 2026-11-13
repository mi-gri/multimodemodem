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
/// English source of a text shown in the current language (reverse lookup
/// over the loaded translation, then the English catalogue).  The live
/// language switch captures captions once; texts set with uiText() in another
/// language would otherwise be kept as their own "source" and never change.
/// Returns the text unchanged when it is not a known translation.
QString sourceText(const QString &displayed);

} // namespace M3I18n

#endif // M3_RUNTIMEI18N_H
