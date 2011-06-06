#include "aslcompiler.h"
#include "aslparser_internal.h"

#include <QScopedPointer>

using namespace asl;

ASLCompiler::ASLCompiler(QSharedPointer<DependencyReader> dependencyReader,
             QObject *parent)
        : QObject(parent), m_dependencyReader(dependencyReader)
{
}

AnnotatedGLShaderProgram * ASLCompiler::compile(QGLShader::ShaderType type,
         const QString &source, const QString &pathOfSource)
{
    parserinternal::clearLog();
    m_log.clear();
    m_success = true;

    QScopedPointer<AnnotatedGLShaderProgram> shaderPrgm(
            parserinternal::parse(source, pathOfSource));
    m_log += parserinternal::log;

    m_success = shaderPrgm->addShaderFromSourceCode(type, source);

    if (!shaderPrgm->log().isEmpty()) {
        m_log += shaderPrgm->log();
    }
    if (!m_success) {
        return NULL;
    }


    shaderPrgm->link();
    // TODO: check for errors

    return shaderPrgm.take();
}

