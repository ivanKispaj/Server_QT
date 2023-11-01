
//
// Message.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "Message.h"

int Message::getAuthorID() const
{
    return _authorID;
}

int Message::getRecipientID() const
{
    return _recipientID;
}

long long Message::getId() const
{
    return _id;
}

QDateTime Message::getDate() const
{
    return _date;
}

bool Message::isPrivateChat() const
{
    return _isPrivate;
}

const QString &Message::getMessage() const
{
    return _message;
}

// IBinaryCodable methods --------------
QJsonObject Message::encodeToJson()
{
    QJsonObject jsonObject;
    jsonObject["message"] = _message;
    jsonObject["author_id"] = _authorID;
    jsonObject["recipient_id"] = _recipientID;
    jsonObject["date"] = _date.toString();
    jsonObject["id"] = _id;
    jsonObject["is_private"] = _isPrivate;
    return jsonObject;
}

void Message::decodeFromJson(const QJsonObject &data)
{
    _message = data["message"].toString();
    _authorID = data["author_id"].toInt();
    _recipientID = data["recipient_id"].toInt();
    _id = data["id"].toInt();
    _date = QDateTime::fromString(data["date"].toString());
    _isPrivate = data["is_private"].toBool();
}

void Message::setMessage(const QString &message)
{
    _message = message;
}

void Message::setAuthorID(int authorID)
{
    _authorID = authorID;
}

void Message::setRecipientID(int recipientID)
{
    _recipientID = recipientID;
}

void Message::setId(long long id)
{
    _id = id;
}

void Message::setDateMessage(const QDateTime &date)
{
    _date = date;
}

Message &Message::operator=(const Message &message)
{
    _isPrivate = message._isPrivate;
    _message = message.getMessage();
    _authorID = message.getAuthorID();
    _recipientID = message.getRecipientID();
    _date = message.getDate();
    _id = message.getId();
    return *this;
}
