#ifndef CLIENT_H
#define CLIENT_H

#include <QtWidgets>
#include <QMessageBox>
#include <QtNetwork>
#include <message_handler/message_handler.h>

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QWidget {
  Q_OBJECT

  public:
    Client(QWidget *parent = 0);

    MessageHandler* message_handler_;
    QLabel *hostLabel;
    QLabel *portLabel;
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;

  private slots:
    void sessionOpened();

    /*-------------------------
    ----------CONNECT----------
    -------------------------*/

    void ConnectToServer(QWidget*);

    /*----------------------------
    ----------DISCONNECT----------
    ----------------------------*/

    void DisconnectFromServer(QWidget*);

  private:
    QNetworkSession *networkSession;
};

#endif
