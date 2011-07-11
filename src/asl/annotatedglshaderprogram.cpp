#include "annotatedglshaderprogram.h"

using namespace asl;

#define SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(m,n,type) \
  void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<n,m,type> &mat); \
  { \
    Q_UNUSED(name) \
    Q_UNUSED(mat) \
  } \


bool AnnotatedGLShaderProgram::addSharedShader(
        QSharedPointer<QGLShader> shader)
{
    if (!addShader(shader.data())) {
        return false;
    }

    m_shadersInUse.append(shader);
    return true;
}

SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(2,1,GLint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(2,1,GLuint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(2,1,GLfloat)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(3,1,GLint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(3,1,GLuint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(3,1,GLfloat)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(4,1,GLint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(4,1,GLuint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(4,1,GLfloat)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(2,2,GLint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(2,3,GLuint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(2,4,GLfloat)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(3,2,GLint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(3,3,GLuint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(3,4,GLfloat)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(4,2,GLint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(4,3,GLuint)
SET_UNIFORM_VALUE_STANDARD_IMPLEMENTATION(4,4,GLfloat)
