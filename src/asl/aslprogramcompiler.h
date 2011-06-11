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

    /**
     * Compiles an ASL shader program with its dependencies and returns a
     * pointer to it.
     *
     * \note Use #success() to test whether the last compilation was successful.
     * Please note that #log() may return a string containing warnings and
     * information messages (but not errors) even if the compilation was
     * successful. Using AnnotatedGLShaderProgram#log() or
     * AnnotatedGLShaderProgram#success() will provide you only with information
     * about the linking as normal OpenGL shader. Any information about the
     * compilation of the Annotated Shading Language or individual shaders will
     * be missing.
     *
     * \note Use always `/' as an universal directory separator for the \a
     * filename argument.
     *
     * \attention You have to free the memory allocated for the shader when you
     * do not need it anymore. Use \c delete for that.
     *
     * \note All loaded shaders will be cached and the same instance will be
     * used in each shader program using the shader. The memory of a shader will
     * be freed after destroying the last shader program using the shader.
     */
    asl::AnnotatedGLShaderProgram * compileFile(QGLShader::ShaderType type,
            const QString &filename);

    const QString log() const { return m_log; }
    bool success() const { return m_success; }

private:
    void compileAndAddShaderAndLoadDependencies(const QString &filename);
    QSharedPointer<AnnotatedGLShader> compileShader(const QString &filename);
    void addShader(const QString &filename,
            QSharedPointer<AnnotatedGLShader> &shader);
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

