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

private:
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
        QLineEdit *send_message_line_edit;
        QPushButton *sendMessageButton;

        /*------------------------------
        ----------CONNECT GRID----------
        ------------------------------*/

        void generateConnectGrid(Client* client);

        QGroupBox *connectGridGroupBox;

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
    void AddSentMessageToMessagesTextEdit();
};

#endif // MAINWINDOW_H
