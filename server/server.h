#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QLabel>
#include <QtNetwork>

#include <message_handler/message_handler.h>


class QLabel;
class QNetworkSession;
class QTcpServer;

class Server : public QWidget {
  Q_OBJECT

  public:
    Server(QWidget *parent = 0);

    QLabel *statusLabel;
    MessageHandler* message_handler_;

    ~Server();

  private slots:
    void Start();
    void InitializeSocket();

  private:
    QTcpServer *tcpServer;
};

#endif
