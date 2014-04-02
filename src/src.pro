#-------------------------------------------------
#
# Project created by hrobeers 2014-04-02T08:17:00
#
#-------------------------------------------------

include(../common.pri)

TARGET = jenson
TEMPLATE = lib

VERSION = 0.1.0

DEFINES += JENSON_LIBRARY

SOURCES += \

HEADERS +=\

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    ../README.md \
    ../LICENSE.BSD
