#include "server.h"

/*-----------------------------
----------CONSTRUCTOR----------
-----------------------------*/

Server::Server(QWidget *parent) : tcpServer(0), networkSession(0), is_connected_to_client_(false) {
  message_handler_ = new MessageHandler();

  statusLabel = new QLabel();

  QNetworkConfigurationManager manager;
  if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
    // Get saved network configuration
    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
    settings.endGroup();

    // If the saved network configuration is not currently discovered use the system default
    QNetworkConfiguration config = manager.configurationFromIdentifier(id);
    if ((config.state() & QNetworkConfiguration::Discovered) !=
        QNetworkConfiguration::Discovered) {
      config = manager.defaultConfiguration();
    }

    networkSession = new QNetworkSession(config, this);
    connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

    statusLabel->setText(tr("Opening network session."));
    networkSession->open();
  } else {
    sessionOpened();
  }

  connect(tcpServer, SIGNAL(newConnection()), this, SLOT(InitializeSocket()));
}

/*--------------------------------
----------SESSION OPENED----------
--------------------------------*/

void Server::sessionOpened()
{
  tcpServer = new QTcpServer(this);
  QString ipAddress;
  QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

  // Save the used configuration
  if (networkSession) {
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));

    if (config.type() == QNetworkConfiguration::UserChoice) {
      id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    }

    else id = config.identifier();

    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
  }

  if (!tcpServer->listen()) {
    QMessageBox::critical(this, tr("Fortune Server"),
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
}

/*-----------------------------------
----------INITIALIZE SOCKET----------
-----------------------------------*/

void Server::InitializeSocket() {
  message_handler_->socket_ = tcpServer->nextPendingConnection();
  connect(message_handler_->socket_, SIGNAL(readyRead()), message_handler_, SLOT(ReadMessage()));
  is_connected_to_client_ = true;
}

/*----------------------------------------
----------IS CONNECTED TO CLIENT----------
----------------------------------------*/

bool Server::IsConnectedToClient() {
  return is_connected_to_client_;
}

/*----------------------------
----------DESTRUCTOR----------
----------------------------*/

Server::~Server() {
    delete message_handler_;
}
