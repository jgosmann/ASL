#include "aslcompiler.h"
#include "aslparser_internal.h"

#include <QScopedPointer>

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

    if (!shaderPrgm->log().isEmpty()) {
        // TODO return false
    }
    if (!m_success) {
        return NULL;
    }

    return shaderPrgm.take();
}

void ASLCompiler::reset()
{
    parserinternal::clearLog();
    m_log.clear();
    m_success = true;
}

