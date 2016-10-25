# #####################################################################
# DataZoom Designer Plugin Project file
# #####################################################################

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

SOURCES  += twdatazoomplugin.cpp 
HEADERS  += twdatazoomplugin.h 

DESTDIR   = $(QTDIR)/plugins/designer
TEMPLATE     = lib
CONFIG      += designer plugin release

unix:LIBS      += -L$(QTDIR)/lib 

unix:POST_TARGETDEPS += ../../lib/libdateinterval.a ../../lib/libdatazoom.a ../../lib/libdialogs.a ../../lib/libtableprinter.a ../../lib/libcsvexporter.a

LIBS += -L../../lib -ldatazoom -ldateinterval -ldialogs -lcsvexporter -ltableprinter

target.path=$(QTDIR)/plugins/designer
#target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

MOC_DIR = tmp/moc
UI_DIR = tmp/ui
OBJECTS_DIR = tmp/obj
RCC_DIR += tmp/rcc
LANGUAGE = C++
win32:debug:CONFIG += console
QT += sql core gui assistant
