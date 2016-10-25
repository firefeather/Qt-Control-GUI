include(../dateinterval.pri)
include(../dateinterval.prh)
include(../dateinterval.prf)

MOC_DIR = tmp/moc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc

LANGUAGE = C++
QT +=  core gui

CONFIG += qt debug warn_on staticlib
CONFIG -= release


DEPENDPATH    += ../src ../forms
INCLUDEPATH    += ../src ./  $(QTDIR)/include

DESTDIR   = ../../lib
TEMPLATE     = lib
TARGET=dateinterval
