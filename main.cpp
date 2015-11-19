#include <QApplication>
#include <QtNetwork>

#include "mainwindow.h"
#include "server.h"
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Server server;
    server.show();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    Client client;
    client.show();

    return a.exec();
}
