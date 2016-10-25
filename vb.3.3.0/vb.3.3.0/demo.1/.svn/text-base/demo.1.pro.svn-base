SOURCES += main.cpp 

TEMPLATE = app

TARGET = demo

LIBPATH = ../lib

INCLUDEPATH += ./ ../include ../src/core ../src/addons

#CONFIG += debug
#CONFIG += release

CONFIG(release, debug|release) {
    UIC_DIR = release
    MOC_DIR = release
    OBJECTS_DIR = release
    LIBS += -lvb4
}

CONFIG(debug, debug|release) {
    UIC_DIR = debug
    MOC_DIR = debug
    OBJECTS_DIR = debug
    LIBS += -lvb4d
}
