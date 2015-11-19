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
           mainwindow.cpp \
           client.cpp \
           server.cpp

HEADERS  += mainwindow.h \
            client.h \
            server.h

FORMS    += mainwindow.ui
