#include <QtWidgets>
#include <iostream>

#include "mainwindow.h"

/*-----------------------------
----------CONSTRUCTOR----------
-----------------------------*/

MainWindow::MainWindow(QWidget *parent) : QDialog(parent) {
    client = new Client;
    server = new Server;

    setWindowTitle(tr("Your Chat"));

    generateChatGrid();
    generateConnectGrid(client);
    generateYourInformationsGrid(server);

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(chatGridGroupBox);
    mainLayout->addWidget(connectGridGroupBox);
    mainLayout->addWidget(yourInformationsGridGroupBox);
    setLayout(mainLayout);

    connect(client, SIGNAL(MessageReceived()), this, SLOT(AddReceivedMessageToMessagesTextEdit()));
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
        QSignalMapper* signalMapper = new QSignalMapper(this);
        QString message;

        messages_text_edit = new QTextEdit();
        message_line_edit = new QLineEdit();
        send_message_button_ = new QPushButton(tr("Send"));

        messages_text_edit->setReadOnly(true);

        layout->addWidget(messages_text_edit, 0, 0, 1, 3);
        layout->addWidget(message_line_edit, 1, 0, 1, 3);
        layout->addWidget(send_message_button_, 2, 1, 1, 1);

        connect(send_message_button_, SIGNAL(clicked()), this, SLOT(StartMessageSending()));

        chatGridGroupBox->setLayout(layout);
    }

    /*---------------------------------------
    ----------GENERATE CONNECT GRID----------
    ---------------------------------------*/

    void MainWindow::generateConnectGrid(Client* client) {
        connectGridGroupBox = new QGroupBox(tr("Connect to a peer"));
        connect_button = new QPushButton(tr("Connect"));
        disconnect_button = new QPushButton(tr("Disconnect"));
        QGridLayout *layout = new QGridLayout;
        QSignalMapper* signalMapper = new QSignalMapper(this);
        QSignalMapper* signalMapper2 = new QSignalMapper(this);

        layout->addWidget(client->hostLabel, 0, 0, 1, 1);
        layout->addWidget(client->hostCombo, 0, 1, 1, 4);
        layout->addWidget(client->portLabel, 1, 0, 1, 1);
        layout->addWidget(client->portLineEdit, 1, 1, 1, 4);
        layout->addWidget(client->statusLabel, 2, 1, 1, 3);
        layout->addWidget(connect_button, 3, 2, 1, 1);
        layout->addWidget(disconnect_button, 4, 2, 1, 1);

        connectGridGroupBox->setLayout(layout);

        connect(connect_button, SIGNAL(clicked()), signalMapper, SLOT(map()));
        connect(disconnect_button, SIGNAL(clicked()), signalMapper2, SLOT(map()));

        signalMapper->setMapping(connect_button, connect_button);
        signalMapper2->setMapping(disconnect_button, connect_button);

        connect(signalMapper, SIGNAL(mapped(QWidget*)), client, SLOT(ConnectToServer(QWidget*)));
        connect(signalMapper2, SIGNAL(mapped(QWidget*)), client, SLOT(DisconnectFromServer(QWidget*)));
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


/*----------------------------------------
------------------------------------------
-----------------MESSAGE------------------
------------------------------------------
----------------------------------------*/


    /*------------------------------------------------------------
    ----------ADD RECEIVED MESSAGE TO MESSAGES TEXT EDIT----------
    ------------------------------------------------------------*/

    void MainWindow::AddReceivedMessageToMessagesTextEdit() {
        QString message = client->GetLastMessageReceived();
        std::cout << message.toStdString() << std::endl;
    }

    /*---------------------------------------------
    -----------------------------------------------
    -----------------SEND MESSAGE------------------
    -----------------------------------------------
    ---------------------------------------------*/


        /*-------------------------------------------------
        ----------GET SEND MESSAGE LINE EDIT TEXT----------
        -------------------------------------------------*/

        QString MainWindow::GetSendMessageLineEditText() {
            return message_line_edit->text();
        }

        /*--------------------------------------------------------
        ----------ADD SENT MESSAGE TO MESSAGES TEXT EDIT----------
        --------------------------------------------------------*/

        void MainWindow::AddSentMessageToMessagesTextEdit(QString message) {
            /* If the message is empty, we don't send it to the peer
               and don't add it to the messages text edit. */
            if (message == tr("")) {
                return;
            }

            QString formatted_message = tr("You > ") + message;

            messages_text_edit->append(formatted_message);

            message_line_edit->setText(tr(""));
        }

        /*---------------------------------------
        ----------START MESSAGE SENDING----------
        ---------------------------------------*/

        void MainWindow::StartMessageSending() {
            QString message = GetSendMessageLineEditText();

            server->SendMessage(message);

            AddSentMessageToMessagesTextEdit(message);
        }


MainWindow::~MainWindow() {
    delete client;
    delete server;
}
