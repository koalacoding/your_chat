#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QWidget>
#include <QtNetwork>

class QTcpSocket;

class MessageHandler : public QWidget {
  Q_OBJECT

  public:
    MessageHandler(QWidget *parent = 0);

    QTcpSocket* socket_;
    quint16 block_size_;
    QString last_message_received_;


    void ConnectSocketToHost(QString host_adress, quint16 port);
    void SetSocket(QTcpSocket*);

    QString GetLastMessageReceived();
    void SetLastMessageReceived(QString);
    void SendMessageToPeer(QString);

  signals:
    void SocketConnected();
    void MessageReceived();

  private slots:
    void HandleConnected();

    void DisplayError(QAbstractSocket::SocketError);
    void EmitMessageReceivedSignal();
    void ReadMessage();
};

#endif // MESSAGEHANDLER_H
