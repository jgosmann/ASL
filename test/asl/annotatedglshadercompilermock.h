#ifndef ANNOTATEDGLSHADERCOMPILERMOCK_H
#define ANNOTATEDGLSHADERCOMPILERMOCK_H

#include "../../src/asl/annotatedglshadercompiler.h"

#include <gmock/gmock.h>

namespace asl
{
class AnnotatedGLShaderCompilerMock : public AnnotatedGLShaderCompiler
{
    public:
        MOCK_METHOD3(compile, asl::AnnotatedGLShader *(
            QGLShader::ShaderType type, const QString &source,
            const QString &pathOfSource));
        MOCK_METHOD3(compileAsMain, asl::AnnotatedGLShader *(
            QGLShader::ShaderType type, const QString &source,
            const QString &pathOfSource));

        MOCK_METHOD2(compileFile, asl::AnnotatedGLShader *(
            QGLShader::ShaderType type, const QString &filename));
        MOCK_METHOD2(compileFileAsMain, asl::AnnotatedGLShader *(
            QGLShader::ShaderType type, const QString &filename));

        MOCK_CONST_METHOD0(log, QString());
        MOCK_CONST_METHOD0(success, bool());
};
} /* namespace asl */

#endif /* ANNOTATEDGLSHADERCOMPILERMOCK_H */

