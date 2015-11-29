#include <QMessageBox>

#include "server.h"


Server::Server(QWidget *parent) {
  message_handler_ = new MessageHandler();
  statusLabel = new QLabel();

  GetPublicIp();
}

/*--------------------------------
----------SESSION OPENED----------
--------------------------------*/

#include <iostream>

void Server::GetPublicIp()
{
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(StartServer(QNetworkReply*)));

  // Getting the program's user's IP address
  manager->get(QNetworkRequest(QUrl("http://whatismyip.org")));
}

void Server::StartServer(QNetworkReply *reply) {
  int x;
  std::string ip_address;

  tcpServer = new QTcpServer(this);

  reply->header(QNetworkRequest::ContentTypeHeader).toString();
  std::string html = QString(reply->readAll()).toStdString();
  /* The IP address in the html code of whatismyip.org is after 'font-weight: 600;">'
     (which is 19 characters long) and before the next "</span>" */
  x = html.find("font-weight: 600");
  html = html.substr(x + 19);
  x = html.find("</span>");
  html = html.substr(0, x);
  ip_address = html;

  if (!tcpServer->listen()) {
    QMessageBox::critical(this, tr("Error"),
                          tr("Unable to start the server: %1.")
                          .arg(tcpServer->errorString()));
    close();
    return;
  }

  statusLabel->setText(tr("Peers can connect to you with these informations :"
                          "\n\n"
                          "IP: %1"
                          "\n"
                          "Port: %2")
                       .arg(QString::fromStdString(ip_address)).arg(tcpServer->serverPort()));

  std::cout << QHostAddress(QHostAddress::LocalHost).toString().toStdString() << std::endl;

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
