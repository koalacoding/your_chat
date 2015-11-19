#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

class QTextEdit;
class QPushButton;

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private:
    QTextEdit *textEdit;
    QPushButton *quitButton;
};

#endif // MAINWINDOW_H
