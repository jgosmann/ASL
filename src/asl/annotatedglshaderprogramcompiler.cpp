
#include "annotatedglshaderprogramcompiler.h"

using namespace asl;

asl::AnnotatedGLShaderProgram * AnnotatedGLShaderProgramCompiler::compileFile(
        QGLShader::ShaderType type, const QString &filename)
{
    AnnotatedGLShaderProgram * program =
        new AnnotatedGLShaderProgram(ShaderInfo());

    QSharedPointer<AnnotatedGLShader> mainShader(
        m_shaderCache.compileFile(type, filename));
    bool success = program->addSharedShader(
        qSharedPointerCast<QGLShader>(mainShader));

    foreach (QString dependency, mainShader->dependencies()) {
        const QString dependencyPath(
                m_dependencyLocator.locate(dependency, filename));
        QSharedPointer<AnnotatedGLShader> shader(
                m_shaderCache.compileFile(type, dependencyPath));
        success &= program->addSharedShader(
                qSharedPointerCast<QGLShader>(shader));
    }

    success &= program->link();

    return program;
}

