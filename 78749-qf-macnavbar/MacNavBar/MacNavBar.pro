# ====================================================================
#  Quartica Framework - Mac NavBar
# ====================================================================

TEMPLATE = lib
TARGET = 

DESTDIR = bin
UI_DIR = tmp-ui
MOC_DIR = tmp
OBJECTS_DIR = tmp

CONFIG += qt

DEPENDPATH += . include src
INCLUDEPATH += . include

# Input
QFMACNAVBAR.CLASSES =	include/QfClickableLabel \
						include/QfNavBarGroup \
						include/QfNavBarItem \
						include/QfSelectableWidget \
						include/QfUiExport

QFMACNAVBAR.HEADERS = 	include/qf_clickablelabel.h \
						include/qf_navbar.h \
						include/qf_navbargroup.h \
						include/qf_navbaritem.h \
						include/qf_selectablewidget.h \
						include/qf_uiexport.h \

QFMACNAVBAR.SOURCES = 	src/clickablelabel.cpp \
						src/navbar.cpp \
			           	src/navbargroup.cpp \
			           	src/navbaritem.cpp \
						src/selectablewidget.cpp

HEADERS += $$QFMACNAVBAR.HEADERS $$QFMACNAVBAR.CLASSES
SOURCES += $$QFMACNAVBAR.SOURCES

macx {
	CONFIG += lib_bundle

	FRAMEWORK_HEADERS.version = Versions
	FRAMEWORK_HEADERS.files = $$QFMACNAVBAR.HEADERS $$QFMACNAVBAR.CLASSES
	FRAMEWORK_HEADERS.path = Headers

	QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS
}

win32 {
	CONFIG += shared dll release
	DEFINES += QFUI_MAKEDLL
}

