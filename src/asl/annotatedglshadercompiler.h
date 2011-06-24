#ifndef ANNOTATEDGLSHADERCOMPILER_H
#define ANNOTATEDGLSHADERCOMPILER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QSet>

#include "annotatedglshader.h"

namespace asl
{
class AnnotatedGLShaderCompiler
{
public:
    virtual ~AnnotatedGLShaderCompiler() { }

    /**
     * Compiles annotated shader and returns a pointer to it.
     *
     * \note Use #success() to test whether the last compilation was
     * successful. Please note that #log() may return a string
     * containing warnings (but not errors) even if the compilation was
     * successful. Using AnnotatedGLShader#log() or AnnotatedGLShader#success()
     * will provide you only with information about the compilation as normal
     * OpenGL shader. Any information about the compilation of the Annotated
     * Shading Language will be missing.
     *
     * \note Use always `/' as an universal directory separator for the
     * \a pathOfSource argument.
     *
     * \attention You have to free the memory allocated for the shader when you
     * do not need it anymore. Use \c delete for that.
     */
    virtual asl::AnnotatedGLShader * compile(QGLShader::ShaderType type,
            const QString &source, const QString &pathOfSource = "") = 0;

    /**
     * Compiles an annotated shader as a main shader (a shader which was not
     * included as dependency) and returns a pointer to it. See #compile() for
     * further information.
     */
    virtual asl::AnnotatedGLShader * compileAsMain(QGLShader::ShaderType type,
            const QString &source, const QString &pathOfSource = "") = 0;

    /**
     * Reads a file and compiles the source code. See #compile() for more
     * information.
     */
    virtual asl::AnnotatedGLShader * compileFile(QGLShader::ShaderType type,
            const QString &filename) = 0;

    /**
     * Reads a file and compiles the source code as main shader (a shader which
     * was not included as dependency) and returns a pointer to it. See
     * #compile() for further information.
     */
    virtual asl::AnnotatedGLShader * compileFileAsMain(
            QGLShader::ShaderType type, const QString &filename) = 0;

    virtual QString log() const = 0;

    /**
     * \note Even if \c true is returned #log() may return a string
     * containing warnings (but not errors).
     */
    virtual const bool success() const = 0;
};
}

#endif // ANNOTATEDGLSHADERCOMPILER_H
