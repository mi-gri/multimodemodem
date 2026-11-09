// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef JS8INBOX_H
#define JS8INBOX_H

#include <QDateTime>
#include <QString>
#include <QVector>

struct Js8StoredMessage
{
    enum class Type { Unread, Read, Store, Delivered };

    int id = 0;
    Type type = Type::Unread;
    QDateTime utc;
    QString from;
    QString to;
    QString path;        // relay path, "K1ABC>N0CALL"
    QString text;
    QString grid;
    int snrDb = 0;
    double dialHz = 0.0;
    double offsetHz = 0.0;
    int submode = 0;
};

/**
 * @brief JS8 message store: received MSGs (inbox) and MSG TO: messages kept
 * for other stations, as JS8Call's inbox.db3 but in a JSON file.
 *
 * "Unread"/"Read" messages are addressed to the own station; "Store"
 * messages wait for their addressee and become "Delivered" once retrieved
 * with QUERY MSG n.  The file is written atomically after each change.
 */
class Js8Inbox
{
public:
    static constexpr int kMaxMessages = 1000;

    explicit Js8Inbox(const QString &path = QString());
    void setPath(const QString &path);
    QString path() const { return m_path; }
    bool load();
    bool save() const;

    int add(const Js8StoredMessage &message);   // returns the new id
    QVector<Js8StoredMessage> messages() const { return m_messages; }
    const Js8StoredMessage *find(int id) const;
    int unreadCount() const;
    void setType(int id, Js8StoredMessage::Type type);
    void remove(int id);

    /// Oldest stored message waiting for @p call (or its base call); -1 = none.
    int nextStoredIdFor(const QString &call) const;
    /// Stored message @p id if it is addressed to @p requester; marks it delivered.
    bool takeStoredFor(int id, const QString &requester, Js8StoredMessage *out);

    static QString typeName(Js8StoredMessage::Type type);

private:
    QString m_path;
    QVector<Js8StoredMessage> m_messages;
    int m_nextId = 1;
};

#endif // JS8INBOX_H
