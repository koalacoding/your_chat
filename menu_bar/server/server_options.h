#ifndef SERVEROPTIONS_H
#define SERVEROPTIONS_H

#include <QDialog>

class ServerOptions : public QDialog {
  Q_OBJECT

  public:
    ServerOptions();
  private slots:
    void Show();
};

#endif // SERVEROPTIONS_H
