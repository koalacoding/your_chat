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
           main_window/main_window.cpp \
           client/client.cpp \
           server/server.cpp \
           message_handler/message_handler.cpp \
    menu_bar/server/server_options.cpp


HEADERS  += main_window/main_window.h \
            client/client.h \
            server/server.h \
            message_handler/message_handler.h \
    menu_bar/server/server_options.h
