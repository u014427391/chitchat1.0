#-------------------------------------------------
#
# Project created by QtCreator 2014-11-30T13:55:49
#
#-------------------------------------------------

QT       += core gui

QT       += network

QT       += webkit

QT       += sql

QT       += phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chitchat
TEMPLATE = app

RC_FILE += app.rc

SOURCES += main.cpp\
        chitchat.cpp \
    tcpserver.cpp \
    tcpclient.cpp \
    drawwidget.cpp \
    doodleboard.cpp \
    login.cpp \
    privatechat.cpp \
    player.cpp \
    note.cpp \
    grapscreen.cpp
    

HEADERS  += chitchat.h \
    tcpserver.h \
    tcpclient.h \
    drawwidget.h \
    doodleboard.h \
    login.h \
    privatechat.h \
    player.h \
    note.h \
    grapscreen.h
  

FORMS    += chitchat.ui \
    tcpserver.ui \
    tcpclient.ui \
    login.ui \
    privatechat.ui \
    player.ui \
    grapscreen.ui
   

RESOURCES += \
    images.qrc \
    playericon.qrc \
    noteicon.qrc \
    exploreicon.qrc \
    usericon.qrc
