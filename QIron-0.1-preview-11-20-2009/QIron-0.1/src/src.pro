TEMPLATE = lib
DEFINES += QIRON_LIBRARY_EXPORT
TARGET = QIron
VERSION = 1.0.0
DESTDIR = ../lib

build_pass:CONFIG(debug, debug|release) {
	TARGET = $$join(TARGET,,,d)
}
macx {
	CONFIG += absolute_library_soname
}
include(Common/Common.pri)
include(QIrQuadSplitter/QIrQuadSplitter.pri)
include(QIrBreadCrumbBar/QIrBreadCrumbBar.pri)
include(QIrExpander/QIrExpander.pri)
include(QIrDock/QIrDock.pri)
