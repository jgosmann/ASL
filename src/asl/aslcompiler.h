#ifndef ASLCOMPILER_H
#define ASLCOMPILER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QSet>

#include "annotatedglshader.h"

namespace asl
{
class ASLCompiler
{
public:
    /**
     * Compiles and links an annotated shader and returns a pointer to it.
     *
     * \note Use ASLCompiler#success() to test whether the last compilation was
     * successful. Please note that ASLCompiler#log() may return a string
     * containing warnings (but not errors) even if the compilation was
     * successful.
     *
     * \note Use always `/' as an universal directory separator for the
     * \a pathOfSource argument.
     *
     * \attention You have to free the memory allocated for the shader when you
     * do not need it anymore. Use \c delete for that.
     */
    asl::AnnotatedGLShader * compile(QGLShader::ShaderType type,
            const QString &source, const QString &pathOfSource = "");

    /**
     * Reads a file and compiles the source code. See #compile() for more
     * information
     */
    asl::AnnotatedGLShader * compileFile(QGLShader::ShaderType type,
            const QString &filename);

    QString log() const { return m_log; }

    /**
     * \note Even if \c true is returned ASLCompiler#log() may return a string
     * containing warnings (but not errors).
     */
    const bool success() const { return m_success; }

private:
    void reset();

    QString m_log;
    bool m_success;
};
}

#endif // ASLCOMPILER_H
