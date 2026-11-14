#ifndef CTYUPDATER_H
#define CTYUPDATER_H

#include "CtyCountryFile.h"

#include <QObject>
#include <QPointer>

class QNetworkAccessManager;
#include <QNetworkReply>

/**
 * @brief Downloads the current country-files.com cty.csv over verified HTTPS
 * and hands it to CtyCountryFile::installDownloaded(), which checks it before
 * it replaces the data in use. One download at a time, size and time limited.
 */
class CtyUpdater final : public QObject
{
    Q_OBJECT
public:
    explicit CtyUpdater(QObject *parent = nullptr);
    bool isRunning() const { return m_reply != nullptr; }

public slots:
    void start(bool userInitiated);

signals:
    void finished(bool ok, bool updated, const QString &message, bool userInitiated);

private:
    void handleFinished();

    QNetworkAccessManager *m_network = nullptr;
    QPointer<QNetworkReply> m_reply;
    bool m_userInitiated = false;
    bool m_tooLarge = false;
};

#endif // CTYUPDATER_H
