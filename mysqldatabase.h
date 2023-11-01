
#pragma once
#include <QString>
#include <QVariantMap>
#include "User.h"
#include "Message.h"
#include <QJsonArray>
class MysqlDataBase {
private:
    QString _host;
    QString _dbName;
    QString _dbUser;
    QString _dbPass;
    bool _isLoadConnectData{false};
public:

    enum MessagesType
    {
        publ,priv,allmesage
    };

    MysqlDataBase();
    ~MysqlDataBase() = default;
    void loadDBConnectData();
    QSqlDatabase* connecting(const QString &connectName); // Метод для установки соединения с базой данных
    void disconnecting(const QString &connectName); // Метод для разрыва соединения с базой данных

    // Users methods....
    bool appendUser(const User &user);
    QJsonArray getAllUser();
    User* getUserByLogin(const QString &login);
    bool deleteUser(const User &user);
    bool updateUser(const User &user);
    User* authentificate(const User &user);

    // Messages methods....
    int appendMessage(Message &message);
    User* updateMessage(const Message &message);
    bool deleteMessageById(int id);
    QJsonObject getMessageById(int id);
    QJsonArray getAllMessages(MessagesType type);

};

