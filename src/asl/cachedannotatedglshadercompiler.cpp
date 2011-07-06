
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
        m_success = true;
        m_log.clear();
    }

    if (shader.isNull()) {
        shader = QSharedPointer<AnnotatedGLShader>(
                m_compiler.compileFile(type, filename));
        m_success = m_compiler.success();
        m_log = m_compiler.log();
        if (m_compiler.success()) {
            m_cache[filename] = shader;
        }
    }

    return shader;
}

QSharedPointer<AnnotatedGLShader>
CachedAnnotatedGLShaderCompiler::compileFileAsMain(
        QGLShader::ShaderType type, const QString &filename)
{
    QSharedPointer<AnnotatedGLShader> shader;

    if (m_mainShaderCache.contains(filename)) {
        shader = m_mainShaderCache[filename];
        m_success = true;
        m_log.clear();
    }

    if (shader.isNull()) {
        shader = QSharedPointer<AnnotatedGLShader>(
                m_compiler.compileFileAsMain(type, filename));
        m_success = m_compiler.success();
        m_log = m_compiler.log();
        if (m_compiler.success()) {
            m_mainShaderCache[filename] = shader;
        }
    }

    return shader;
}

