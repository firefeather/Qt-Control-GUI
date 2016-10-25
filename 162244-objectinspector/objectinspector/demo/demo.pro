#-------------------------------------------------
#
# Project created by QtCreator 2013-12-10T17:12:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo
TEMPLATE = app

INCLUDEPATH += \
    ../objectinspector \
    ../objectinspector/model

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L/PATH_TO_YOUR_BUILD/objectinspector -lobjectinspector_1

