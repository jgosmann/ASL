
#include "aslprogramcompiler.h"

#include <QStringBuilder>

using namespace asl;

asl::AnnotatedGLShaderProgram * ASLProgramCompiler::compileFile(
        QGLShader::ShaderType type, const QString &filename)
{
    m_success = true;
    m_shaderType = type;
    m_addedShaders.clear();

    m_compiler.prefixSourcesWith("#define ASL_MAIN\n#line 0\n");
    QSharedPointer<AnnotatedGLShader> mainShader(compileShader(filename));

    m_programUnderConstruction = new AnnotatedGLShaderProgram(
            mainShader->shaderInfo());

    addShader(filename, mainShader);
    compileAndAddDependencies(mainShader->dependencies(), filename);

    m_success &= m_programUnderConstruction->link();
    m_log += m_programUnderConstruction->log();

    return m_programUnderConstruction;
}

void ASLProgramCompiler::compileAndAddShaderAndLoadDependencies(
        const QString &filename)
{
    QSharedPointer<AnnotatedGLShader> shader(compileShader(filename));
    addShader(filename, shader);
    compileAndAddDependencies(shader->dependencies(), filename);
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

void ASLProgramCompiler::addShader(const QString &filename,
        QSharedPointer<AnnotatedGLShader> &shader)
{
    m_success &= m_programUnderConstruction->addSharedShader(
            qSharedPointerCast<QGLShader>(shader));
    m_addedShaders.insert(filename);
}

void ASLProgramCompiler::compileAndAddDependencies(
        const QStringList &dependencies, const QString &includingFile)
{
    const unsigned int includingSourceStringNo = m_addedShaders.size() - 1;
    foreach (QString dependency, dependencies) {
        const QString dependencyPath(
                m_dependencyLocator.locate(dependency, includingFile));

        if (m_addedShaders.contains(dependencyPath)) {
            continue;
        }

        m_log = m_log % "INFO: " % QString::number(includingSourceStringNo)
            % ":0: Compiling \"" % dependency % "\" as source string number "
            % QString::number(m_addedShaders.size()) % ".\n";

        m_compiler.prefixSourcesWith(
            "#line 0 " + QString::number(m_addedShaders.size()) + "\n");
        compileAndAddShaderAndLoadDependencies(dependencyPath);
    }
}

