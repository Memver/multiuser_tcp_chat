#include "server.h"
#include <QDebug>

Server::Server(QObject* parent) : QTcpServer(parent) {
    logFile.setFileName("chat_log.txt");
    if (!logFile.open(QIODevice::Append | QIODevice::Text)) {
        qCritical() << "Unable to open log file!";
    }

    if (!this->listen(QHostAddress::Any, 2323)) {
        qCritical() << "Server could not start!";
    } else {
        qDebug() << "Server started! Listening on port 2323.";
    }
}

Server::~Server() {
    logFile.close();
}

void Server::incomingConnection(qintptr socketDescriptor) {
    auto* socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onClientDisconnected);

    // Вставляем нового клиента и сообщаем о его подключении
    clients.insert(socket, "");
    qDebug() << "New client connected";
}


void Server::onReadyRead() {
    auto* socket = qobject_cast<QTcpSocket*>(sender());

    // Получаем все данные от клиента
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data).trimmed();

    if (clients[socket].isEmpty()) {
        // Первое сообщение — это имя клиента
        clients[socket] = message;
        broadcast(QString("%1 joined the chat").arg(clients[socket]));
        sendClientList();
    } else {
        // Проверяем, является ли сообщение адресным
        if (message.startsWith("@")) {
            // Извлекаем имя получателя и само сообщение
            int spaceIndex = message.indexOf(' ');
            if (spaceIndex != -1) {
                QString recipient = message.mid(1, spaceIndex - 1);
                QString msg = message.mid(spaceIndex + 1);
                sendToClient(recipient, QString("%1 (private): %2").arg(clients[socket], msg));
            }
        } else {
            // Протоколируем сообщение и отправляем его всем
            logMessage(clients[socket], message);
            broadcast(QString("%1: %2").arg(clients[socket], message));
        }
    }
}

void Server::onClientDisconnected() {
    auto* socket = qobject_cast<QTcpSocket*>(sender());
    broadcast(QString("%1 left the chat").arg(clients[socket]));
    clients.remove(socket);
    socket->deleteLater();
}

void Server::broadcast(const QString& message) {
    QByteArray data = message.toUtf8();  // Преобразуем сообщение в QByteArray

    for (auto* client : clients.keys()) {
        if (client->isOpen()) {
            client->write(data);  // Отправляем данные целиком
            client->flush();
        }
    }
}

void Server::sendClientList() {
    QStringList clientNames = clients.values();
    QString clientList = "Clients: " + clientNames.join(", ");

    // Преобразуем список клиентов в QByteArray
    QByteArray data = clientList.toUtf8();

    // Отправляем список всем клиентам
    for (auto* client : clients.keys()) {
        if (client->isOpen()) {
            client->write(data);
            client->flush();
        }
    }
}


void Server::sendToClient(const QString& recipient, const QString& message) {
    for (auto* client : clients.keys()) {
        if (clients[client] == recipient && client->isOpen()) {
            client->write(message.toUtf8());
            client->flush();
            break;  // Найден и отправлен, выходим из цикла
        }
    }
}

void Server::logMessage(const QString& sender, const QString& message) {
    QTextStream out(&logFile);
    out << sender << ": " << message << "\n";
    logFile.flush();
}
