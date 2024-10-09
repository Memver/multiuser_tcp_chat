#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class MainWindow;

class Client : public QObject {
    Q_OBJECT

public:
    MainWindow* w;

    Client(QObject* parent = nullptr);

    void connectToServer();
    void setName(const QString& name);
    void sendMessage(const QString& message);
    void sendPrivateMessage(const QString &recipient, const QString &message);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket* socket;
    QString name;
};

#endif // CLIENT_H
