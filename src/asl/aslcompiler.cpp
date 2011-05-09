#include "aslcompiler.h"

#include <QFileInfo>
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
            new AnnotatedGLShaderProgram(
                QFileInfo(pathOfSource).fileName(), ""));
    bool success = shaderPrgm->addShaderFromSourceCode(type, source);

    if (!success || !shaderPrgm->log().isEmpty()) {
        throw CompilationException(CompilationException::COMPILATION,
                shaderPrgm->log());
    }

    shaderPrgm->link();
    // TODO: check for errors

    return shaderPrgm.take();
}

