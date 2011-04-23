#-------------------------------------------------
#
# Project created by QtCreator 2011-04-13T10:38:35
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += warn_on

TARGET = cg
TEMPLATE = app

include(src/asl/asl.pri)
include(src/gui/gui.pri)
include(src/render/render.pri)

SOURCES += src/main.cpp

DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = ui
