#include "main_window.h"
#include "menu_bar/server/server_options.h"

/*-----------------------------
----------CONSTRUCTOR----------
-----------------------------*/

MainWindow::MainWindow(QWidget *parent) : QDialog(parent) {
  client = new Client;
  server = new Server;
  QVBoxLayout *mainLayout = new QVBoxLayout;

  setWindowTitle(tr("Your Chat"));

  GenerateMenuBar();
  generateChatGrid();
  generateConnectGrid(client);
  generateYourInformationsGrid(server);

  mainLayout->setMenuBar(menu_bar_);
  mainLayout->addWidget(chatGridGroupBox);
  mainLayout->addWidget(connectGridGroupBox);
  mainLayout->addWidget(yourInformationsGridGroupBox);
  setLayout(mainLayout);

  connect(client->message_handler_, SIGNAL(MessageReceived()), this, SLOT(AddReceivedMessageToMessagesTextEdit()));
  connect(server->message_handler_, SIGNAL(MessageReceived()), this, SLOT(AddReceivedMessageToMessagesTextEdit()));
}


/*--------------------------------------------------------------
----------------------------------------------------------------
-----------------GENERATE MAIN WINDOW ELEMENTS------------------
----------------------------------------------------------------
--------------------------------------------------------------*/


  /*-----------------------------------
  ----------GENERATE MENU BAR----------
  -----------------------------------*/

  void MainWindow::GenerateMenuBar() {
    menu_bar_ = new QMenuBar;
    ServerOptions* t = new ServerOptions;
    server_menu_ = menu_bar_->addMenu(tr("Server"));
    server_menu_options_ = new QAction(tr("&Options"), server_menu_);
    server_menu_->addAction(server_menu_options_);
    connect(server_menu_options_, SIGNAL(triggered()), t, SLOT(Show()));
  }

  /*------------------------------------
  ----------GENERATE CHAT GRID----------
  ------------------------------------*/

  void MainWindow::generateChatGrid() {
    chatGridGroupBox = new QGroupBox(tr("Chat"));
    QGridLayout *layout = new QGridLayout;
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

    layout->addWidget(client->hostLabel, 0, 0, 1, 1);
    layout->addWidget(client->hostCombo, 0, 1, 1, 4);
    layout->addWidget(client->portLabel, 1, 0, 1, 1);
    layout->addWidget(client->portLineEdit, 1, 1, 1, 4);
    layout->addWidget(connect_button, 3, 2, 1, 1);
    layout->addWidget(disconnect_button, 4, 2, 1, 1);

    connectGridGroupBox->setLayout(layout);

    connect(connect_button, SIGNAL(clicked()), client, SLOT(ConnectToHost()));
    connect(client->message_handler_, SIGNAL(SocketConnected()), this,
            SLOT(DisableConnectButton()));
    connect(disconnect_button, SIGNAL(clicked()), client, SLOT(DisconnectFromServer()));
    connect(client->message_handler_, SIGNAL(SocketDisconnected()), this,
            SLOT(EnableConnectButton()));
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


/*----------------------------------------------------------------
------------------------------------------------------------------
-----------------ENABLE / DISABLE CONNECT BUTTON------------------
------------------------------------------------------------------
----------------------------------------------------------------*/


  /*----------------------------------------
  ----------DISABLE CONNECT BUTTON----------
  ----------------------------------------*/

  void MainWindow::DisableConnectButton() {
    connect_button->setEnabled(false);
  }

  /*---------------------------------------
  ----------ENABLE CONNECT BUTTON----------
  ---------------------------------------*/

  void MainWindow::EnableConnectButton() {
    connect_button->setEnabled(true);
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
      QString message;
      QString formatted_message;

      // If the server's socket is connected
      if (server->message_handler_->socket_->state() == QAbstractSocket::ConnectedState) {
        // Then we grab the message from the server
        message = server->message_handler_->GetLastMessageReceived();
      } else message = client->message_handler_->GetLastMessageReceived();

      formatted_message = tr("Peer > ") + message;
      messages_text_edit->append(formatted_message);
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
        QString formatted_message = tr("You > ") + message;

        messages_text_edit->append(formatted_message);

        message_line_edit->setText(tr(""));
    }

    /*---------------------------------------
    ----------START MESSAGE SENDING----------
    ---------------------------------------*/

    void MainWindow::StartMessageSending() {
      QString message = GetSendMessageLineEditText();
      bool message_sent = false;

      if (message == tr("")) return; // If the message is empty, we don't send it.

      // If the server's socket is connected
      if (server->message_handler_->socket_->state() == QAbstractSocket::ConnectedState) {
        // Then it is the server that sends the message, not the client
        server->message_handler_->SendMessageToPeer(message);
        message_sent = true;
      }
      // Otherwise, if the client's socket is connected
      else if (client->message_handler_->socket_->state() == QAbstractSocket::ConnectedState) {
        // Then it is the client that sends the message
        client->message_handler_->SendMessageToPeer(message);
        message_sent = true;
      }

      if (message_sent) AddSentMessageToMessagesTextEdit(message);
    }


MainWindow::~MainWindow() {
    delete client;
    delete server;
}
