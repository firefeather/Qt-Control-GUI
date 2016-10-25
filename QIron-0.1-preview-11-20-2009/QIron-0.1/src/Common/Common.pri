HEADERS += \
	Common/private/qirobject_p.h \
	Common/ui/qirwidget_ui.h \
	Common/qiron_export.h \
	Common/qirmacros.h \
	Common/qirwidget.h \
	Common/qirboolblocker.h \
	Common/qirsubstyle.h \
	Common/qirsmartpointer.h
win32 {
HEADERS += Common/qirwindows.h
}

SOURCES += \
	Common/qirwidget.cpp \
	Common/qirobject.cpp \
	Common/qirsubstyle.cpp \
	Common/qirboolblocker.cpp \