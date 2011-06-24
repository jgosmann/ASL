#ifndef ASLCOMPILER_H
#define ASLCOMPILER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QSet>

#include "annotatedglshader.h"
#include "annotatedglshadercompiler.h"
#include "exportedfunctionsretriever.h"

namespace asl
{
class ASLCompiler : public asl::AnnotatedGLShaderCompiler
{
public:
    ASLCompiler(ExportedFunctionsRetriever &exportedFunctionsRetriever)
            : m_exportedFunctionsRetriever(exportedFunctionsRetriever) { }

    asl::AnnotatedGLShader * compile(QGLShader::ShaderType type,
            const QString &source, const QString &pathOfSource = "");
    asl::AnnotatedGLShader * compileAsMain(QGLShader::ShaderType type,
            const QString &source, const QString &pathOfSource = "");

    asl::AnnotatedGLShader * compileFile(QGLShader::ShaderType type,
            const QString &filename);
    asl::AnnotatedGLShader * compileFileAsMain(QGLShader::ShaderType type,
            const QString &filename);

    QString log() const { return m_log; }
    const bool success() const { return m_success; }

private:
    QString getExportedFunctionsOfDependencies(QGLShader::ShaderType shaderType,
             const QString &pathOfSource, const ShaderInfo &shaderInfo);
    static void prefixKeepingVersionAndLineIntact(const QString &prefix,
            QString &source);
    QString readFile(const QString &filename);
    void reset();

    QString m_log;
    bool m_success;

    ExportedFunctionsRetriever &m_exportedFunctionsRetriever;
};
}

#endif // ASLCOMPILER_H
