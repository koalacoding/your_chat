#ifndef SERVER_H
#define SERVER_H

#include <QDialog>

class QTcpSocket;
class QLabel;
class QTcpServer;
class QNetworkSession;

class Server : public QWidget {
    Q_OBJECT

    public:
        Server(QWidget *parent = 0);

        QLabel *statusLabel;

        void SendMessage(QString);
    private slots:
        /*--------------------------------
        ----------SESSION OPENED----------
        --------------------------------*/

        void sessionOpened();

        /*-----------------------------------
        ----------INITIALIZE SOCKET----------
        -----------------------------------*/

        void InitializeSocket();

        void sendFortune();

    private:
        QTcpSocket *socket;
        QTcpServer *tcpServer;
        QStringList fortunes;
        QNetworkSession *networkSession;
};

#endif
