#include <QtWidgets>
#include <iostream>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QDialog(parent) {
    textEdit = new QTextEdit();
    lineEdit = new QLineEdit();
    connectButton = new QPushButton(tr("Connect"));
    Client* client = new Client;

    textEdit->setReadOnly(true);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(connectButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(textEdit);
    mainLayout->addWidget(lineEdit);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Your chat"));

    connect(connectButton, SIGNAL(clicked()),
            client, SLOT(show()));
}
