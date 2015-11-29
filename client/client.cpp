#include "client.h"

/*-----------------------------
----------CONSTRUCTOR----------
-----------------------------*/

Client::Client(QWidget *parent) {
    message_handler_ = new MessageHandler();

    hostLabel = new QLabel(tr("P&eer IP:"));
    portLabel = new QLabel(tr("&Port:"));

    hostCombo = new QComboBox;
    hostCombo->setEditable(true);

    QString name = QHostInfo::localHostName();

    if (!name.isEmpty()) {
      QString domain = QHostInfo::localDomainName();

      hostCombo->addItem(name);

      if (!domain.isEmpty()) hostCombo->addItem(name + QChar('.') + domain);
    }

    if (name != QString("localhost")) hostCombo->addItem(QString("localhost"));

    // Find out IP addresses of this machine.
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // Add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
      if (!ipAddressesList.at(i).isLoopback()) {
        hostCombo->addItem(ipAddressesList.at(i).toString());
      }
    }

    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback()) {
            hostCombo->addItem(ipAddressesList.at(i).toString());
        }
    }

    portLineEdit = new QLineEdit;
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    hostLabel->setBuddy(hostCombo);
    portLabel->setBuddy(portLineEdit);

    portLineEdit->setFocus();
}

/*---------------------------------
----------CONNECT TO HOST----------
---------------------------------*/

void Client::ConnectToHost()
{
  QString host_address = hostCombo->currentText();
  quint16 port = portLineEdit->text().toInt();

  message_handler_->ConnectSocketToHost(host_address, port);
}

/*-------------------------------------
----------HANDLE DISCONNECTED----------
-------------------------------------*/

void Client::HandleDisconnected() {
  DisconnectedFromServer(); // Emits this signal
}

/*----------------------------
----------DISCONNECT----------
----------------------------*/

void Client::DisconnectFromServer() {
    message_handler_->socket_->abort();
}

/*----------------------------
----------DESTRUCTOR----------
----------------------------*/

Client::~Client() {
    delete message_handler_;
}
