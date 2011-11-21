
#include "aslprogramcompiler.h"

#include <QStringBuilder>

using namespace asl;

asl::AnnotatedGLShaderProgram * ASLProgramCompiler::compileFile(
        QGLShader::ShaderType type, const QString &filename)
{
    reset();
    m_shaderType = type;

    QSharedPointer<AnnotatedGLShader> mainShader(compileMainShader(filename));

    m_programUnderConstruction = new AnnotatedGLShaderProgram(
            mainShader->shaderInfo());

    addShaderWithoutParameters(filename, mainShader);
    compileAndAddDependencies(mainShader->dependencies(), filename);

    m_success &= m_programUnderConstruction->link();
    m_log += m_programUnderConstruction->log();

    return m_programUnderConstruction;
}

void ASLProgramCompiler::reset()
{
    m_success = true;
    m_log.clear();
    m_addedShaders.clear();
}

void ASLProgramCompiler::compileAndAddShaderAndLoadDependencies(
        const QString &filename)
{
    QSharedPointer<AnnotatedGLShader> shader(compileShader(filename));
    addShader(filename, shader);
    compileAndAddDependencies(shader->dependencies(), filename);
}

QSharedPointer<AnnotatedGLShader> ASLProgramCompiler::compileMainShader(
        const QString &filename)
{

    QSharedPointer<AnnotatedGLShader> shader(
            m_shaderCache.compileFileAsMain(m_shaderType, filename));
    m_log += m_shaderCache.log();
    m_success &= m_shaderCache.success();
    return shader;
}

QSharedPointer<AnnotatedGLShader> ASLProgramCompiler::compileShader(
        const QString &filename)
{
    QSharedPointer<AnnotatedGLShader> shader(
            m_shaderCache.compileFile(m_shaderType, filename));
    m_log += m_shaderCache.log();
    m_success &= m_shaderCache.success();
    return shader;
}

void ASLProgramCompiler::addShaderWithoutParameters(const QString &filename,
        QSharedPointer<AnnotatedGLShader> &shader)
{
    m_success &= m_programUnderConstruction->addSharedShader(
            qSharedPointerCast<QGLShader>(shader));
    m_addedShaders.insert(filename);
}

void ASLProgramCompiler::addShader(const QString &filename,
        QSharedPointer<AnnotatedGLShader> &shader)
{
    addShaderWithoutParameters(filename, shader);
    m_programUnderConstruction->addParameters(shader->parameters());
}

void ASLProgramCompiler::compileAndAddDependencies(
        const QStringList &dependencies, const QString &includingFile)
{
    foreach (QString dependency, dependencies) {
        const QString dependencyPath(
                m_dependencyLocator.locate(dependency, includingFile));

        if (m_addedShaders.contains(dependencyPath)) {
            continue;
        }

        m_log = m_log % "INFO: 0:0: Compiling \"" % dependency % "\"\n";

        compileAndAddShaderAndLoadDependencies(dependencyPath);
    }
}

