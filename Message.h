//
// Message.h
// 2023-03-11
// Ivan Konishchev
//

#pragma once
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QVariant>
#include "IJsonCodable.h"
#include <QString>

struct Message: public IJsonCodable
{
public:
    /// @brief default init
    Message() = default;

    /// @brief default deinit
    ~Message() = default;

    /// @brief init
    /// @param message string message
    /// @param authorID int author ID
    /// @param recipientID int recipient ID
    /// @param privateChat bool -> true - private chat / false - public chat
    Message(const QString &message, int authorID, int recipientID, bool privateChat = false) : _isPrivate(privateChat),
        _message(message),
        _authorID(authorID),
        _recipientID(recipientID) {}

    // Getters

    /// @brief Returns the ID of the author of the message
    /// @return int author ID
    int getAuthorID() const;

    /// @brief Returns the ID of the recipient of the message
    /// @return int recipient ID
    int getRecipientID() const;

    /// @brief Returns the ID of the message
    /// @return int
    long long int getId() const;

    /// @brief Unix Timestamp returns the date in seconds from 1970
    /// @return long long int -> timestamp date in seconds
    QDateTime getDate() const;

    bool isPrivateChat() const;

    /// @brief Returns a message as a string
    /// @return string
    const QString &getMessage() const;

    // IBinaryCodable methods --------------
    QJsonObject encodeToJson();

    void decodeFromJson(const QJsonObject &data);

    // Setters

    /// @brief Assigns a message to an object
    /// @param message string
    void setMessage(const QString &message);

    /// @brief Assigns a author ID to an object
    /// @param authorID int -> ID of the author of the message
    void setAuthorID(int authorID);

    /// @brief Assigns a recipient ID to an object
    /// @param recipientID int -> ID of the message recipient
    void setRecipientID(int recipientID);

    void setId(long long int id);

    /// @brief set date Unix Timestamp
    void setDateMessage(const QDateTime &date);

    /// @brief Overloading the assignment operator "="
    /// @param message Message to assign struct "Message"
    /// @return Message
    Message &operator=(const Message &message);

private:
    QString _message;
    int _authorID = 0;
    int _recipientID = 0;
    QDateTime _date;
    int _id{0};
    bool _isPrivate{false};
};
