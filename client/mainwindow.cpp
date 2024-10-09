#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

MainWindow::MainWindow(Client* client, QWidget *parent)
    : QMainWindow(parent)
    ,  ui (new Ui::MainWindow)
{
    ui->setupUi(this);
    this->client = client;
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addLog(QString str)
{
    ui->textBrowser->append(str);
}


void MainWindow::on_pushButton_send_clicked()
{
    QString message = ui->textEdit->toPlainText(); // Получаем текст сообщения из поля ввода
    QString name = ui->lineEdit_name->text();

    if (name.isEmpty()) {
        // Если поле получателя пустое, отправляем всем
        client->sendMessage(message);
    } else {
        // Иначе, отправляем конкретному пользователю
        client->sendPrivateMessage(name, message);
    }

    ui->textEdit->clear();
}

void MainWindow::on_pushButton_connect_clicked()
{
    client->connectToServer();
}
