#include <QApplication>
#include <QtNetwork>

#include "main_window/main_window.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow main_window;
  main_window.show();

  qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

  return app.exec();
}
