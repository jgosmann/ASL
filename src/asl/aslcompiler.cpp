#include "aslcompiler.h"
#include "aslparser_internal.h"

#include <QScopedPointer>

using namespace asl;

ASLCompiler::ASLCompiler(DependencyReader &dependencyReader, QObject *parent)
        : QObject(parent), m_dependencyReader(dependencyReader)
{
}

AnnotatedGLShaderProgram * ASLCompiler::compile(QGLShader::ShaderType type,
         const QString &source, const QString &pathOfSource)
{
    reset();

    m_shaderInfo = parserinternal::parse(source, pathOfSource);
    m_log += parserinternal::log;

    parseDependencies(m_shaderInfo.dependencies, pathOfSource);

    QScopedPointer<AnnotatedGLShaderProgram> shaderPrgm(
            new AnnotatedGLShaderProgram(m_shaderInfo));
    m_success = shaderPrgm->addShaderFromSourceCode(type, source);
    foreach (QString dependency, m_shaderInfo.dependencies) {
        m_success &= shaderPrgm->addShaderFromSourceCode(type, 
                m_dependencyReader.readDependency(dependency, pathOfSource));
    }

    if (!shaderPrgm->log().isEmpty()) {
        m_log += shaderPrgm->log();
    }
    if (!m_success) {
        return NULL;
    }


    shaderPrgm->link();
    // TODO: check for errors

    return shaderPrgm.take();
}

void ASLCompiler::reset()
{
    parserinternal::clearLog();
    m_log.clear();
    m_success = true;
}

/* Do not pass dependencies as reference. Otherwise we might directly iterate
 * over m_shaderInfo which will be modified by this function.
 * This should be no big perfomance impact as QLists are implicitly shared.
 */
void ASLCompiler::parseDependencies(const QStringList dependencies,
        const QString &includingFile)
{
    foreach (QString dependency, dependencies) {
        QString path;
        //const QString path = m_dependencyReader.dependencyPath(dependency,
                //includingFile):
        const QString source = m_dependencyReader.readDependency(dependency,
                includingFile);
        ShaderInfo info = parserinternal::parse(source, path);
        m_log += parserinternal::log;

        m_shaderInfo.dependencies.append(info.dependencies);
        m_shaderInfo.parameters.append(info.parameters);

        parseDependencies(info.dependencies, path);
    }
}

