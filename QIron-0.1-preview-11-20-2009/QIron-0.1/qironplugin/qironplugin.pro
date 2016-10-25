TEMPLATE = lib
CONFIG += designer plugin
TARGET = qironplugin
DESTDIR = ../plugins

build_pass:CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,d)
}
macx {
	CONFIG += absolute_library_soname
}
include(qironplugin.pri)

RESOURCES += qironplugin.qrc
