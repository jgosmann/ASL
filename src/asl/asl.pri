HEADERS += \
    src/asl/annotated.h \
    src/asl/annotatedglshader.h \
    src/asl/annotatedglshadercompiler.h \
    src/asl/annotatedglshaderprogram.h \
    src/asl/aslcompiler.h \
    src/asl/aslparser_internal.h \
    src/asl/aslpreprocessor.h \
    src/asl/aslpreprocessor_parserinternal.h \
    src/asl/aslprogramcompiler.h \
    src/asl/cachedannotatedglshadercompiler.h \
    src/asl/defaultaslprogramcompiler.h \
    src/asl/dependencylocator.h \
    src/asl/exportedfunctionsretriever.h \
    src/asl/gltypeinfo.h \
    src/asl/gltypenames.h \
    src/asl/glvariant.h \
    src/asl/relativedependencylocator.h \
    src/asl/shaderinfo.h \
    src/asl/shaderparameterinfo.h \
    src/asl/shaderparameterinfobuilder.h \

SOURCES += \
    src/asl/annotatedglshaderprogram.cpp \
    src/asl/aslcompiler.cpp \
    src/asl/aslpreprocessor.cpp \
    src/asl/aslprogramcompiler.cpp \
    src/asl/cachedannotatedglshadercompiler.cpp \
    src/asl/gltypeinfo.cpp \
    src/asl/gltypenames.cpp \
    src/asl/glvariant.cpp \
    src/asl/relativedependencylocator.cpp \
    src/asl/shaderparameterinfobuilder.cpp \

LEXSOURCES += \
    src/asl/aslpreprocessor.l \
    src/asl/aslparser.l

YACCSOURCES += \
    src/asl/aslpreprocessor.y \
    src/asl/aslparser.y

