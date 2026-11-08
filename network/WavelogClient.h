// SPDX-License-Identifier: GPL-3.0-or-later
#pragma once

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>
#include <QUrl>
#include <QVector>

class QNetworkReply;

/**
 * @brief Direct logging to Wavelog and Cloudlog through their HTTP API.
 *
 * Both share the API that WaveLogGate uses:
 *  - POST {url}/api/qso      {"key","station_profile_id","type":"adif","string"}
 *  - GET  {url}/api/station_info/{key}   station profiles of the key's owner
 *  - POST {url}/api/radio    {"key","radio","frequency"(Hz),"mode","power"(W)}
 *
 * {url} is the base address as entered by the operator, with index.php
 * when the server needs it (https://log.example.com/index.php).  The local
 * ADIF log stays authoritative; an upload failing for network reasons is
 * retried a few times, a rejected QSO is reported and not retried.
 */
class WavelogClient : public QObject
{
    Q_OBJECT
public:
    struct Config
    {
        QString url;
        QString apiKey;
        QString stationId;
        // The operator explicitly accepted sending the key over plain http.
        bool allowInsecureHttp = false;
        bool isComplete() const;
    };

    struct Station
    {
        QString id;
        QString name;
        QString callsign;
        QString grid;
        bool active = false;
    };

    struct QsoResult
    {
        bool ok = false;
        bool retry = false;     // network/server trouble, worth another try
        QString message;
    };

    explicit WavelogClient(QObject *parent = nullptr);

    static QUrl endpoint(const QString &baseUrl, const QString &path);
    /// Plain http to a host outside this computer / the local network would
    /// send the API key unencrypted; such URLs are refused.
    static bool isInsecureUrl(const QString &baseUrl);
    static QByteArray qsoPayload(const Config &config, const QString &adifRecord);
    static QByteArray radioPayload(const QString &apiKey, const QString &radio, qint64 frequencyHz,
                                   const QString &mode, double watts);
    static QVector<Station> parseStations(const QByteArray &body, QString *error);
    static QsoResult parseQsoReply(int httpStatus, const QByteArray &body, bool networkError,
                                   const QString &networkErrorText);

    void uploadQso(const Config &config, const QString &callsign, const QString &adifRecord);
    void fetchStations(const Config &config);
    void sendRadio(const Config &config, const QString &radio, qint64 frequencyHz,
                   const QString &mode, double watts);

    /// Delays before the retries of a failed upload (ms); tests shorten them.
    void setRetryDelays(const QVector<int> &delaysMs) { m_retryDelaysMs = delaysMs; }

signals:
    void qsoUploaded(const QString &callsign, bool ok, const QString &message, bool willRetry);
    void stationsFetched(bool ok, const QVector<WavelogClient::Station> &stations, const QString &error);
    void radioSent(bool ok, const QString &error);

private:
    void postQso(const Config &config, const QString &callsign, const QString &adifRecord, int attempt);
    QNetworkReply *postJson(const QUrl &url, const QByteArray &body);
    static QString insecureUrlMessage();

    QNetworkAccessManager m_network;
    QVector<int> m_retryDelaysMs{30000, 120000, 600000};
};
