#VERSION = 3.3

HEADERS += defines.h \
    libvb4.h \
    ../include/iisVB.h \
    ../include/iisVBAddons.h \    
    ../src/addons/iisBoolValue.h \
    ../src/addons/iisColorValue.h \ 
    ../src/addons/iisNumericValue.h \ 
    ../src/addons/iisTextValue.h \ 
    ../src/addons/iisFontValue.h \    
    ../src/addons/iisCoordinateValue.h \    
    ../src/addons/iisTextEditorDialog.h \
    ../src/addons/iisPathValue.h \
    ../src/core/iisDropList.h \ 
    ../src/core/iisEditors.h \ 
    ../src/core/iisItemList.h \ 
    ../src/core/iisItems.h \     
    ../src/core/iisValidators.h \  
    ../src/core/iisValueBrowser.h 
    
SOURCES += libvb4.cpp \
    ../src/addons/iisBoolValue.cpp \
    ../src/addons/iisColorValue.cpp \ 
    ../src/addons/iisNumericValue.cpp \ 
    ../src/addons/iisTextValue.cpp \ 
    ../src/addons/iisFontValue.cpp \ 
    ../src/addons/iisCoordinateValue.cpp \
    ../src/addons/iisPathValue.cpp \
    ../src/core/iisDropList.cpp \ 
    ../src/core/iisEditors.cpp \ 
    ../src/core/iisItemList.cpp \ 
    ../src/core/iisItems.cpp \     
    ../src/core/iisValidators.cpp \  
    ../src/core/iisValueBrowser.cpp \
    ../src/addons/iisTextEditorDialog.cpp

#uncomment for QT_VERSION = 4.1.x
#FORMS += ../src/addons/iisTextEditorDialog-4-1-x.ui

#uncomment for QT_VERSION = 4.2.x
FORMS += ../src/addons/iisTextEditorDialog.ui

RESOURCES += ../images/images.qrc
    
TEMPLATE = lib

DESTDIR = ../lib

INCLUDEPATH += ./ ../include ../src/core ../src/addons

#CONFIG += debug
#CONFIG += release

CONFIG += staticlib

CONFIG(release, debug|release) {
    UI_DIR = release
    MOC_DIR = release
    OBJECTS_DIR = release
    TARGET = vb4
}

CONFIG(debug, debug|release) {
    UI_DIR = debug
    MOC_DIR = debug
    OBJECTS_DIR = debug
    TARGET = vb4d
}
