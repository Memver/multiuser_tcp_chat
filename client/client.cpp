#include "mainwindow.h"
#include "client.h"

#include <QDebug>

Client::Client(QObject* parent) : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Client::onDisconnected);
}

void Client::connectToServer() {
    socket->connectToHost("127.0.0.1", 2323);
}

void Client::setName(const QString& name) {
    this->name = name;
    sendMessage(name);
}

void Client::sendMessage(const QString& message) {
    QByteArray data = message.toUtf8();  // Преобразуем сообщение в QByteArray
    socket->write(data);                 // Отправляем сообщение целиком
    socket->flush();
}

void Client::sendPrivateMessage(const QString& recipient, const QString& message) {
    QString privateMessage = QString("@%1 %2").arg(recipient, message);
    w->addLog(privateMessage);

    sendMessage(privateMessage);
}

void Client::onReadyRead() {
    QByteArray data = socket->readAll();  // Получаем все данные
    QString message = QString::fromUtf8(data);  // Преобразуем в строку

    w->addLog(message);  // Добавляем полученное сообщение в лог
}

void Client::onDisconnected() {
    qDebug() << "Disconnected from server";
}
