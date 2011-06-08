#include "annotatedglshaderprogram.h"

using namespace asl;

AnnotatedGLShaderProgram::AnnotatedGLShaderProgram(const ShaderInfo &shaderInfo)
    : QGLShaderProgram(static_cast<QObject *>(NULL)), m_shaderInfo(shaderInfo)
{
}
