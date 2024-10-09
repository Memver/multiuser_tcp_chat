#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QFile>

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server(QObject* parent = nullptr);
    ~Server();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void onReadyRead();
    void onClientDisconnected();

private:
    QMap<QTcpSocket*, QString> clients;
    QFile logFile;

    void broadcast(const QString& message);
    void sendClientList();
    void logMessage(const QString& sender, const QString& message);
    void sendToClient(const QString &recipient, const QString &message);
};

#endif // SERVER_H
