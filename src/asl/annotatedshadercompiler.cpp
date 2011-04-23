#include "annotatedshadercompiler.h"

using namespace asl;

AnnotatedShaderCompiler::AnnotatedShaderCompiler(QObject *parent) :
    QObject(parent)
{
}

AnnotatedGLShaderProgram *
AnnotatedShaderCompiler::compile(QGLShader::ShaderType type,
                                 const QString &source)
{
    // TODO: Better error handling
    AnnotatedGLShaderProgram *shaderPrgm = new AnnotatedGLShaderProgram();
    shaderPrgm->addShaderFromSourceCode(type, source);
    if (!shaderPrgm->log().isEmpty())
        return shaderPrgm;

    shaderPrgm->link();
    return shaderPrgm;
}
