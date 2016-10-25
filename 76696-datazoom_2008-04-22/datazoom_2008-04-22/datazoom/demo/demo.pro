# #####################################################################
# DataZoom Demo Project file
# #####################################################################
TEMPLATE = app

include(demo.prh)
include(demo.pri)
include(../../dateinterval/dateinterval.prf)
include(../../tableprinter/tableprinter.prf)
include(../../csvexporter/csvexporter.prf)
include(../datazoom.prf)
include(../../dialogs/dialogs.prf)

DEPENDPATH += ../forms ../src \
	      ../../dateinterval/forms \
	      ../../dialogs/forms \
	      ../../csvexporter/forms \
	      ../../tableprinter/forms

INCLUDEPATH += ../src \
	       ../../dateinterval/src \
	       ../../dialogs/src \
	       ../../csvexporter/src \
	       ../../tableprinter/src

TARGET = demo_datazoom

CONFIG += debug warn_on assistant
CONFIG -= release
MOC_DIR = tmp/moc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj
RCC_DIR += tmp/rcc
LANGUAGE = C++
win32:debug:CONFIG += console
QT += core gui sql

RESOURCES += ../../tableprinter/tableprinterresource.qrc

LIBS += -L../../lib -ldatazoom -ldateinterval -ldialogs -lcsvexporter -ltableprinter
unix:POST_TARGETDEPS += ../../lib/libdatazoom.a ../../lib/libdialogs.a ../../lib/libdateinterval.a \
			../../lib/libtableprinter.a ../../lib/libcsvexporter.a
