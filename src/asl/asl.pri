HEADERS += asl/shaderparameterinfo.h \
    asl/gltypeinfo.h \
    asl/annotated.h \
    asl/annotatedglshader.h \
    asl/annotatedglshadercompiler.h \
    asl/annotatedglshaderprogram.h \
    asl/aslcompiler.h \
    asl/aslpreprocessor.h \
    asl/aslpreprocessor_parserinternal.h \
    asl/aslparser_internal.h \
    asl/annotatedglshaderprogrambuilder.h \
    asl/cachedannotatedglshadercompiler.h \
    asl/shaderparameterinfobuilder.h \
    asl/glvariant.h \
    asl/gltypenames.h \
    asl/dependencyreader.h \
    asl/shaderinfo.h

SOURCES += \
    asl/aslcompiler.cpp \
    asl/aslpreprocessor.cpp \
    asl/annotatedglshaderprogram.cpp \
    asl/cachedannotatedglshadercompiler.cpp \
    asl/shaderparameterinfobuilder.cpp \
    asl/gltypeinfo.cpp \
    asl/glvariant.cpp \
    asl/gltypenames.cpp

LEXSOURCES += asl/aslpreprocessor.l \
    asl/aslparser.l

YACCSOURCES += asl/aslpreprocessor.y \
    asl/aslparser.y

