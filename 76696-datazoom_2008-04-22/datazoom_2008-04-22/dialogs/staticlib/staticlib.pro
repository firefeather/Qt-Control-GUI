include(../dialogs.pri)
include(../dialogs.prh)
include(../dialogs.prf)

MOC_DIR = tmp/moc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc

LANGUAGE = C++
QT +=  core gui

CONFIG += qt debug warn_on staticlib
CONFIG -= release

DEPENDPATH    += ../src ../forms ../../dateinterval/forms
INCLUDEPATH    += ../src ./  $(QTDIR)/include ../../dateinterval/src

DESTDIR   = ../../lib
TEMPLATE     = lib
LIBS += -L../../lib -ldateinterval
TARGET=dialogs
