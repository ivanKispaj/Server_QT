//
// TCP_Server.h
// 2023-28-10
// Ivan Konishchev
//

#include "TCPServer.h"
#include <QJsonArray>

void TCPServer::closeAllConnections()
{
    // Отключение всех активных соединений
    for (QTcpSocket* socket : _clientConnections) {
        socket->disconnectFromHost();
        socket->waitForDisconnected(); // Ожидание завершения отключения
        socket->deleteLater();
    }
    _clientConnections.clear();
}

TCPServer::~TCPServer()
{
    {
        closeAllConnections();
    }
}

bool TCPServer::isConnected()
{
    if (this->listen(_host, _port))
    {
        return true;
    }
    return false;
}

void TCPServer::handleUpdateUI(const QString &message)
{
    emit updateUI(message);
    // Обновление UI с использованием полученного сообщения
}

void TCPServer::disconnectClient(QTcpSocket *socket)
{
    if (!_clientConnections.empty())
    {
        _clientConnections.removeOne(socket);
        emit updateUI(tr("Клиент отключен"));
    }
}

void TCPServer::processData(QTcpSocket *socket, int command, const QJsonObject &data)
{
    MysqlDataBase *db = new MysqlDataBase();
    switch(command)
    {
    case ADD_USER:
    {
        User user;
        user.decodeFromJson(data["user"].toObject());
        db->appendUser(user);
        User *addeduser = db->getUserByLogin(user.getUserLogin());
        QJsonObject request;
        request["auth_state"] = false;
        if (addeduser)
        {
            request["user"] = addeduser->encodeToJson();
            request.insert("auth_state", true);
            delete addeduser;
        }
        QJsonDocument doc(request);
        QByteArray dat = doc.toJson();
        socket->write(dat);
        socket->waitForBytesWritten();
    }
    break;
    case ADD_MESSAGE:
    {
        Message message;
        message.decodeFromJson(data["message"].toObject());
        int id = db->appendMessage(message);
        QJsonObject messageObj = db->getMessageById(id);
        QJsonObject request;
        request["state"] = false;
        if (!messageObj.empty())
        {
            request["state"] = true;

            if (messageObj["is_private"].isBool())
            {
                request["p_message"] = messageObj;
            } else
            {
                request["message"] = messageObj;
            }

        }
        QJsonDocument doc(request);
        QByteArray dat = doc.toJson();
        socket->write(dat);
        socket->waitForBytesWritten();
    }
    break;
    case IS_UNIQUE_LOGIN:
    {
        //  db->isUniqueUser(data["login"].toString());
    }
    break;
    case GET_ALL_USERS:
    {
        QJsonArray users = db->getAllUser();
        QJsonObject sendData;
        sendData["state"] = false;
        if (users.count() > 0)
        {
            sendData["state"] = true;
            sendData["users"] = users;
        }
        QJsonDocument doc(sendData);
        socket->write(doc.toJson());
        socket->waitForBytesWritten();
    }
    break;
    case GET_USER_BY_LOGIN:
    {

    }
    break;
    case USERS_COUNT:
    {
        //   int count = db->getCount("users");
    }
    break;
    case GET_ALL_PRIVATE_MESSAGE:
    {
        QJsonArray messages = db->getAllMessages(MysqlDataBase::priv);
        QJsonObject sendData;
        sendData["state"] = false;
        if (messages.count() > 0)
        {
            sendData["state"] = true;
            sendData["p_messages"] = messages;
        }
        QJsonDocument doc(sendData);
        socket->write(doc.toJson());
        socket->waitForBytesWritten();
    }
    break;
    case UPDATE_USER_DATA:
    {

    }
    break;
    case DELETE_MESSAGE_BY_ID:
    {

    }
    break;
    case GET_ALL_PUBLIC_MESSAGES:
    {
        QJsonArray messages = db->getAllMessages(MysqlDataBase::publ);
        QJsonObject sendData;
        sendData["state"] = false;
        if (messages.count() > 0)
        {
            sendData["state"] = true;
            sendData["messages"] = messages;
        }
        QJsonDocument doc(sendData);
        socket->write(doc.toJson());
        socket->waitForBytesWritten();
    }
    break;
    case AUTHENTIFICATE:
    {
        QJsonObject userObj = data["user"].toObject();
        User user;
        user.decodeFromJson(userObj);
        User *authUser = db->authentificate(user);
        QJsonObject request;
        request["auth_state"] = false;
        if (authUser)
        {
            if (!authUser->isDeleted())
            {
                request["user"] = authUser->encodeToJson();
                request["auth_state"] = true;
                delete authUser;
            }
        }
        QJsonDocument doc(request);
        QByteArray dat = doc.toJson();
        socket->write(dat);
        socket->waitForBytesWritten();

    }
    break;
    default:
        break;
    }
    delete db;
}

void TCPServer::incomingConnection(qintptr socketDescriptor)
{
    // Создание нового сокета для входящего подключения
    QTcpSocket* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    _clientConnections.append(socket);
    // Создание отдельного потока для обработки подключения
    QThread* thread = new QThread(this);
    ConnectionHandler* handler = new ConnectionHandler(socket);
    handler->moveToThread(thread);
    emit updateUI(tr("connect client ") + QString::number(socket->socketDescriptor()));
    connect(thread, &QThread::finished, handler, &ConnectionHandler::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::started, handler, &ConnectionHandler::readData);
    connect(handler, &ConnectionHandler::messageReceived, this, &TCPServer::handleUpdateUI);
    connect(handler, &ConnectionHandler::disconnectClient, this, &TCPServer::disconnectClient);
    connect(handler, &ConnectionHandler::receiveData, this, &TCPServer::processData);
    thread->start();
}


