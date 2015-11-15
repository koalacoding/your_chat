#-------------------------------------------------
#
# Project created by QtCreator 2015-11-14T14:32:45
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = your_chat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    client.h

FORMS    += mainwindow.ui
