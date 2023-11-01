//
// ConnectionHandler.h
// 2023-08-13
// Ivan Konishchev
//

#pragma once

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include <QJsonDocument>
#include <QJsonObject>

// Класс для обработки подключений
class ConnectionHandler : public QObject {
    Q_OBJECT
public:
    explicit ConnectionHandler(QTcpSocket* socket, QObject* parent = nullptr) : QObject(parent), _socket(socket) {
        connect(socket, &QTcpSocket::readyRead, this, &ConnectionHandler::readData);
        connect(socket, &QTcpSocket::disconnected, this, &ConnectionHandler::onDisconnected);

    }

signals:
    /**
     * Signal for sending a message about the status of the server
     *
     * @brief messageReceived
     * @param message
     */
    void messageReceived(const QString& message);

    /**
     * Processes the received data
     * @brief receiveData
     * @param socket
     * @param command
     * @param data
     */
    void receiveData(QTcpSocket *socket, int command,const QJsonObject &data);

    /**
     * Signal to close the socket (removing the socket from the list of connections)
     * on the server side
     *
     * @brief disconnectClient
     * @param socket
     */
    void disconnectClient(QTcpSocket *socket);

public slots:
    /**
    * @brief Getting data from clients
    *
    * This method receives data over the network from the client in QByteArray;
    *
    */
    void readData();

    /**
     * @brief Called when the client is disconnected from the server
     *
     * It is necessary to handle the closure of the socket and the thread
     *
     */
    void onDisconnected();

private:
    QTcpSocket* _socket;
};
