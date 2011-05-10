HEADERS += asl/shaderparameterinfo.h \
    asl/glvariant.h \
    asl/gltypeinfo.h \
    asl/annotatedglshaderprogram.h \
    asl/aslcompiler.h \
    asl/compilationexception.h \
    asl/aslpreprocessor.h \
    asl/aslpreprocessor_parserinternal.h \
    asl/aslparser_internal.h

SOURCES += \
    asl/annotatedglshaderprogram.cpp \
    asl/aslcompiler.cpp \
    asl/compilationexception.cpp \
    asl/aslpreprocessor.cpp

LEXSOURCES += asl/aslpreprocessor.l \
    asl/aslparser.l

YACCSOURCES += asl/aslpreprocessor.y \
    asl/aslparser.y

