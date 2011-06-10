
#include "annotatedglshaderprogramcompiler.h"

using namespace asl;

asl::AnnotatedGLShaderProgram * AnnotatedGLShaderProgramCompiler::compileFile(
        QGLShader::ShaderType type, const QString &filename)
{
    AnnotatedGLShaderProgram * program =
        new AnnotatedGLShaderProgram(ShaderInfo());

    QSharedPointer<AnnotatedGLShader> mainShader(
        m_compiler.compileFile(type, filename));
    bool success = program->addSharedShader(
        qSharedPointerCast<QGLShader>(mainShader));

    success &= program->link();

    return program;
}

