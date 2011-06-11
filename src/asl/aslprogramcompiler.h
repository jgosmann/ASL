#ifndef ASLPROGRAMCOMPILER_H
#define ASLPROGRAMCOMPILER_H

#include "annotatedglshadercompiler.h"
#include "annotatedglshaderprogram.h"
#include "cachedannotatedglshadercompiler.h"
#include "dependencylocator.h"

namespace asl
{
class ASLProgramCompiler
{
public:
    ASLProgramCompiler(AnnotatedGLShaderCompiler &compiler,
            DependencyLocator &dependencyLocator)
        : m_shaderCache(compiler), m_dependencyLocator(dependencyLocator),
        m_compiler(compiler) { }

    asl::AnnotatedGLShaderProgram * compileFile(QGLShader::ShaderType type,
            const QString &filename);

    const QString log() const { return m_log; }
    bool success() const { return m_success; }

private:
    void compileAndAddShader(const QString &filename);
    void compileAndAddDependencies(const QStringList &dependencies,
            const QString &includingFile);

    CachedAnnotatedGLShaderCompiler m_shaderCache;
    DependencyLocator &m_dependencyLocator;
    AnnotatedGLShaderCompiler &m_compiler;

    bool m_success;
    QGLShader::ShaderType m_shaderType;
    QSet<QString> m_addedShaders;
    AnnotatedGLShaderProgram *m_programUnderConstruction;

    QString m_log;
};

} /* namespace asl */


#endif /* ASLPROGRAMCOMPILER_H */

