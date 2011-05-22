#include "annotatedglshaderprogram.h"

using namespace asl;

AnnotatedGLShaderProgram::AnnotatedGLShaderProgram(const QString &name,
            const QString &description,
            const QList<ShaderParameterInfo> &parameters) :
    QGLShaderProgram(static_cast<QObject *>(NULL)),
    m_name(name), m_description(description), m_parameters(parameters)
{
}
