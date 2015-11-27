#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
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
  QGroupBox *chatGridGroupBox;
  QTextEdit *messages_text_edit;
  QLineEdit *message_line_edit;
  QPushButton *send_message_button_;
  QGroupBox *connectGridGroupBox;
  QPushButton *connect_button;
  QPushButton *disconnect_button;
  QGroupBox *yourInformationsGridGroupBox;
  QString GetSendMessageLineEditText();

  void generateChatGrid();
  void generateConnectGrid(Client* client);
  void generateYourInformationsGrid(Server* server);

private slots:
  void DisableConnectButton();
  void EnableConnectButton();
  void AddReceivedMessageToMessagesTextEdit();
  void AddSentMessageToMessagesTextEdit(QString);
  void StartMessageSending();
};

#endif // MAINWINDOW_H
