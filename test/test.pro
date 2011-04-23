#-------------------------------------------------
#
# Project created by QtCreator 2011-04-23T16:13:14
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET = cg_test

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += warn_on

DESTDIR = ../bin
MOC_DIR = ../moc
OBJECTS_DIR = ../obj
UI_DIR = ../ui

SOURCES += tst_annotatedshaderloadertest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"
