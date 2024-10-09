#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class Client;

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE


class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Client*, QWidget *parent = nullptr);
    ~MainWindow();

    void addLog(QString str);

private slots:
    void on_pushButton_send_clicked();

    void on_pushButton_connect_clicked();

private:
    Ui::MainWindow* ui;
    Client* client;
};


#endif // MAINWINDOW_H
