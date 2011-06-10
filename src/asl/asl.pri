HEADERS += \
    asl/annotated.h \
    asl/annotatedglshader.h \
    asl/annotatedglshadercompiler.h \
    asl/annotatedglshaderprogram.h \
    asl/annotatedglshaderprogrambuilder.h \
    asl/aslcompiler.h \
    asl/aslparser_internal.h \
    asl/aslpreprocessor.h \
    asl/aslpreprocessor_parserinternal.h \
    asl/cachedannotatedglshadercompiler.h \
    asl/dependencylocator.h \
    asl/gltypeinfo.h \
    asl/gltypenames.h \
    asl/glvariant.h \
    asl/relativedependencylocator.h \
    asl/shaderinfo.h \
    asl/shaderparameterinfo.h \
    asl/shaderparameterinfobuilder.h \

SOURCES += \
    asl/annotatedglshaderprogram.cpp \
    asl/aslcompiler.cpp \
    asl/aslpreprocessor.cpp \
    asl/cachedannotatedglshadercompiler.cpp \
    asl/gltypeinfo.cpp \
    asl/gltypenames.cpp \
    asl/glvariant.cpp \
    asl/relativedependencylocator.cpp \
    asl/shaderparameterinfobuilder.cpp \

LEXSOURCES += asl/aslpreprocessor.l \
    asl/aslparser.l

YACCSOURCES += asl/aslpreprocessor.y \
    asl/aslparser.y

