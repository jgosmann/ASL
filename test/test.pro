#-------------------------------------------------
#
# Project created by QtCreator 2011-04-23T16:13:14
#
#-------------------------------------------------

include(../src/src.pro)
SOURCES -= main.cpp
for(file, HEADERS) {
    correctPath = ../src/$${file}
    HEADERS -= $$file
    HEADERS += $$correctPath
}
for(file, SOURCES) {
    correctPath = ../src/$${file}
    SOURCES -= $$file
    SOURCES += $$correctPath
}
for(file, LEXSOURCES) {
    correctPath = ../src/$${file}
    LEXSOURCES -= $$file
    LEXSOURCES += $$correctPath
}
for(file, YACCSOURCES) {
    correctPath = ../src/$${file}
    YACCSOURCES -= $$file
    YACCSOURCES += $$correctPath
}
for(file, FORMS) {
    correctPath = ../src/$${file}
    FORMS -= $$file
    FORMS += $$correctPath
}

LIBS += -lcppunit

TARGET = cg_test

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += warn_on

DESTDIR = ../bin
MOC_DIR = ../moc
OBJECTS_DIR = ../obj
UI_DIR = ../ui

include(asl/asl.pri)
HEADERS += common.h
SOURCES += testmain.cpp \
    common.cpp

DEFINES += TEST
