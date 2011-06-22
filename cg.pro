QT += core gui opengl
CONFIG += warn_on
TEMPLATE = app

QMAKE_CFLAGS_RELEASE += -D NDEBUG
QMAKE_CXXFLAGS_RELEASE += -D NDEBUG

DESTDIR = bin
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = ui

include(src/src.pri)

test {
    CONFIG   += console
    CONFIG   -= app_bundle
    TARGET = cg_test

    SOURCES -= src/main.cpp

    DEFINES += COMPILES_TEST
    LIBS += -lcppunit -lgmock -lgtest -coverage

    include(test/test.pri)
} else {
    TARGET = cg
    TEMPLATE = app
}

