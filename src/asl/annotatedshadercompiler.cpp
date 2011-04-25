#include "annotatedshadercompiler.h"

#include <QScopedPointer>

#include "compilationexception.h"

using namespace asl;

AnnotatedShaderCompiler::AnnotatedShaderCompiler(QObject *parent) :
    QObject(parent)
{
}

AnnotatedGLShaderProgram *
AnnotatedShaderCompiler::compile(QGLShader::ShaderType type,
                                 const QString &source)
{
    QScopedPointer<AnnotatedGLShaderProgram> shaderPrgm(
            new AnnotatedGLShaderProgram());
    bool success = shaderPrgm->addShaderFromSourceCode(type, source);

    if (!success || !shaderPrgm->log().isEmpty()) {
        throw CompilationException(CompilationException::COMPILATION,
                shaderPrgm->log());
    }

    shaderPrgm->link();
    // TODO: check for errors

    return shaderPrgm.take();
}

