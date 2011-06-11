
#include "annotatedglshaderprogramcompiler.h"

#include <QStringBuilder>

using namespace asl;

asl::AnnotatedGLShaderProgram * AnnotatedGLShaderProgramCompiler::compileFile(
        QGLShader::ShaderType type, const QString &filename)
{
    m_success = true;
    m_shaderType = type;
    m_addedShaders.clear();

    m_compiler.prefixSourcesWith("#define ASL_MAIN\n#line 0\n");
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

    unsigned int sourceStringNo = m_addedShaders.size();
    m_log = m_log % "INFO: " % QString::number(sourceStringNo - 1) % ":0: "
        % "Compiling \"" % filename % "\" as source string number "
        % QString::number(sourceStringNo) % ".\n";

    m_compiler.prefixSourcesWith(
            "#line 0 " + QString::number(sourceStringNo) + "\n");
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

