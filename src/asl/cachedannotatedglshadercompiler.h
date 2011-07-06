#ifndef CACHEDANNOTATEDGLSHADERCOMPILER_H
#define CACHEDANNOTATEDGLSHADERCOMPILER_H

#include <QHash>
#include <QSharedPointer>
#include <QWeakPointer>

#include "annotatedglshadercompiler.h"

namespace asl
{
class CachedAnnotatedGLShaderCompiler
{
public:
    CachedAnnotatedGLShaderCompiler(asl::AnnotatedGLShaderCompiler &compiler);

    QSharedPointer<AnnotatedGLShader> compileFile(
            QGLShader::ShaderType type, const QString &filename);
    QSharedPointer<AnnotatedGLShader> compileFileAsMain(
            QGLShader::ShaderType type, const QString &filename);

    QString log() const { return m_log; }
    const bool success() const { return m_success; }

private:
    asl::AnnotatedGLShaderCompiler &m_compiler;

    QHash<QString, QWeakPointer<AnnotatedGLShader> > m_cache;
    QHash<QString, QWeakPointer<AnnotatedGLShader> > m_mainShaderCache;

    bool m_success;
    QString m_log;
};
} /* namespace asl */

#endif /* CACHEDANNOTATEDGLSHADERCOMPILER_H */

