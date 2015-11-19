#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include "client/client.h"

class QTextEdit;
class QLineEdit;
class QPushButton;
class Client;

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    QTextEdit *textEdit;
    QLineEdit *lineEdit;
    QPushButton *connectButton;
};

#endif // MAINWINDOW_H
