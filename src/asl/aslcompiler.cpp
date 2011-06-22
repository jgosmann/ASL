#include "aslcompiler.h"
#include "aslparser_internal.h"

#include <QFile>
#include <QRegExp>
#include <QTextStream>

using namespace asl;
AnnotatedGLShader * ASLCompiler::compile(QGLShader::ShaderType type,
        const QString &source, const QString &pathOfSource)
{
    reset();

    QString prefixedSource(source);
    prefixSource(prefixedSource);

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

void ASLCompiler::prefixSource(QString &source) const
{
    const QRegExp versionDirectiveMatcher(
            "^\\s*#version\\s+[^\r\n]*([\r\n]+|$)");
    const int pos = versionDirectiveMatcher.indexIn(source);
    if (pos < 0) {
        source.prepend(m_prefix);
    } else {
        source.insert(pos + versionDirectiveMatcher.matchedLength(), m_prefix);
    }
}

void ASLCompiler::reset()
{
    parserinternal::clearLog();
    m_log.clear();
    m_success = true;
}

