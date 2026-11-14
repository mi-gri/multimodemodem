#include "CtyUpdater.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

namespace {
constexpr qint64 kMaxDownloadBytes = 8 * 1024 * 1024;
constexpr int kTimeoutMs = 30000;
}

CtyUpdater::CtyUpdater(QObject *parent)
    : QObject(parent)
    , m_network(new QNetworkAccessManager(this))
{
}

void CtyUpdater::start(bool userInitiated)
{
    if (m_reply != nullptr) return;
    m_userInitiated = userInitiated;
    m_tooLarge = false;
    QNetworkRequest request{QUrl(CtyCountryFile::downloadUrl())};
    request.setHeader(QNetworkRequest::UserAgentHeader, QStringLiteral("MultiModeModem"));
    // HTTPS only; redirects may not downgrade to HTTP. TLS is verified by Qt.
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    request.setTransferTimeout(kTimeoutMs);
#endif
    m_reply = m_network->get(request);
    connect(m_reply, &QNetworkReply::downloadProgress, this, [this](qint64 received, qint64) {
        if (received > kMaxDownloadBytes && m_reply != nullptr) {
            m_tooLarge = true;
            m_reply->abort();
        }
    });
    connect(m_reply, &QNetworkReply::finished, this, &CtyUpdater::handleFinished);
}

void CtyUpdater::handleFinished()
{
    QNetworkReply *reply = m_reply;
    m_reply = nullptr;
    if (reply == nullptr) return;
    reply->deleteLater();
    if (m_tooLarge) {
        emit finished(false, false, QStringLiteral("cty.csv download aborted: larger than 8 MB"), m_userInitiated);
        return;
    }
    if (reply->error() != QNetworkReply::NoError) {
        emit finished(false, false, QStringLiteral("cty.csv download failed: %1").arg(reply->errorString()), m_userInitiated);
        return;
    }
    if (reply->url().scheme() != QStringLiteral("https")) {
        emit finished(false, false, QStringLiteral("cty.csv download rejected: not served over HTTPS"), m_userInitiated);
        return;
    }
    const CtyCountryFile::InstallResult result = CtyCountryFile::installDownloaded(reply->readAll());
    emit finished(result.ok, result.updated, result.message, m_userInitiated);
}
