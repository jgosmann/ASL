
#include "annotatedglshaderprogram.h"

using namespace asl;

bool AnnotatedGLShaderProgram::addSharedShader(
        QSharedPointer<QGLShader> shader)
{
    if (!addShader(shader.data())) {
        return false;
    }

    m_shadersInUse.append(shader);
    return true;
}

