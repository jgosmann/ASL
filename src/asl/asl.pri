HEADERS += asl/shaderparameterinfo.h \
    asl/gltypeinfo.h \
    asl/annotatedglshaderprogram.h \
    asl/aslcompiler.h \
    asl/aslpreprocessor.h \
    asl/aslpreprocessor_parserinternal.h \
    asl/aslparser_internal.h \
    asl/annotatedglshaderprogrambuilder.h \
    asl/shaderparameterinfobuilder.h \
    asl/glvariant.h \
    asl/gltypenames.h \
    asl/dependencyreader.h

SOURCES += \
    asl/annotatedglshaderprogram.cpp \
    asl/aslcompiler.cpp \
    asl/aslpreprocessor.cpp \
    asl/shaderparameterinfobuilder.cpp \
    asl/gltypeinfo.cpp \
    asl/glvariant.cpp \
    asl/gltypenames.cpp

LEXSOURCES += asl/aslpreprocessor.l \
    asl/aslparser.l

YACCSOURCES += asl/aslpreprocessor.y \
    asl/aslparser.y

