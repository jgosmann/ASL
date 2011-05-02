HEADERS += asl/shaderparameterinfo.h \
    asl/glvariant.h \
    asl/gltypeinfo.h \
    asl/annotatedglshaderprogram.h \
    asl/annotatedshadercompiler.h \
    asl/compilationexception.h \
    asl/aslpreprocessor.h \
    asl/aslpreprocessor_parserinteral.h

SOURCES += \
    asl/annotatedglshaderprogram.cpp \
    asl/annotatedshadercompiler.cpp \
    asl/compilationexception.cpp \
    asl/aslpreprocessor.cpp

LEXSOURCES += asl/aslpreprocessor.l

YACCSOURCES += asl/aslpreprocessor.y

