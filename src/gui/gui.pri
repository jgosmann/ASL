SOURCES += \
    src/gui/mainwindow.cpp \
    src/gui/glimagerenderer.cpp \
    src/gui/glimageviewer.cpp \
    src/gui/shaderparameterbundle.cpp \
    src/gui/shaderlistview.cpp \
    src/gui/shaderitemlist.cpp \
    src/gui/shaderitem.cpp

HEADERS  += \
    src/gui/mainwindow.h \
    src/gui/glimagerenderer.h \
    src/gui/glimageviewer.h \
    src/gui/shaderparameterbundle.h \
    src/gui/widgetwrapper.h \
    src/gui/doublespinboxwidgetwrapper.h \
    src/gui/spinboxwidgetwrapper.h \
    src/gui/checkboxwidgetwrapper.h \
    src/gui/doublesliderwidgetwrapper.h \
    src/gui/sliderwidgetwrapper.h \
    src/gui/shaderparametercontrol.h \
    src/gui/shaderparametercontrol.cpp \ # template class, do not compile as cpp
    src/gui/shaderparametercontrolhandle.h \
    src/gui/shaderlistview.h \
    src/gui/shaderitemlist.h \
    src/gui/shaderitem.h

FORMS    += \
    src/gui/mainwindow.ui
