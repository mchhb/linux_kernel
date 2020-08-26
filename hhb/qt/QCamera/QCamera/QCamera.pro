#-------------------------------------------------
#
# Project created by QtCreator 2019-10-02T15:54:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QCamera
TEMPLATE = app


SOURCES += main.cpp\
    v4l2camera.cpp \
    informationdlg.cpp \
    camera_app.cpp

HEADERS  += \
    v4l2camera.h \
    informationdlg.h \
    camera_app.h

RESOURCES += \
    icon.qrc
