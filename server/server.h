#ifndef SERVER_H
#define SERVER_H

#include <QtWidgets>
#include <QDialog>
#include <QtNetwork>
#include <message_handler/message_handler.h>

class QLabel;
class QTcpServer;
class QNetworkSession;

class Server : public QWidget {
    Q_OBJECT

  public:
    Server(QWidget *parent = 0);
    MessageHandler* message_handler_;
    QLabel *statusLabel;

    ~Server();

  private slots:
    void sessionOpened();
    void InitializeSocket();

  private:
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;
};

#endif
