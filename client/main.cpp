#include "mainwindow.h"
#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client client;
    MainWindow w = MainWindow(&client);
    client.w = &w;
    w.show();

    return a.exec();
}
