#include "client.h"

/*-----------------------------
----------CONSTRUCTOR----------
-----------------------------*/

Client::Client(QWidget *parent) : networkSession(0) {
    QNetworkConfigurationManager manager;

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

        networkSession->open();
    }
}

/*-----------------------------------
----------CONNECT TO SERVER----------
-----------------------------------*/

void Client::ConnectToServer(QWidget* connect_button)
{
    connect_button->setEnabled(false);

    message_handler_->socket_->abort(); // In case there was already a connection.
    message_handler_->socket_->connectToHost(hostCombo->currentText(),
                                             portLineEdit->text().toInt());

    connect(message_handler_->socket_, SIGNAL(readyRead()), message_handler_, SLOT(ReadMessage()));
}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
}

/*----------------------------
----------DISCONNECT----------
----------------------------*/

void Client::DisconnectFromServer(QWidget* connect_button) {
    message_handler_->socket_->abort();

    connect_button->setEnabled(true);
}
