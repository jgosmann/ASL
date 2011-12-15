#include "aslcompiler.h"
#include "aslparser_internal.h"
#include "aslpreprocessor_parserinternal.h"

#include <QFile>
#include <QRegExp>
#include <QTextStream>

using namespace asl;
AnnotatedGLShader * ASLCompiler::compile(QGLShader::ShaderType type,
        const QString &source, const QString &pathOfSource)
{
    reset();

    QString preprocessed;
    QTextStream preprocessOut(&preprocessed, QIODevice::WriteOnly);
    ppinternal::parse(source, &preprocessOut);
    ShaderInfo shaderInfo = parserinternal::parse(preprocessed, pathOfSource);
    m_log += parserinternal::log;

    QString prefixedSource(source);
    prefixKeepingVersionAndLineIntact(
            getExportedFunctionsOfDependencies(type, pathOfSource, shaderInfo),
            prefixedSource);

    AnnotatedGLShader *shaderPrgm = new AnnotatedGLShader(type, shaderInfo);
    m_success = shaderPrgm->compileSourceCode(prefixedSource);
    m_log += shaderPrgm->log();

    return shaderPrgm;
}

AnnotatedGLShader * ASLCompiler::compileAsMain(QGLShader::ShaderType type,
        const QString &source, const QString &pathOfSource)
{
    QString prefixedSource(source);
    prefixKeepingVersionAndLineIntact("#define ASL_MAIN\n", prefixedSource);
    return compile(type, prefixedSource, pathOfSource);
}

asl::AnnotatedGLShader * ASLCompiler::compileFile(QGLShader::ShaderType type,
        const QString &filename)
{
    return compile(type, readFile(filename), filename);
}

asl::AnnotatedGLShader * ASLCompiler::compileFileAsMain(
        QGLShader::ShaderType type, const QString &filename)
{
    return compileAsMain(type, readFile(filename), filename);
}

QString ASLCompiler::getExportedFunctionsOfDependencies(
        QGLShader::ShaderType shaderType, const QString &pathOfSource,
        const ShaderInfo &shaderInfo)
{
    QStringList exportedFunctions;
    foreach (QString dependency, shaderInfo.dependencies) {
        exportedFunctions.append(
            m_exportedFunctionsRetriever.getExportedFunctionsForDependency(
                shaderType, dependency, pathOfSource));
    }
    return exportedFunctions.join("\n") + "\n";
}

void ASLCompiler::prefixKeepingVersionAndLineIntact(
        const QString &prefix, QString &source)
{
    const QRegExp versionDirectiveMatcher(
            "^\\s*#version\\s+[^\r\n]*([\r\n]+|$)");
    const int pos = versionDirectiveMatcher.indexIn(source);
    if (pos < 0) {
        source.prepend(prefix + "#line 0\n");
    } else {
        const unsigned int matchLen = versionDirectiveMatcher.matchedLength();
        unsigned int lineCount = source.left(matchLen).count('\n');
        source.insert(pos + matchLen,
                prefix + "#line " + QString::number(lineCount) + "\n");
    }
}

QString ASLCompiler::readFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly)) {
        m_log += "ERROR: Could not open file \"" + filename + "\".\n";
    }

    QTextStream inStream(&file);
    const QString contents(inStream.readAll());
    file.close();
    return contents;
}

void ASLCompiler::reset()
{
    parserinternal::clearLog();
    m_log.clear();
    m_success = true;
}

