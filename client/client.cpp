#include <iostream>
#include <QtWidgets>
#include <QtNetwork>

#include "client.h"

Client::Client(QWidget *parent)
:   QDialog(parent), networkSession(0)
{
    hostLabel = new QLabel(tr("P&eer IP:"));
    portLabel = new QLabel(tr("&Port:"));

    hostCombo = new QComboBox;
    hostCombo->setEditable(true);

    QString name = QHostInfo::localHostName();

    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QString("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit = new QLineEdit;
    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    hostLabel->setBuddy(hostCombo);
    portLabel->setBuddy(portLineEdit);

    statusLabel = new QLabel(tr("This examples requires that you run the "
                                "Fortune Server example as well."));

    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(displayError(QAbstractSocket::SocketError)));

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(ReadMessage()));

    portLineEdit->setFocus();

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

        std::cout << "Opening network session." << std::endl;
        networkSession->open();
    }
}

/*-----------------------------------
----------CONNECT TO SERVER----------
-----------------------------------*/

void Client::ConnectToServer(QWidget* connect_button)
{
    connect_button->setEnabled(false);

    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(),
                             portLineEdit->text().toInt());
}

/*----------------------------------------
------------------------------------------
-----------------MESSAGE------------------
------------------------------------------
----------------------------------------*/


    /*----------------------------------------------
    ----------EMIT MESSAGE RECEIVED SIGNAL----------
    ----------------------------------------------*/

    void Client::EmitMessageReceivedSignal() {
        emit MessageReceived();
    }

    /*------------------------------
    ----------READ MESSAGE----------
    ------------------------------*/

    void Client::ReadMessage()
    {
        QDataStream in(tcpSocket);
        in.setVersion(QDataStream::Qt_4_0);

        if (blockSize == 0) {
            if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
                return;

            in >> blockSize;
        }

        if (tcpSocket->bytesAvailable() < blockSize)
            return;

        QString message;
        in >> message;

        SetLastMessageReceived(message);
        EmitMessageReceivedSignal();
    }

    /*-------------------------------------------
    ----------SET LAST MESSAGE RECEIVED----------
    -------------------------------------------*/

    void Client::SetLastMessageReceived(QString message) {
        last_message_received_ = message;
    }

    /*-------------------------------------------
    ----------GET LAST MESSAGE RECEIVED----------
    -------------------------------------------*/

    QString Client::GetLastMessageReceived() {
        return last_message_received_;
    }

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Fortune Client"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }
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

    statusLabel->setText(tr("This examples requires that you run the "
                            "Fortune Server example as well."));
}

/*----------------------------
----------DISCONNECT----------
----------------------------*/

void Client::DisconnectFromServer(QWidget* connect_button) {
    tcpSocket->abort();

    connect_button->setEnabled(true);
}
