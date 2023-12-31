#include "mysqldatabase.h"
#include <QSqlDatabase>
#include "Constants.h"
#include "FileManager.h"
#include <QSqlQuery>
#include <QSqlError>

MysqlDataBase::MysqlDataBase()
{
    loadDBConnectData();
}

void MysqlDataBase::loadDBConnectData()
{
    std::string filePath = (Constants::dir.baseDIR + Constants::dir.dataDIR + Constants::dir.configFileName);
    if (std::filesystem::exists(filePath) && !_isLoadConnectData)
    {
        FileManager::File file (filePath,FileManager::load,false,false);
        std::string dataBaseName;
        std::string host;
        std::string user;
        std::string pass;
        file.readStringInsideDelimiters('"', host);
        file.readStringInsideDelimiters('"', dataBaseName);
        file.readStringInsideDelimiters('"', user);
        file.readStringInsideDelimiters('"', pass);
        _host = QString::fromStdString(host);
        _dbName = QString::fromStdString(dataBaseName);
        _dbUser = QString::fromStdString(user);
        _dbPass = QString::fromStdString(pass);
        _isLoadConnectData = true;
        createDefaultTable();
    }
}

QSqlDatabase* MysqlDataBase::connecting(const QString &connectName)
{
    if (_isLoadConnectData)
    {
        QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL", connectName));
        db->setHostName(_host);
        db->setDatabaseName(_dbName);
        db->setUserName(_dbUser);
        db->setPassword(_dbPass);


        if (db->open())
        {
            return db;
        }
    }
    return nullptr;
}

void MysqlDataBase::disconnecting(const QString &connectName)
{
    QSqlDatabase::database(connectName).close();
    QSqlDatabase::removeDatabase(connectName);
}

bool MysqlDataBase::appendUser(const User &user)
{
    bool result{false};
    QString dbConnectName = "addedUser";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("INSERT INTO users (name, login, isAdmin, isDeleted, isBanned) VALUES(:name, :login, :isAdmin, :isDeleted, :isBannded)");
        query.bindValue(":name", user.getUserName());
        query.bindValue(":login", user.getUserLogin());
        query.bindValue(":isAdmin", user.isAdmin());
        query.bindValue(":isDeleted", user.isDeleted());
        query.bindValue(":isBannded", user.isBanned());
        db->transaction();
        if (query.exec())
        {
            query.prepare("SET @a = LAST_INSERT_ID()");
            if (query.exec())
            {
                query.prepare("INSERT INTO users_pass (users_id, user_pass)  VALUES(@a, :password)");
                query.bindValue(":password", user.getUserPassword());
                if (query.exec())
                {
                    db->commit();
                    result = true;
                } else
                {
                    qDebug() << "rollback and last query error: " << query.lastError();
                    db->rollback();
                }
            }
        } else
        {
            qDebug() << "rollback and last query error: " << query.lastError();
            db->rollback();
        }
        delete db;
    }
    disconnecting(dbConnectName);
    return result;
}

QJsonArray MysqlDataBase::getAllUser()
{
    QJsonArray users;
    QString dbConnectName = "getAllUser";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("SELECT * FROM users");
        query.exec();
        while(query.next())
        {
            QJsonObject user;
            user["name"] = query.value("name").toString();
            user["login"] = query.value("login").toString();
            user["is_admin"] = query.value("isAdmin").toBool();
            user["is_deleted"] = query.value("isDeleted").toBool();
            user["is_banned"] = query.value("isBanned").toBool();
            user["id"] = query.value("id").toInt();
            users.append(user);
        }
        delete db;
    }
    disconnecting(dbConnectName);
    return users;
}

User *MysqlDataBase::getUserByLogin(const QString &login)
{
    User *user{nullptr};
    QString dbConnectName = "getUserByLogin";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("SELECT * FROM users WHERE login = '" + login + "'");
        query.exec();
        if (query.next())
        {
            user = new User();
            user->setUserID(query.value("id").toInt());
            user->setUserLogin(query.value("login").toString());
            user->setUserName(query.value("name").toString());
            user->setIsAdmin(query.value("isAdmin").toBool());
            user->setIsBanned(query.value("isBanned").toBool());
            user->setIsDeleted(query.value("isDeleted").toBool());
        }


        delete db;
    }
    disconnecting(dbConnectName);
    return user;
}

bool MysqlDataBase::deleteUser(const User &user)
{
    bool result{false};
    QString dbConnectName = "deleteUser";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("UPDATE users SET isAdmin= :isAdmin, isBanned= :isBanned, isDeleted= :isDeleted   WHERE login= :login");
        query.bindValue(":isAdmin", false);
        query.bindValue(":isBanned", true);
        query.bindValue(":isDeleted", true);
        query.bindValue(":login", user.getUserLogin());
        if (query.exec())
        {
            result = true;
        } else
        {
            qDebug() << "last query error: " << query.lastError();
        }
        delete db;
    }
    disconnecting(dbConnectName);
    return result;
}

bool MysqlDataBase::updateUser(const User &user)
{
    bool result{false};
    QString dbConnectName = "updateUser";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("UPDATE users SET name= :name, isAdmin= :isAdmin, isBanned= :isBanned   WHERE login= :login");
        query.bindValue(":name", user.getUserName());
        query.bindValue(":isAdmin", user.isAdmin());
        query.bindValue(":isBanned", user.isBanned());
        query.bindValue(":login", user.getUserLogin());
        if (query.exec())
        {
            result = true;
        } else
        {
            qDebug() << "last query error: " << query.lastError();
        }
        delete db;
    }
    disconnecting(dbConnectName);
    return result;
}

User *MysqlDataBase::authentificate(const User &user)
{
    QString dbConnectName = "authentificate";
    User *retUser{nullptr};
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("SELECT * FROM users u INNER JOIN users_pass up ON u.login = :login AND up.user_pass = :password");
        query.bindValue(":login", user.getUserLogin());
        query.bindValue(":password",user.getUserPassword());
        query.exec();
        if (query.next())
        {
            retUser = new User();
            retUser->setUserID(query.value("id").toInt());
            retUser->setUserLogin(query.value("login").toString());
            retUser->setUserName(query.value("name").toString());
            retUser->setIsAdmin(query.value("isAdmin").toBool());
            retUser->setIsBanned(query.value("isBanned").toBool());
            retUser->setIsDeleted(query.value("isDeleted").toBool());
        } else
        {
            qDebug() << "Failed to prepare query:" << query.lastError().text();
        }
        delete db;
    } else
    {
        qDebug() << "db dont connect: " << user.getUserLogin();
    }
    disconnecting(dbConnectName);
    return retUser;

}

int MysqlDataBase::appendMessage( Message &message)
{
    int result{-1};
    QString dbConnectName = "appendMessage";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("INSERT INTO messages (message, isPrivate, author_id, recipient_id, `date`) VALUES( :message, :isPrivate, :authorId, :recipientId, NOW())");
        query.bindValue(":message", message.getMessage());
        query.bindValue(":isPrivate", message.isPrivateChat());
        query.bindValue(":authorId", message.getAuthorID());
        query.bindValue(":recipientId", message.getRecipientID());
        if (query.exec())
        {
            result = query.lastInsertId().toInt();
        } else
        {
            qDebug() << "Last query error: " << query.lastError();

        }
        delete db;
    }
    disconnecting(dbConnectName);
    return result;
}

bool MysqlDataBase::deleteMessageById(int id)
{
    bool result{false};
    QString dbConnectName = "deleteMessageById";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("DELETE FROM messages WHERE id = :id");
        query.bindValue(":id", id);
        if (query.exec())
        {
            result = true;
        } else
        {
            qDebug() << "last query error: " << query.lastError();
        }
        delete db;
    }
    disconnecting(dbConnectName);
    return result;
}

QJsonObject MysqlDataBase::getMessageById(int id)
{
    QString dbConnectName = "getMessageById";
    QJsonObject obj;
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        query.prepare("SELECT * FROM messages WHERE id = :id");
        query.bindValue(":id", id);
        query.exec();
        if (query.next())
        {
            obj["id"] = query.value("id").toInt();
            obj["message"] = query.value("message").toString();
            obj["is_private"] = query.value("isPrivate").toBool();
            obj["author_id"] = query.value("author_id").toInt();
            obj["recipient_id"] = query.value("recipient_id").toInt();
            obj["date"] = query.value("date").toString();
        }
        delete db;
    }
    disconnecting(dbConnectName);
    return obj;
}

QJsonArray MysqlDataBase::getAllMessages(MessagesType type)
{
    QJsonArray messages;
    QString dbConnectName = "getAllPublicMessage";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        QString prepare;
        switch (type) {
        case publ:
            prepare = "SELECT * FROM messages WHERE isPrivate=FALSE";
            break;
        case priv:
            prepare = "SELECT * FROM messages WHERE isPrivate=TRUE";
            break;
        case allmesage:
            prepare = "SELECT * FROM messages";
            break;
        default:
            break;
        }

        query.prepare(prepare);
        query.exec();
        while (query.next())
        {
            QJsonObject message;
            message["id"] = query.value("id").toInt();
            message["message"] = query.value("message").toString();
            message["is_private"] = query.value("isPrivate").toBool();
            message["author_id"] = query.value("author_id").toInt();
            message["recipient_id"] = query.value("recipient_id").toInt();
            message["date"] = query.value("date").toString();
            messages.append(message);
        }
        delete db;
    }
    disconnecting(dbConnectName);
    return messages;
}

void MysqlDataBase::createDefaultTable()
{
    QString dbConnectName = "createDefaultTable";
    if (auto db = connecting(dbConnectName))
    {
        QSqlQuery query(*db);
        QString queryStr{"CREATE TABLE IF NOT EXISTS users (\
        id int NOT NULL AUTO_INCREMENT,\
        name varchar(50) NOT NULL, \
        login varchar(50) NOT NULL,\
        isAdmin tinyint(1) DEFAULT NULL,\
        isDeleted tinyint(1) DEFAULT NULL,\
        isBanned tinyint(1) DEFAULT NULL,\
        PRIMARY KEY (id),\
        UNIQUE KEY login (login)\
         ) ENGINE=InnoDB"};

        query.exec(queryStr);
        queryStr = "CREATE TABLE IF NOT EXISTS users_pass (\
                    users_id int DEFAULT NULL,\
                    user_pass varchar(255) DEFAULT NULL,\
                    KEY users_id (users_id),\
                    CONSTRAINT users_pass_ibfk_1 FOREIGN KEY (users_id) REFERENCES users (id) ON DELETE CASCADE\
                    ) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;";
        query.exec(queryStr);

        queryStr = "CREATE TABLE IF NOT EXISTS messages (\
                id int NOT NULL AUTO_INCREMENT,\
                message varchar(255) DEFAULT NULL,\
                isPrivate tinyint(1) DEFAULT NULL,\
                author_id int NOT NULL,\
                recipient_id int DEFAULT NULL,\
                date timestamp NOT NULL,\
                PRIMARY KEY (id),\
                CONSTRAINT `messages_ibfk_1` FOREIGN KEY (`author_id`) REFERENCES `users` (`id`)\
                ) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;";
        query.exec(queryStr);
        delete db;
    }
    disconnecting(dbConnectName);
}
