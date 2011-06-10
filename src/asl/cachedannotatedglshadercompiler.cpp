
#include "cachedannotatedglshadercompiler.h"

using namespace asl;

CachedAnnotatedGLShaderCompiler::CachedAnnotatedGLShaderCompiler(
        AnnotatedGLShaderCompiler &compiler)
    : m_compiler(compiler)
{
}

QSharedPointer<AnnotatedGLShader> CachedAnnotatedGLShaderCompiler::compileFile(
        QGLShader::ShaderType type, const QString &filename)
{
    QSharedPointer<AnnotatedGLShader> shader;

    if (m_cache.contains(filename)) {
        shader = m_cache[filename];
    }

    if (shader.isNull()) {
        shader = QSharedPointer<AnnotatedGLShader>(
                m_compiler.compileFile(type, filename));
        m_cache[filename] = shader;
    }

    return shader;
}
