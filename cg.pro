#-------------------------------------------------
#
# Project created by QtCreator 2011-04-13T10:38:35
#
#-------------------------------------------------

QT       += core gui opengl

CONFIG += warn_on

TARGET = cg
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/glimageviewer.cpp

HEADERS  += src/mainwindow.h \
    src/glimageviewer.h

FORMS    += src/mainwindow.ui

DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = ui
