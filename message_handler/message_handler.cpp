#include <QMessageBox>

#include "message_handler.h"


MessageHandler::MessageHandler(QWidget *parent) {
  socket_ = new QTcpSocket(this);
  block_size_ = 0;

  connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this,
          SLOT(DisplayError(QAbstractSocket::SocketError)));
}

/*-------------------------------
----------DISPLAY ERROR----------
-------------------------------*/

void MessageHandler::DisplayError(QAbstractSocket::SocketError socket_error) {
  switch (socket_error) {
    case QAbstractSocket::RemoteHostClosedError:
      break;

    case QAbstractSocket::HostNotFoundError:
      QMessageBox::information(this, tr("Error"),
                               tr("The host was not found. Please check the "
                                  "host name and port settings."));
      break;

    case QAbstractSocket::ConnectionRefusedError:
      QMessageBox::information(this, tr("Error"),
                               tr("The connection was refused by the peer. "
                                  "Make sure his server is running, "
                                  "and check that the host name and port "
                                  "settings are correct."));
      break;

    default:
      QMessageBox::information(this, tr("Error"),
                               tr("The following error occurred: %1.")
                               .arg(socket_->errorString()));
  }
}


/*------------------------------------------------------
--------------------------------------------------------
-----------------SOCKET INITIALIZATION------------------
--------------------------------------------------------
------------------------------------------------------*/


  /*----------------------------------------
  ----------CONNECT SOCKET TO HOST----------
  ----------------------------------------*/

  void MessageHandler::ConnectSocketToHost(QString host_address, quint16 port) {
    socket_->abort(); // In case there was already a connection
    socket_->connectToHost(host_address, port);

    connect(socket_, SIGNAL(connected()), this, SLOT(HandleConnected()));
    connect(socket_, SIGNAL(disconnected()), this, SLOT(HandleDisconnected()));
  }

  /*----------------------------------
  ----------HANDLE CONNECTED----------
  ----------------------------------*/

  void MessageHandler::HandleConnected() {
    connect(socket_, SIGNAL(readyRead()), this, SLOT(ReadMessage()));
    SocketConnected(); // Emits this signal
  }

  /*-------------------------------------
  ----------HANDLE DISCONNECTED----------
  -------------------------------------*/

  void MessageHandler::HandleDisconnected() {
    SocketDisconnected(); // Emits this signal
  }

  /*----------------------------
  ----------SET SOCKET----------
  ----------------------------*/

  void MessageHandler::SetSocket(QTcpSocket* socket) {
    socket_ = socket;
    connect(socket_, SIGNAL(readyRead()), this, SLOT(ReadMessage()));
  }


/*----------------------------------------
------------------------------------------
-----------------MESSAGE------------------
------------------------------------------
----------------------------------------*/


  /*----------------------------------------------
  ----------EMIT MESSAGE RECEIVED SIGNAL----------
  ----------------------------------------------*/

  void MessageHandler::EmitMessageReceivedSignal() {
      emit MessageReceived();
  }


  /*------------------------------------------------------
  --------------------------------------------------------
  -----------------LAST MESSAGE RECEIVED------------------
  --------------------------------------------------------
  ------------------------------------------------------*/


    /*-------------------------------------------
    ----------GET LAST MESSAGE RECEIVED----------
    -------------------------------------------*/

    QString MessageHandler::GetLastMessageReceived() {
        return last_message_received_;
    }

    /*-------------------------------------------
    ----------SET LAST MESSAGE RECEIVED----------
    -------------------------------------------*/

    void MessageHandler::SetLastMessageReceived(QString message) {
        last_message_received_ = message;
    }


  /*------------------------------
  ----------READ MESSAGE----------
  ------------------------------*/

  void MessageHandler::ReadMessage() {
    QDataStream in(socket_);
    in.setVersion(QDataStream::Qt_4_0);

    if (block_size_ == 0) {
      // If there is nothing interesting to read from the socket
      if (socket_->bytesAvailable() < (int)sizeof(quint16))
          return;

      in >> block_size_; // We store the size of the block we will read inside block_size_
    }

    // Until we can read block entirely, we don't read the block
    if (socket_->bytesAvailable() < block_size_) return;

    QString message;

    in >> message; // We read the block and store the data inside the the "message" variable.

    block_size_ = 0; // We reset block_size_ to be able to read a new block

    SetLastMessageReceived(message);

    EmitMessageReceivedSignal();
  }

  /*------------------------------
  ----------SEND MESSAGE----------
  ------------------------------*/

  void MessageHandler::SendMessageToPeer(QString message) {
    QByteArray block; // The data will be stored in this variable
    // This data stream will allow use to write data into the block
    QDataStream out(&block, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_4_0);
    /* We first reserve the size of a quint16 (16bits int) in the beginning of the block
       We will later write in this space the total size of the block we will write in the socket */
    out << (quint16)0;
    // We then write in the block the message we will send to the peer
    out << message;
    // We position the cursor at the beginning of the block
    out.device()->seek(0);
    /* And then, in the 16bits space we previously reserved, we write the total size of the block
       we are going to write in the socket */
    out << (quint16)(block.size() - sizeof(quint16));

    socket_->write(block);
  }
