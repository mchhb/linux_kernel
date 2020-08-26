#-------------------------------------------------
#
# Project created by QtCreator 2019-06-21T11:38:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QFileView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scandir.cpp

HEADERS  += mainwindow.h \
    config.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
