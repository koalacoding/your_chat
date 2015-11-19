#include <QApplication>
#include <QtNetwork>

#include "mainwindow/mainwindow.h"
#include "server/server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Server server;
    server.show();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    return a.exec();
}
