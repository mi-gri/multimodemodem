// SPDX-License-Identifier: GPL-3.0-or-later
// Wavelog/Cloudlog API client against a local fake server.
#include "network/WavelogClient.h"

#include <QCoreApplication>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <functional>
#include <iostream>
#include <stdexcept>

static void check(bool ok, const char *why) { if (!ok) throw std::runtime_error(why); }

struct Request { QByteArray method, path, body; };

// Minimal HTTP/1.1 server: one request per connection, scripted replies.
class FakeServer : public QObject
{
public:
    std::function<QPair<int, QByteArray>(const Request &)> handler;
    QVector<Request> requests;
    FakeServer()
    {
        connect(&server, &QTcpServer::newConnection, this, [this]() {
            while (QTcpSocket *socket = server.nextPendingConnection()) {
                auto *buffer = new QByteArray;
                connect(socket, &QTcpSocket::readyRead, socket, [this, socket, buffer]() {
                    buffer->append(socket->readAll());
                    const int headerEnd = buffer->indexOf("\r\n\r\n");
                    if (headerEnd < 0) return;
                    const QByteArray header = buffer->left(headerEnd);
                    int length = 0;
                    for (const QByteArray &line : header.split('\n'))
                        if (line.toLower().startsWith("content-length:")) length = line.mid(15).trimmed().toInt();
                    if (buffer->size() < headerEnd + 4 + length) return;
                    Request request;
                    const QList<QByteArray> first = header.split('\n').first().trimmed().split(' ');
                    request.method = first.value(0);
                    request.path = first.value(1);
                    request.body = buffer->mid(headerEnd + 4, length);
                    requests.append(request);
                    const auto reply = handler(request);
                    socket->write("HTTP/1.1 " + QByteArray::number(reply.first) + " X\r\nContent-Type: application/json\r\n"
                                  "Content-Length: " + QByteArray::number(reply.second.size()) +
                                  "\r\nConnection: close\r\n\r\n" + reply.second);
                    socket->disconnectFromHost();
                    delete buffer;
                });
                connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
            }
        });
        server.listen(QHostAddress::LocalHost);
    }
    QString url() const { return QStringLiteral("http://127.0.0.1:%1/index.php/").arg(server.serverPort()); }
    QTcpServer server;
};

template <typename Signal>
static bool waitFor(WavelogClient *sender, Signal signal, int count = 1, int timeoutMs = 8000)
{
    int seen = 0;
    QEventLoop loop;
    auto connection = QObject::connect(sender, signal, &loop, [&]() { if (++seen >= count) loop.quit(); });
    QTimer::singleShot(timeoutMs, &loop, &QEventLoop::quit);
    if (seen < count) loop.exec();
    QObject::disconnect(connection);
    return seen >= count;
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    // Pure helpers.
    check(WavelogClient::endpoint(" https://log.example.com/index.php/ ", "qso").toString() ==
          "https://log.example.com/index.php/api/qso", "endpoint with index.php");
    check(WavelogClient::endpoint("log.example.com", "radio").toString() == "https://log.example.com/api/radio",
          "endpoint defaults to https");
    check(WavelogClient::isInsecureUrl("http://log.example.com/index.php"), "public http refused");
    check(WavelogClient::isInsecureUrl("http://93.184.216.34/index.php"), "public IP over http refused");
    check(!WavelogClient::isInsecureUrl("https://log.example.com/index.php") && !WavelogClient::isInsecureUrl("log.example.com"),
          "https allowed");
    check(!WavelogClient::isInsecureUrl("http://127.0.0.1:8086/index.php") && !WavelogClient::isInsecureUrl("http://localhost/") &&
          !WavelogClient::isInsecureUrl("http://192.168.1.20/wavelog") && !WavelogClient::isInsecureUrl("http://10.1.2.3") &&
          !WavelogClient::isInsecureUrl("http://wavelog/index.php") && !WavelogClient::isInsecureUrl("http://nas.local/wavelog") &&
          !WavelogClient::isInsecureUrl("http://[::1]:8080/") && !WavelogClient::isInsecureUrl("http://[fd12:3456::1]/") &&
          !WavelogClient::isInsecureUrl("http://[fe80::1]/"), "local http allowed");
    check(WavelogClient::isInsecureUrl("http://[2a00:1450:4001::200e]/index.php"), "public IPv6 over http refused");
    WavelogClient::Config config{"x", "KEY", "7"};
    {   // Plain http to a public host is refused unless explicitly allowed.
        WavelogClient guard;
        WavelogClient::Config publicHttp{"http://log.example.invalid/index.php", "KEY", "7"};
        QString refused;
        QObject::connect(&guard, &WavelogClient::stationsFetched, [&](bool, const QVector<WavelogClient::Station> &, const QString &e) { refused = e; });
        guard.fetchStations(publicHttp);
        check(refused.contains("unencrypted"), "public http refused without override");
        publicHttp.allowInsecureHttp = true; refused.clear();
        guard.fetchStations(publicHttp);
        waitFor(&guard, &WavelogClient::stationsFetched);
        check(!refused.contains("unencrypted"), "override sends over http");
    }
    const QJsonObject qso = QJsonDocument::fromJson(WavelogClient::qsoPayload(config, "<CALL:4>DL1X<EOR>\n")).object();
    check(qso.value("key").toString() == "KEY" && qso.value("station_profile_id").toString() == "7" &&
          qso.value("type").toString() == "adif" && qso.value("string").toString() == "<CALL:4>DL1X<EOR>", "qso payload");
    const QJsonObject radio = QJsonDocument::fromJson(WavelogClient::radioPayload("KEY", "MMM", 14074000, "ft8", 48.25)).object();
    check(radio.value("frequency").toVariant().toLongLong() == 14074000 && radio.value("mode").toString() == "FT8" &&
          radio.value("radio").toString() == "MMM" && qAbs(radio.value("power").toDouble() - 48.25) < 1e-9, "radio payload");
    check(!QJsonDocument::fromJson(WavelogClient::radioPayload("KEY", "MMM", 7074000, "", -1)).object().contains("power"),
          "radio payload without power");
    check(WavelogClient::parseQsoReply(201, R"({"status":"created","adif_count":1,"adif_errors":0,"messages":[]})", false, {}).ok,
          "wavelog created");
    check(WavelogClient::parseQsoReply(201, R"({"status":"created","imported_count":1,"messages":[]})", false, {}).ok,
          "cloudlog created");
    const auto dupe = WavelogClient::parseQsoReply(201, R"({"status":"created","adif_count":1,"adif_errors":1,"messages":["Duplicate for DL1X"]})", false, {});
    check(!dupe.ok && !dupe.retry && dupe.message.contains("Duplicate"), "rejected record reported");
    const auto key = WavelogClient::parseQsoReply(401, R"({"status":"failed","reason":"missing or wrong api key"})", true, "x");
    check(!key.ok && !key.retry && key.message.contains("api key"), "wrong key not retried");
    check(WavelogClient::parseQsoReply(0, {}, true, "Connection refused").retry, "network error retried");
    check(WavelogClient::parseQsoReply(503, "<html>", true, "x").retry, "server error retried");
    const auto html = WavelogClient::parseQsoReply(200, "<html>login</html>", false, {});
    check(!html.ok && !html.retry && html.message.contains("index.php"), "HTML reply hints at the URL");
    QString error;
    const auto stations = WavelogClient::parseStations(
        R"([{"station_id":"3","station_profile_name":"Home","station_callsign":"do3box","station_gridsquare":"jo31","station_active":"1"},
            {"station_id":4,"station_profile_name":"Portable","station_callsign":"DO3BOX/P","station_active":null}])", &error);
    check(stations.size() == 2 && stations[0].id == "3" && stations[0].callsign == "DO3BOX" && stations[0].active &&
          stations[1].id == "4" && !stations[1].active, "station list parsed");
    check(WavelogClient::parseStations(R"({"status":"failed","reason":"missing or invalid api key"})", &error).isEmpty() &&
          error.contains("invalid api key"), "station error reported");

    // Against the fake server.
    FakeServer server;
    check(server.server.isListening(), "fake server listening");
    config.url = server.url();
    WavelogClient client;
    client.setRetryDelays({50, 50});

    server.handler = [](const Request &request) -> QPair<int, QByteArray> {
        if (request.path.endsWith("/api/station_info/KEY"))
            return {200, R"([{"station_id":"7","station_profile_name":"Home","station_callsign":"DO3BOX","station_active":"1"}])"};
        return {404, "{}"};
    };
    QVector<WavelogClient::Station> fetched;
    QObject::connect(&client, &WavelogClient::stationsFetched, [&](bool ok, const QVector<WavelogClient::Station> &list, const QString &) {
        if (ok) fetched = list;
    });
    client.fetchStations(config);
    check(waitFor(&client, &WavelogClient::stationsFetched) && fetched.size() == 1 && fetched[0].id == "7", "stations fetched");
    check(server.requests.last().method == "GET" && server.requests.last().path == "/index.php/api/station_info/KEY", "station_info path");

    int calls = 0;
    server.handler = [&calls](const Request &) -> QPair<int, QByteArray> {
        return ++calls == 1 ? QPair<int, QByteArray>{503, "busy"}
                            : QPair<int, QByteArray>{201, R"({"status":"created","adif_count":1,"adif_errors":0})"};
    };
    QVector<bool> outcomes, retries;
    QObject::connect(&client, &WavelogClient::qsoUploaded, [&](const QString &, bool ok, const QString &, bool retry) {
        outcomes << ok; retries << retry;
    });
    client.uploadQso(config, "DL1X", "<CALL:4>DL1X<EOR>");
    check(waitFor(&client, &WavelogClient::qsoUploaded, 2), "upload finished after retry");
    check(outcomes == QVector<bool>({false, true}) && retries.first(), "server error retried, then created");
    const QJsonObject sent = QJsonDocument::fromJson(server.requests.last().body).object();
    check(server.requests.last().method == "POST" && server.requests.last().path == "/index.php/api/qso" &&
          sent.value("station_profile_id").toString() == "7" && sent.value("string").toString().contains("DL1X"), "qso posted");

    outcomes.clear(); retries.clear();
    const int before = server.requests.size();
    server.handler = [](const Request &) -> QPair<int, QByteArray> {
        return {401, R"({"status":"failed","reason":"station id does not belong to the API key owner."})"};
    };
    client.uploadQso(config, "DL2Y", "<CALL:4>DL2Y<EOR>");
    check(waitFor(&client, &WavelogClient::qsoUploaded), "rejected upload reported");
    QEventLoop settle; QTimer::singleShot(200, &settle, &QEventLoop::quit); settle.exec();
    check(outcomes == QVector<bool>({false}) && !retries.first() && server.requests.size() == before + 1, "rejection not retried");

    server.handler = [](const Request &) -> QPair<int, QByteArray> { return {200, R"({"status":"success"})"}; };
    bool radioOk = false;
    QObject::connect(&client, &WavelogClient::radioSent, [&](bool ok, const QString &) { radioOk = ok; });
    client.sendRadio(config, "MultiModeModem", 14074000, "FT8", -1);
    check(waitFor(&client, &WavelogClient::radioSent) && radioOk && server.requests.last().path == "/index.php/api/radio", "radio sent");

    std::cout << "PASS endpoint/payloads/replies/stations/upload+retry/rejection/radio\n";
    return 0;
}
