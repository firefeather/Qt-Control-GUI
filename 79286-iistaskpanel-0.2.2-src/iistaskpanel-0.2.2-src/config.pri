MAJOR_VERSION = 0
VERSION       = 0.2.2
#SUFFIX        = "snapshot"

# Do not change this line
CONFIG += qt

# Choose one of the following two lines to configure the build

CONFIG += release
#CONFIG += debug
#CONFIG += debug_and_release

# Choose this to build static library and tests
#CONFIG += static


# Configuration for MacOS X
macx {
        # Using gcc
        QMAKESPEC=macx-g++
        # uncoment this line if you want use xcode
        # QMAKESPEC=macx-xcode
}

CONFIG(debug, debug|release) {
        unix {
                TARGET = $$join(TARGET,,,_debug)
        }
        else: TARGET = $$join(TARGET,,,d)

  MOC_DIR = ./debug
        OBJECTS_DIR = ./debug
        UI_DIR = ./debug
        RCC_DIR = ./debug
}

CONFIG(release, debug|release) {
        MOC_DIR = ./release
        OBJECTS_DIR = ./release
        UI_DIR = ./release
        RCC_DIR = ./release
}

QIISTASKPANELLIB = iistaskpanel

CONFIG(debug, debug|release) {
    unix: QIISTASKPANELLIB = $$join(QIISTASKPANELLIB,,,_debug)
    else: QIISTASKPANELLIB = $$join(QIISTASKPANELLIB,,,d)
}

contains(CONFIG, static) {
    DEFINES += QIIS_STATICLIB
} else {
    DEFINES += QIIS_SHAREDLIB
    win32 {
        QIISTASKPANELLIB = $$join(QIISTASKPANELLIB,,,$$MAJOR_VERSION)
    }
}

