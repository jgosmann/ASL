#ifndef ANNOTATEDGLSHADERPROGRAMCOMPILER_H
#define ANNOTATEDGLSHADERPROGRAMCOMPILER_H

#include "annotatedglshadercompiler.h"
#include "annotatedglshaderprogram.h"
#include "cachedannotatedglshadercompiler.h"

namespace asl
{
class AnnotatedGLShaderProgramCompiler
{
public:
    AnnotatedGLShaderProgramCompiler(AnnotatedGLShaderCompiler &compiler)
        : m_shaderCache(compiler) { }

    asl::AnnotatedGLShaderProgram * compileFile(QGLShader::ShaderType type,
            const QString &filename);

    const QString log() const { return QString(); }
    bool success() const { return true; }

private:
    CachedAnnotatedGLShaderCompiler m_shaderCache;
    //AnnotatedGLShaderCompiler &m_compiler;
};

} /* namespace asl */


#endif /* ANNOTATEDGLSHADERPROGRAMCOMPILER_H */

