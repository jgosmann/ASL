
#include "annotatedglshaderprogramcompiler.h"

using namespace asl;

asl::AnnotatedGLShaderProgram * AnnotatedGLShaderProgramCompiler::compileFile(
        QGLShader::ShaderType type, const QString &filename)
{
    m_success = true;
    m_shaderType = type;
    m_addedShaders.clear();

    QSharedPointer<AnnotatedGLShader> mainShader(
            m_shaderCache.compileFile(m_shaderType, filename));
    m_log += m_shaderCache.log();
    m_success &= m_shaderCache.success();
    m_programUnderConstruction = new AnnotatedGLShaderProgram(
            mainShader->shaderInfo());

    m_success &= m_programUnderConstruction->addSharedShader(
            qSharedPointerCast<QGLShader>(mainShader));
    m_addedShaders.insert(filename);

    compileAndAddDependencies(mainShader->dependencies(), filename);

    m_success &= m_programUnderConstruction->link();
    m_log += m_programUnderConstruction->log();

    return m_programUnderConstruction;
}

void AnnotatedGLShaderProgramCompiler::compileAndAddShader(
        const QString &filename)
{
    if (m_addedShaders.contains(filename)) {
        return;
    }

    QSharedPointer<AnnotatedGLShader> shader(
            m_shaderCache.compileFile(m_shaderType, filename));
    m_log += m_shaderCache.log();
    m_success &= m_shaderCache.success();
    m_success &= m_programUnderConstruction->addSharedShader(
            qSharedPointerCast<QGLShader>(shader));
    m_addedShaders.insert(filename);
    compileAndAddDependencies(shader->dependencies(), filename);
}

void AnnotatedGLShaderProgramCompiler::compileAndAddDependencies(
        const QStringList &dependencies, const QString &includingFile)
{
    foreach (QString dependency, dependencies) {
        const QString dependencyPath(
                m_dependencyLocator.locate(dependency, includingFile));
        compileAndAddShader(dependencyPath);
    }
}

