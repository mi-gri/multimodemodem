// SPDX-License-Identifier: GPL-3.0-or-later
#include "Js8Inbox.h"

#include "Js8Message.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSaveFile>

namespace {
Js8StoredMessage::Type typeFromName(const QString &name)
{
    if (name == QLatin1String("READ")) return Js8StoredMessage::Type::Read;
    if (name == QLatin1String("STORE")) return Js8StoredMessage::Type::Store;
    if (name == QLatin1String("DELIVERED")) return Js8StoredMessage::Type::Delivered;
    return Js8StoredMessage::Type::Unread;
}

bool sameStation(const QString &a, const QString &b)
{
    const QString x = a.trimmed().toUpper();
    const QString y = b.trimmed().toUpper();
    if (x.isEmpty() || y.isEmpty()) return false;
    return x == y || Js8Message::baseCallsign(x) == Js8Message::baseCallsign(y);
}
} // namespace

Js8Inbox::Js8Inbox(const QString &path) : m_path(path) {}

void Js8Inbox::setPath(const QString &path) { m_path = path; }

QString Js8Inbox::typeName(Js8StoredMessage::Type type)
{
    switch (type) {
    case Js8StoredMessage::Type::Read: return QStringLiteral("READ");
    case Js8StoredMessage::Type::Store: return QStringLiteral("STORE");
    case Js8StoredMessage::Type::Delivered: return QStringLiteral("DELIVERED");
    case Js8StoredMessage::Type::Unread:
    default: return QStringLiteral("UNREAD");
    }
}

bool Js8Inbox::load()
{
    m_messages.clear();
    m_nextId = 1;
    if (m_path.isEmpty()) return false;
    QFile file(m_path);
    if (!file.exists()) return true;
    if (!file.open(QIODevice::ReadOnly)) return false;
    const QJsonDocument doc = QJsonDocument::fromJson(file.read(16 * 1024 * 1024));
    if (!doc.isObject()) return false;
    const QJsonArray list = doc.object().value(QStringLiteral("messages")).toArray();
    for (const QJsonValue &v : list) {
        const QJsonObject o = v.toObject();
        Js8StoredMessage m;
        m.id = o.value(QStringLiteral("id")).toInt();
        if (m.id <= 0) continue;
        m.type = typeFromName(o.value(QStringLiteral("type")).toString());
        m.utc = QDateTime::fromString(o.value(QStringLiteral("utc")).toString(), Qt::ISODate).toUTC();
        m.from = o.value(QStringLiteral("from")).toString();
        m.to = o.value(QStringLiteral("to")).toString();
        m.path = o.value(QStringLiteral("path")).toString();
        m.text = o.value(QStringLiteral("text")).toString();
        m.grid = o.value(QStringLiteral("grid")).toString();
        m.snrDb = o.value(QStringLiteral("snr")).toInt();
        m.dialHz = o.value(QStringLiteral("dial")).toDouble();
        m.offsetHz = o.value(QStringLiteral("offset")).toDouble();
        m.submode = o.value(QStringLiteral("submode")).toInt();
        m_messages << m;
        m_nextId = qMax(m_nextId, m.id + 1);
    }
    return true;
}

bool Js8Inbox::save() const
{
    if (m_path.isEmpty()) return false;
    QDir().mkpath(QFileInfo(m_path).absolutePath());
    QJsonArray list;
    for (const Js8StoredMessage &m : m_messages) {
        QJsonObject o;
        o.insert(QStringLiteral("id"), m.id);
        o.insert(QStringLiteral("type"), typeName(m.type));
        o.insert(QStringLiteral("utc"), m.utc.toUTC().toString(Qt::ISODate));
        o.insert(QStringLiteral("from"), m.from);
        o.insert(QStringLiteral("to"), m.to);
        if (!m.path.isEmpty()) o.insert(QStringLiteral("path"), m.path);
        o.insert(QStringLiteral("text"), m.text);
        if (!m.grid.isEmpty()) o.insert(QStringLiteral("grid"), m.grid);
        o.insert(QStringLiteral("snr"), m.snrDb);
        o.insert(QStringLiteral("dial"), m.dialHz);
        o.insert(QStringLiteral("offset"), m.offsetHz);
        o.insert(QStringLiteral("submode"), m.submode);
        list << o;
    }
    QJsonObject root;
    root.insert(QStringLiteral("version"), 1);
    root.insert(QStringLiteral("messages"), list);
    QSaveFile file(m_path);
    if (!file.open(QIODevice::WriteOnly)) return false;
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return file.commit();
}

int Js8Inbox::add(const Js8StoredMessage &message)
{
    Js8StoredMessage m = message;
    m.id = m_nextId++;
    m_messages << m;
    // Drop the oldest finished messages first, then the oldest of any kind.
    while (m_messages.size() > kMaxMessages) {
        int victim = 0;
        for (int i = 0; i < m_messages.size(); ++i) {
            const auto t = m_messages.at(i).type;
            if (t == Js8StoredMessage::Type::Read || t == Js8StoredMessage::Type::Delivered) { victim = i; break; }
        }
        m_messages.removeAt(victim);
    }
    save();
    return m.id;
}

const Js8StoredMessage *Js8Inbox::find(int id) const
{
    for (const Js8StoredMessage &m : m_messages)
        if (m.id == id) return &m;
    return nullptr;
}

int Js8Inbox::unreadCount() const
{
    int n = 0;
    for (const Js8StoredMessage &m : m_messages)
        if (m.type == Js8StoredMessage::Type::Unread) ++n;
    return n;
}

void Js8Inbox::setType(int id, Js8StoredMessage::Type type)
{
    for (Js8StoredMessage &m : m_messages) {
        if (m.id == id && m.type != type) {
            m.type = type;
            save();
            return;
        }
    }
}

void Js8Inbox::remove(int id)
{
    for (int i = 0; i < m_messages.size(); ++i) {
        if (m_messages.at(i).id == id) {
            m_messages.removeAt(i);
            save();
            return;
        }
    }
}

int Js8Inbox::nextStoredIdFor(const QString &call) const
{
    for (const Js8StoredMessage &m : m_messages)
        if (m.type == Js8StoredMessage::Type::Store && !m.text.trimmed().isEmpty() && sameStation(m.to, call))
            return m.id;
    return -1;
}

bool Js8Inbox::takeStoredFor(int id, const QString &requester, Js8StoredMessage *out)
{
    for (Js8StoredMessage &m : m_messages) {
        if (m.id != id) continue;
        if (m.type != Js8StoredMessage::Type::Store || m.text.trimmed().isEmpty() || !sameStation(m.to, requester))
            return false;
        m.type = Js8StoredMessage::Type::Delivered;
        save();
        if (out != nullptr) *out = m;
        return true;
    }
    return false;
}
