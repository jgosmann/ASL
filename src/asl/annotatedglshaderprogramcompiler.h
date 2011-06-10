#ifndef ANNOTATEDGLSHADERPROGRAMCOMPILER_H
#define ANNOTATEDGLSHADERPROGRAMCOMPILER_H

#include "annotatedglshadercompiler.h"
#include "annotatedglshaderprogram.h"

namespace asl
{
class AnnotatedGLShaderProgramCompiler
{
public:
    AnnotatedGLShaderProgramCompiler(AnnotatedGLShaderCompiler &compiler)
        : m_compiler(compiler) { }

    asl::AnnotatedGLShaderProgram * compileFile(QGLShader::ShaderType type,
            const QString &filename);

    const QString log() const { return QString(); }
    bool success() const { return true; }

private:
    AnnotatedGLShaderCompiler &m_compiler;
};

} /* namespace asl */


#endif /* ANNOTATEDGLSHADERPROGRAMCOMPILER_H */

