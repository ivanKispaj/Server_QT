//
// ConnectionHandler.cpp
// 2023-08-13
// Ivan Konishchev
//


#include "ConnectionHandler.h"
#include <QJsonObject>
#include <QJsonDocument>

void ConnectionHandler::readData()
{

    QByteArray data = _socket->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    if (!jsonDoc.isNull())
    {
        // Преобразование QJsonDocument в QJsonObject
        QJsonObject jsonObj = jsonDoc.object();
        // Доступ к данным в формате JSON
        int command = jsonObj["command"].toString().toInt();
        emit receiveData(_socket, command, jsonObj);
    } else {
        emit messageReceived(tr("Failed to parse JSON"));
    }
}

void ConnectionHandler::onDisconnected()
{
    emit disconnectClient(_socket);
    _socket->disconnectFromHost();
    thread()->quit();
    _socket->deleteLater();
    deleteLater();

}
