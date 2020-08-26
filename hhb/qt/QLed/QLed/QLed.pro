#-------------------------------------------------
#
# Project created by QtCreator 2019-06-28T09:49:10
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QLed
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    weater.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
