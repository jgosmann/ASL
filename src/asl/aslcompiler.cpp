#include "aslcompiler.h"
#include "aslparser_internal.h"

#include <QFile>
#include <QTextStream>

using namespace asl;

AnnotatedGLShader * ASLCompiler::compile(QGLShader::ShaderType type,
         const QString &source, const QString &pathOfSource)
{
    reset();

    const QString prefixedSource(m_prefix + source);

    ShaderInfo shaderInfo = parserinternal::parse(prefixedSource, pathOfSource);
    m_log += parserinternal::log;

    AnnotatedGLShader *shaderPrgm = new AnnotatedGLShader(type, shaderInfo);
    m_success = shaderPrgm->compileSourceCode(prefixedSource);
    m_log += shaderPrgm->log();

    return shaderPrgm;
}

asl::AnnotatedGLShader * ASLCompiler::compileFile(QGLShader::ShaderType type,
        const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        m_log += "ERROR: Could not open file \"" + filename + "\".\n";
        return compile(type, "", filename);
    }

    QTextStream inStream(&file);
    AnnotatedGLShader *retVal = compile(type, inStream.readAll(), filename);
    file.close();
    return retVal;
}

void ASLCompiler::reset()
{
    parserinternal::clearLog();
    m_log.clear();
    m_success = true;
}

