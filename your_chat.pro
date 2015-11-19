#-------------------------------------------------
#
# Project created by QtCreator 2015-11-14T14:32:45
#
#-------------------------------------------------

QT += core gui network widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = your_chat
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow/mainwindow.cpp \
           client/client.cpp \
           server/server.cpp

HEADERS  += mainwindow/mainwindow.h \
            client/client.h \
            server/server.h
