//
// TCP_Server.h
// 2023-28-10
// Ivan Konishchev
//

#pragma once

#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QThread>
#include "ConnectionHandler.h"
#include "User.h"
#include "Message.h"
#include "mysqldatabase.h"
#include <QHostAddress>

// Класс для сервера
class TCPServer : public QTcpServer {
    Q_OBJECT

    enum CommandToServer : uint16_t
    {
        ADD_USER = 2,
        ADD_MESSAGE = 4,
        IS_UNIQUE_LOGIN = 8,
        GET_ALL_USERS = 16,
        GET_USER_BY_LOGIN = 32,
        GET_USER_BY_ID = 64,
        USERS_COUNT = 128,
        GET_ALL_PRIVATE_MESSAGE = 256,
        UPDATE_USER_DATA = 512,
        DELETE_MESSAGE_BY_ID = 1024,
        GET_ALL_PUBLIC_MESSAGES = 2048,
        AUTHENTIFICATE = 4096,
    };


signals:
    void updateUI(const QString &message);

private:
    int _port = 45000;
    QHostAddress _host = QHostAddress("localhost");
    QList<QTcpSocket*> _clientConnections;

    /**
     * @brief closeAllConnections
     * Disables all connected clients
     */
    void closeAllConnections();
public:
    /**
     * @brief TCPServer
     * @param parent
     * init
     */
    explicit TCPServer(QObject* parent = nullptr) : QTcpServer(parent) {}

    /**
     * @brief ~TCPServer
     * deinit
     */
    ~TCPServer();

    /**
     * @brief isConnected
     * @return
     * Checks whether the server is connected to the port
     */
    bool isConnected();

public slots:
    /**
     * @brief handleUpdateUI
     * @param message
     * outputs message on a signal from ConnectionHandler -> messageReceived
     */
    void handleUpdateUI(const QString& message);

    /**
     * @brief disconnectClient
     * @param socket
     * Кemoves the client's socket from the list of connected by signal from
     * ConnectionHandler -> disconnectClient.
     * Called when the client is disconnected from the server
     */
    void disconnectClient(QTcpSocket *socket);

    /**
     * @brief processData
     * @param socked -> needed to respond to the client
     * @param command -> command to process
     * @param data -> data in jsonobject
     * Processing of received data from the client
     */
    void processData(QTcpSocket *socket, int command, const QJsonObject &data);

protected:
    /**
     * @brief incomingConnection
     * @param socketDescriptor
     * Waiting for new clients to connect.
     * The new connection is passed in a new thread to the ConnectionHandler
     */
    void incomingConnection(qintptr socketDescriptor) override;
};
