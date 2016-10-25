include(../datazoom.prf)
include(../datazoom.pri)
include(../datazoom.prh)
include(../../dateinterval/dateinterval.prf)
include(../../tableprinter/tableprinter.prf)
include(../../csvexporter/csvexporter.prf)
include(../../dialogs/dialogs.prf)

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

DEPENDPATH += ../../dateinterval/src  ../../dateinterval/forms \ 
	      ../../tableprinter/src  ../../tableprinter/forms \
	      ../../dialogs/src  ../../dialogs/forms \
	      ../../csvexporter/src  ../../csvexporter/forms 

INCLUDEPATH += ../../dateinterval/src \ 
	       ../../tableprinter/src \
	       ../../dialogs/src \
	       ../../csvexporter/src \

DESTDIR   = ../../lib
TEMPLATE     = lib
RESOURCES += ../datazoomresource.qrc
LIBS	+= -L/usr/lib/qt4/lib
TARGET = datazoom
