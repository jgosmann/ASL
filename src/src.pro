QT += core gui opengl
CONFIG += warn_on

TARGET = cg
TEMPLATE = app

include(asl/asl.pri)
include(gui/gui.pri)
include(render/render.pri)

SOURCES += main.cpp

DESTDIR = ../bin
MOC_DIR = ../moc
OBJECTS_DIR = ../obj
UI_DIR = ../ui