#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QWidget>
#include <QMessageBox>
#include <QtNetwork>

class QTcpSocket;

class MessageHandler : public QWidget {
    Q_OBJECT

  public:
    MessageHandler(QWidget *parent = 0);

    QString GetLastMessageReceived();
    void SetLastMessageReceived(QString);
    void SendMessageToPeer(QString);

    QTcpSocket* socket_;
    quint16 block_size_;

    QString last_message_received_;

  signals:
    void MessageReceived();

  private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void EmitMessageReceivedSignal();
    void ReadMessage();
};

#endif // MESSAGEHANDLER_H
