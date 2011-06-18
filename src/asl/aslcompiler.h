#ifndef ASLCOMPILER_H
#define ASLCOMPILER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QSet>

#include "annotatedglshader.h"
#include "annotatedglshadercompiler.h"

namespace asl
{
class ASLCompiler : public asl::AnnotatedGLShaderCompiler
{
public:
    asl::AnnotatedGLShader * compile(QGLShader::ShaderType type,
            const QString &source, const QString &pathOfSource = "");

    asl::AnnotatedGLShader * compileFile(QGLShader::ShaderType type,
            const QString &filename);

    void prefixSourcesWith(const QString &prefix) { m_prefix = prefix; }

    QString log() const { return m_log; }
    const bool success() const { return m_success; }

private:
    void reset();

    QString m_log;
    bool m_success;

    QString m_prefix;
};
}

#endif // ASLCOMPILER_H
