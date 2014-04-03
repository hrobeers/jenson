#-------------------------------------------------
#
# Project created by hrobeers 2014-04-02T08:20:07
#
#-------------------------------------------------

QT       += testlib

include(../common.pri)

TARGET = tests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    jensontests.cpp

LIBS += -L$$PWD/../bin/ -ljenson

INCLUDEPATH +=  $$PWD/../src \
                ../

HEADERS += \
    jensontests.h
