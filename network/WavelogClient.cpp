// SPDX-License-Identifier: GPL-3.0-or-later
#include "network/WavelogClient.h"

#include "utils/ProductName.h"
#include "MadModemVersion.h"

#include <QHostAddress>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPointer>
#include <QTimer>

namespace {
constexpr int kTimeoutMs = 20000;

QString jsonText(const QJsonValue &value)
{
    if (value.isString()) return value.toString().trimmed();
    if (value.isDouble()) return QString::number(value.toDouble(), 'f', 0);
    return QString();
}

bool jsonFlag(const QJsonValue &value)
{
    if (value.isBool()) return value.toBool();
    if (value.isDouble()) return value.toInt() != 0;
    const QString text = value.toString().trimmed();
    return text == QStringLiteral("1") || text.compare(QStringLiteral("true"), Qt::CaseInsensitive) == 0;
}

QString reasonFrom(const QJsonObject &object)
{
    QString reason = object.value(QStringLiteral("reason")).toString().trimmed();
    QStringList messages;
    const QJsonValue raw = object.value(QStringLiteral("messages"));
    if (raw.isArray()) {
        for (const QJsonValue &message : raw.toArray()) {
            const QString text = message.toString().trimmed();
            if (!text.isEmpty()) messages << text;
        }
    } else if (raw.isString() && !raw.toString().trimmed().isEmpty()) {
        messages << raw.toString().trimmed();
    }
    if (!messages.isEmpty()) {
        reason += (reason.isEmpty() ? QString() : QStringLiteral(": ")) + messages.join(QStringLiteral("; "));
    }
    return reason;
}

QString notJsonHint()
{
    return QStringLiteral("server reply is not JSON - check the URL (many servers need /index.php at the end)");
}
}

bool WavelogClient::Config::isComplete() const
{
    return !url.trimmed().isEmpty() && !apiKey.trimmed().isEmpty() && !stationId.trimmed().isEmpty();
}

WavelogClient::WavelogClient(QObject *parent)
    : QObject(parent)
{
}

QUrl WavelogClient::endpoint(const QString &baseUrl, const QString &path)
{
    QString base = baseUrl.trimmed();
    while (base.endsWith(QLatin1Char('/'))) base.chop(1);
    if (!base.contains(QStringLiteral("://"))) base.prepend(QStringLiteral("https://"));
    return QUrl(base + QStringLiteral("/api/") + path);
}

bool WavelogClient::isInsecureUrl(const QString &baseUrl)
{
    const QUrl url = endpoint(baseUrl, QString());
    if (url.scheme().compare(QStringLiteral("http"), Qt::CaseInsensitive) != 0) return false;
    const QString host = url.host().toLower();
    if (host == QStringLiteral("localhost") || host.endsWith(QStringLiteral(".local")) ||
        host.endsWith(QStringLiteral(".lan")) || host.endsWith(QStringLiteral(".home.arpa")) || !host.contains(QLatin1Char('.')))
        return false;
    QHostAddress address;
    if (!address.setAddress(host)) return true;   // public DNS name over plain http
    if (address.isLoopback() || address.isLinkLocal()) return false;
    const QPair<QHostAddress, int> privateNets[] = {
        QHostAddress::parseSubnet(QStringLiteral("10.0.0.0/8")),
        QHostAddress::parseSubnet(QStringLiteral("172.16.0.0/12")),
        QHostAddress::parseSubnet(QStringLiteral("192.168.0.0/16")),
        QHostAddress::parseSubnet(QStringLiteral("100.64.0.0/10")),
        QHostAddress::parseSubnet(QStringLiteral("fc00::/7")),
    };
    for (const auto &net : privateNets)
        if (address.isInSubnet(net)) return false;
    return true;
}

QString WavelogClient::insecureUrlMessage()
{
    return QStringLiteral("refused: plain http would send the API key unencrypted over the internet - use https");
}

QByteArray WavelogClient::qsoPayload(const Config &config, const QString &adifRecord)
{
    QJsonObject object;
    object.insert(QStringLiteral("key"), config.apiKey.trimmed());
    object.insert(QStringLiteral("station_profile_id"), config.stationId.trimmed());
    object.insert(QStringLiteral("type"), QStringLiteral("adif"));
    object.insert(QStringLiteral("string"), adifRecord.trimmed());
    return QJsonDocument(object).toJson(QJsonDocument::Compact);
}

QByteArray WavelogClient::radioPayload(const QString &apiKey, const QString &radio, qint64 frequencyHz,
                                       const QString &mode, double watts)
{
    QJsonObject object;
    object.insert(QStringLiteral("key"), apiKey.trimmed());
    object.insert(QStringLiteral("radio"), radio.trimmed().left(250));
    object.insert(QStringLiteral("frequency"), frequencyHz);
    if (!mode.trimmed().isEmpty()) object.insert(QStringLiteral("mode"), mode.trimmed().toUpper().left(10));
    if (watts >= 0.0) object.insert(QStringLiteral("power"), qRound(watts * 1000.0) / 1000.0);
    return QJsonDocument(object).toJson(QJsonDocument::Compact);
}

QVector<WavelogClient::Station> WavelogClient::parseStations(const QByteArray &body, QString *error)
{
    QVector<Station> stations;
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(body, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        if (error != nullptr) *error = notJsonHint();
        return stations;
    }
    if (document.isObject()) {
        if (error != nullptr) {
            const QString reason = reasonFrom(document.object());
            *error = reason.isEmpty() ? QStringLiteral("unexpected reply") : reason;
        }
        return stations;
    }
    for (const QJsonValue &value : document.array()) {
        const QJsonObject object = value.toObject();
        Station station;
        station.id = jsonText(object.value(QStringLiteral("station_id")));
        station.name = object.value(QStringLiteral("station_profile_name")).toString().trimmed();
        station.callsign = object.value(QStringLiteral("station_callsign")).toString().trimmed().toUpper();
        station.grid = object.value(QStringLiteral("station_gridsquare")).toString().trimmed().toUpper();
        station.active = jsonFlag(object.value(QStringLiteral("station_active")));
        if (!station.id.isEmpty()) stations.append(station);
    }
    if (stations.isEmpty() && error != nullptr) *error = QStringLiteral("no station profiles for this API key");
    return stations;
}

WavelogClient::QsoResult WavelogClient::parseQsoReply(int httpStatus, const QByteArray &body, bool networkError,
                                                      const QString &networkErrorText)
{
    QsoResult result;
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(body, &parseError);
    const bool json = parseError.error == QJsonParseError::NoError && document.isObject();
    const QJsonObject object = document.object();

    if (httpStatus == 0 || (networkError && !json)) {
        // No answer at all (DNS, refused, timeout, TLS): try again later.
        result.retry = true;
        result.message = networkErrorText.isEmpty() ? QStringLiteral("no reply from server") : networkErrorText;
        return result;
    }
    if (httpStatus >= 500) {
        result.retry = true;
        result.message = QStringLiteral("server error HTTP %1").arg(httpStatus);
        return result;
    }
    if (!json) {
        result.message = notJsonHint();
        return result;
    }
    const QString status = object.value(QStringLiteral("status")).toString().trimmed().toLower();
    const QString reason = reasonFrom(object);
    // Wavelog counts rejected records in adif_errors, Cloudlog reports
    // imported_count; both answer HTTP 201 "created" even for a duplicate.
    const int adifErrors = object.value(QStringLiteral("adif_errors")).toInt(0);
    const bool importedNone = object.contains(QStringLiteral("imported_count")) &&
                              object.value(QStringLiteral("imported_count")).toInt(0) == 0;
    if (httpStatus == 201 && status == QStringLiteral("created") && adifErrors == 0 && !importedNone) {
        result.ok = true;
        result.message = reason;
        return result;
    }
    result.message = reason.isEmpty() ? QStringLiteral("rejected (HTTP %1, %2)").arg(httpStatus).arg(status)
                                      : reason;
    return result;
}

QNetworkReply *WavelogClient::postJson(const QUrl &url, const QByteArray &body)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setRawHeader("Accept", "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QStringLiteral("%1/%2").arg(ProductName::longName(), QStringLiteral(MADMODEM_VERSION_STRING)));
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    request.setTransferTimeout(kTimeoutMs);
#endif
    return m_network.post(request, body);
}

void WavelogClient::uploadQso(const Config &config, const QString &callsign, const QString &adifRecord)
{
    postQso(config, callsign, adifRecord, 0);
}

void WavelogClient::postQso(const Config &config, const QString &callsign, const QString &adifRecord, int attempt)
{
    if (!config.allowInsecureHttp && isInsecureUrl(config.url)) {
        emit qsoUploaded(callsign, false, insecureUrlMessage(), false);
        return;
    }
    QNetworkReply *reply = postJson(endpoint(config.url, QStringLiteral("qso")), qsoPayload(config, adifRecord));
    connect(reply, &QNetworkReply::finished, this, [this, reply, config, callsign, adifRecord, attempt]() {
        reply->deleteLater();
        const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QsoResult result = parseQsoReply(status, reply->readAll(),
                                               reply->error() != QNetworkReply::NoError, reply->errorString());
        const bool willRetry = !result.ok && result.retry && attempt < m_retryDelaysMs.size();
        emit qsoUploaded(callsign, result.ok, result.message, willRetry);
        if (willRetry) {
            QPointer<WavelogClient> self(this);
            QTimer::singleShot(m_retryDelaysMs.at(attempt), this, [self, config, callsign, adifRecord, attempt]() {
                if (self) self->postQso(config, callsign, adifRecord, attempt + 1);
            });
        }
    });
}

void WavelogClient::fetchStations(const Config &config)
{
    if (!config.allowInsecureHttp && isInsecureUrl(config.url)) {
        emit stationsFetched(false, {}, insecureUrlMessage());
        return;
    }
    QNetworkRequest request(endpoint(config.url, QStringLiteral("station_info/") +
                                                 QString::fromLatin1(QUrl::toPercentEncoding(config.apiKey.trimmed()))));
    request.setRawHeader("Accept", "application/json");
    request.setHeader(QNetworkRequest::UserAgentHeader,
                      QStringLiteral("%1/%2").arg(ProductName::longName(), QStringLiteral(MADMODEM_VERSION_STRING)));
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    request.setTransferTimeout(kTimeoutMs);
#endif
    QNetworkReply *reply = m_network.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QByteArray body = reply->readAll();
        QString error;
        QVector<Station> stations;
        if (status == 0) {
            error = reply->errorString();
        } else {
            stations = parseStations(body, &error);
            if (stations.isEmpty() && status >= 400 && error.isEmpty()) error = QStringLiteral("HTTP %1").arg(status);
        }
        emit stationsFetched(!stations.isEmpty(), stations, error);
    });
}

void WavelogClient::sendRadio(const Config &config, const QString &radio, qint64 frequencyHz,
                              const QString &mode, double watts)
{
    if (!config.allowInsecureHttp && isInsecureUrl(config.url)) {
        emit radioSent(false, insecureUrlMessage());
        return;
    }
    QNetworkReply *reply = postJson(endpoint(config.url, QStringLiteral("radio")),
                                    radioPayload(config.apiKey, radio, frequencyHz, mode, watts));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QJsonObject object = QJsonDocument::fromJson(reply->readAll()).object();
        const bool ok = status >= 200 && status < 300 &&
                        object.value(QStringLiteral("status")).toString() == QStringLiteral("success");
        QString error;
        if (!ok) {
            error = status == 0 ? reply->errorString() : reasonFrom(object);
            if (error.isEmpty()) error = QStringLiteral("HTTP %1").arg(status);
        }
        emit radioSent(ok, error);
    });
}
