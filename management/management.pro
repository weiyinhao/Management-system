#-------------------------------------------------
#
# Project created by QtCreator 2018-10-21T11:34:34
#
#-------------------------------------------------

QT       += core gui
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = management
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp\
        mainwindow.cpp \
    dlgitem.cpp

HEADERS  += mainwindow.h \
    dlgitem.h

FORMS    += mainwindow.ui \
    dlgitem.ui
