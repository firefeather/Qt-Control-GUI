TEMPLATE = app
TARGET = actions

include(../tests.pri)

HEADERS += actions.h
SOURCES += actions.cpp main.cpp

FORMS += actions.ui

RESOURCES += actions.qrc

QMAKE_CLEAN += ""core.* $(TARGET)""

