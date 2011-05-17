#include "aslcompiler.h"
#include "aslparser_internal.h"

#include <QScopedPointer>

using namespace asl;

ASLCompiler::ASLCompiler(QObject *parent) : QObject(parent)
{
}

AnnotatedGLShaderProgram * ASLCompiler::compile(QGLShader::ShaderType type,
         const QString &source, const QString &pathOfSource)
{
    m_log.clear();
    m_success = true;

    QScopedPointer<AnnotatedGLShaderProgram> shaderPrgm(
            parserinternal::parse(source, pathOfSource));

    m_success = shaderPrgm->addShaderFromSourceCode(type, source);

    if (!shaderPrgm->log().isEmpty()) {
        m_log += shaderPrgm->log();
    }
    if (!m_success) {
        return NULL;
    }

    parserinternal::parse(source);
    m_log += parserinternal::log;

    shaderPrgm->link();
    // TODO: check for errors

    return shaderPrgm.take();
}

