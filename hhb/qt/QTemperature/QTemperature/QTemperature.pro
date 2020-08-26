#-------------------------------------------------
#
# Project created by QtCreator 2018-09-27T16:58:58
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTemperature
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp  \
        qcustomplot.cpp

HEADERS  += mainwindow.h  \
            qcustomplot.h \
    config.h

RESOURCES += \
    resource.qrc
