#include "aslcompiler.h"
#include "aslparser_internal.h"

#include <QScopedPointer>

#include "compilationexception.h"

using namespace asl;

ASLCompiler::ASLCompiler(QObject *parent) : QObject(parent)
{
}

AnnotatedGLShaderProgram * ASLCompiler::compile(QGLShader::ShaderType type,
         const QString &source, const QString &pathOfSource)
{
    QScopedPointer<AnnotatedGLShaderProgram> shaderPrgm(
            parserinternal::parse(source, pathOfSource));

    bool success = shaderPrgm->addShaderFromSourceCode(type, source);

    if (!success || !shaderPrgm->log().isEmpty()) {
        throw CompilationException(CompilationException::COMPILATION,
                shaderPrgm->log());
    }

    parserinternal::parse(source);

    shaderPrgm->link();
    // TODO: check for errors

    return shaderPrgm.take();
}

