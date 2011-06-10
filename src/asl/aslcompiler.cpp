#include "aslcompiler.h"
#include "aslparser_internal.h"

#include <QFile>
#include <QScopedPointer>
#include <QTextStream>

using namespace asl;

AnnotatedGLShader* ASLCompiler::compile(QGLShader::ShaderType type,
         const QString &source, const QString &pathOfSource)
{
    reset();

    ShaderInfo shaderInfo = parserinternal::parse(source, pathOfSource);
    m_log += parserinternal::log;

    QScopedPointer<AnnotatedGLShader> shaderPrgm(
            new AnnotatedGLShader(type, shaderInfo));
    m_success = shaderPrgm->compileSourceCode(source);
    m_log += shaderPrgm->log();

    if (!m_success) {
        return NULL;
    }

    return shaderPrgm.take();
}

asl::AnnotatedGLShader * ASLCompiler::compileFile(QGLShader::ShaderType type,
        const QString &filename)
{
    QFile file(filename);
    QTextStream inStream(&file);
    return compile(type, inStream.readAll(), filename);
}

void ASLCompiler::reset()
{
    parserinternal::clearLog();
    m_log.clear();
    m_success = true;
}

