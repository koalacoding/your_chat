#include <QMessageBox>

#include "server.h"


Server::Server(QWidget *parent) {
  message_handler_ = new MessageHandler();
  statusLabel = new QLabel();

  Start();
}

/*--------------------------------
----------SESSION OPENED----------
--------------------------------*/

void Server::Start()
{
  tcpServer = new QTcpServer(this);
  QString ipAddress;
  QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

  if (!tcpServer->listen()) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Unable to start the server: %1.")
                          .arg(tcpServer->errorString()));
    close();
    return;
  }

  // Use the first non-localhost IPv4 address
  for (int i = 0; i < ipAddressesList.size(); ++i) {
    if (ipAddressesList.at(i) != QHostAddress::LocalHost
        && ipAddressesList.at(i).toIPv4Address()) {
      ipAddress = ipAddressesList.at(i).toString();
      break;
    }
  }

  // If we did not find one, use IPv4 localhost
  if (ipAddress.isEmpty()) ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

  statusLabel->setText(tr("Peers can connect to you with these informations :"
                          "\n\n"
                          "IP: %1"
                          "\n"
                          "Port: %2")
                       .arg(ipAddress).arg(tcpServer->serverPort()));

  connect(tcpServer, SIGNAL(newConnection()), this, SLOT(InitializeSocket()));
}

/*-----------------------------------
----------INITIALIZE SOCKET----------
-----------------------------------*/

void Server::InitializeSocket() {
  message_handler_->SetSocket(tcpServer->nextPendingConnection());
}

/*----------------------------
----------DESTRUCTOR----------
----------------------------*/

Server::~Server() {
    delete message_handler_;
}
