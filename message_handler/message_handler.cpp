#include "message_handler.h"

MessageHandler::MessageHandler(QWidget *parent) {
  socket_ = new QTcpSocket(this);
  block_size_ = 0;

  connect(socket_, SIGNAL(error(QAbstractSocket::SocketError)), this,
              SLOT(displayError(QAbstractSocket::SocketError)));
}

/*-------------------------------
----------DISPLAY ERROR----------
-------------------------------*/

void MessageHandler::displayError(QAbstractSocket::SocketError socketError) {
  switch (socketError) {
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
                                  "Make sure the server is running, "
                                  "and check that the host name and port "
                                  "settings are correct."));
      break;
    default:
      QMessageBox::information(this, tr("Error"),
                               tr("The following error occurred: %1.")
                               .arg(socket_->errorString()));
  }
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
        if (socket_->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> block_size_;
    }

    if (socket_->bytesAvailable() < block_size_)
        return;

    QString message;
    in >> message;

    block_size_ = 0;

    SetLastMessageReceived(message);
    EmitMessageReceivedSignal();
  }

  /*------------------------------
  ----------SEND MESSAGE----------
  ------------------------------*/

  void MessageHandler::SendMessage(QString message) {
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    socket_->write(block);
  }
