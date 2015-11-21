#include <QtWidgets>
#include <iostream>

#include "mainwindow.h"

/*-----------------------------
----------CONSTRUCTOR----------
-----------------------------*/

MainWindow::MainWindow(QWidget *parent) : QDialog(parent) {
    Client* client = new Client;
    Server* server = new Server;
    connectButton = new QPushButton(tr("Connect"));

    generateChatGrid();
    generateConnectGrid(client);
    generateYourInformationsGrid(server);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(chatGridGroupBox);
    mainLayout->addWidget(connectGridGroupBox);
    mainLayout->addWidget(yourInformationsGridGroupBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Your chat"));

    connect(connectButton, SIGNAL(clicked()),
            client, SLOT(requestNewFortune()));
}


/*-----------------------------------------------
-------------------------------------------------
-----------------GENERATE GRIDS------------------
-------------------------------------------------
-----------------------------------------------*/


    /*------------------------------------
    ----------GENERATE CHAT GRID----------
    ------------------------------------*/

    void MainWindow::generateChatGrid() {
        chatGridGroupBox = new QGroupBox(tr("Chat"));
        QGridLayout *layout = new QGridLayout;

        textEdit = new QTextEdit();
        lineEdit = new QLineEdit();
        sendMessageButton = new QPushButton(tr("Send"));

        textEdit->setReadOnly(true);

        layout->addWidget(textEdit, 0, 0, 1, 3);
        layout->addWidget(lineEdit, 1, 0, 1, 3);
        layout->addWidget(sendMessageButton, 2, 1, 1, 1);

        chatGridGroupBox->setLayout(layout);
    }

    /*---------------------------------------
    ----------GENERATE CONNECT GRID----------
    ---------------------------------------*/

    void MainWindow::generateConnectGrid(Client* client) {
        connectGridGroupBox = new QGroupBox(tr("Connect to a peer"));
        QGridLayout *layout = new QGridLayout;

        connectButton = new QPushButton(tr("Connect"));

        layout->addWidget(client->hostLabel, 0, 0, 1, 1);
        layout->addWidget(client->hostCombo, 0, 1, 1, 4);
        layout->addWidget(client->portLabel, 1, 0, 1, 1);
        layout->addWidget(client->portLineEdit, 1, 1, 1, 4);
        layout->addWidget(client->statusLabel, 2, 1, 1, 3);
        layout->addWidget(connectButton, 3, 2, 1, 1);

        connectGridGroupBox->setLayout(layout);
    }

    /*-------------------------------------------------
    ----------GENERATE YOUR INFORMATIONS GRID----------
    -------------------------------------------------*/

    void MainWindow::generateYourInformationsGrid(Server* server) {
        yourInformationsGridGroupBox = new QGroupBox(tr("Your informations"));
        QGridLayout *layout = new QGridLayout;

        layout->addWidget(server->statusLabel, 0, 0, 1, 1);

        yourInformationsGridGroupBox->setLayout(layout);
    }
