#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "client/client.h"

class QGroupBox;
class QTextEdit;
class QLineEdit;
class QPushButton;

class Client;

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    /*---------------------------
    ----------CHAT GRID----------
    ---------------------------*/

    void generateChatGrid();

    QGroupBox *chatGridGroupBox;
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QPushButton *sendMessageButton;

    /*------------------------------
    ----------CONNECT GRID----------
    ------------------------------*/

    void generateConnectGrid(Client* client);

    QGroupBox *connectGridGroupBox;
    QPushButton *connectButton;
};

#endif // MAINWINDOW_H
