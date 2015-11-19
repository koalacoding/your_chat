#include <QtWidgets>
#include <iostream>

#include "mainwindow.h"

/*-----------------------------
----------CONSTRUCTOR----------
-----------------------------*/

MainWindow::MainWindow(QWidget *parent) : QDialog(parent) {
    connectButton = new QPushButton(tr("Connect"));
    Client* client = new Client;

    generateChatGrid();
    generateConnectGrid(client);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    /*QGridLayout *firstLayout = new QGridLayout;

    QGridLayout *tempLayout = new QGridLayout;
    tempLayout->addWidget(connectButton, 3, 1, 1, 1);
    gridGroupBox->setLayout(tempLayout);*/

    mainLayout->addWidget(chatGridGroupBox);
    mainLayout->addWidget(connectGridGroupBox);
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
