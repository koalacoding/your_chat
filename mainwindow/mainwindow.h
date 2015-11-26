#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

#include "server/server.h"
#include "client/client.h"

class QGroupBox;
class QTextEdit;
class QLineEdit;
class QPushButton;

class Server;
class Client;

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Client* client;
    Server* server;

    /*-----------------------------------------------
    -------------------------------------------------
    -----------------GENERATE GRIDS------------------
    -------------------------------------------------
    -----------------------------------------------*/

        /*---------------------------
        ----------CHAT GRID----------
        ---------------------------*/

        void generateChatGrid();

        QGroupBox *chatGridGroupBox;
        QTextEdit *messages_text_edit;
        QLineEdit *message_line_edit;
        QPushButton *send_message_button_;

        /*------------------------------
        ----------CONNECT GRID----------
        ------------------------------*/

        void generateConnectGrid(Client* client);

        QGroupBox *connectGridGroupBox;
        QPushButton *connect_button;
        QPushButton *disconnect_button;

        /*----------------------------------------
        ----------YOUR INFORMATIONS GRID----------
        ----------------------------------------*/

        void generateYourInformationsGrid(Server* server);

        QGroupBox *yourInformationsGridGroupBox;


    /*-------------------------------------------------
    ----------GET SEND MESSAGE LINE EDIT TEXT----------
    -------------------------------------------------*/

    QString GetSendMessageLineEditText();

private slots:
    /*------------------------------------------------------------
    ----------ADD RECEIVED MESSAGE TO MESSAGES TEXT EDIT----------
    ------------------------------------------------------------*/

    void AddReceivedMessageToMessagesTextEdit();

    /*--------------------------------------------------------
    ----------ADD SENT MESSAGE TO MESSAGES TEXT EDIT----------
    --------------------------------------------------------*/

    void AddSentMessageToMessagesTextEdit(QString);

    /*---------------------------------------
    ----------START MESSAGE SENDING----------
    ---------------------------------------*/

    void StartMessageSending();
};

#endif // MAINWINDOW_H
