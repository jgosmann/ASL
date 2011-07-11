
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

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,2,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // ivec2
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,2,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // uivec2
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,2,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // vec2

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,3,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // ivec3
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,3,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // uivec3
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,3,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // vec3

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,4,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // ivec4
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,4,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // uivec4
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<1,4,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) } // vec4

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,2,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,2,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,2,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,3,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,3,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,3,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,4,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,4,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<2,4,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,2,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,2,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,2,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,3,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,3,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,3,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,4,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,4,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<3,4,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,2,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,2,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,2,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,3,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,3,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,3,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }

void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,4,GLint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,4,GLuint> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
void AnnotatedGLShaderProgram::setUniformValue(const char* name, const QGenericMatrix<4,4,GLfloat> &mat, 
    int tupelSize) { Q_UNUSED(name) Q_UNUSED(mat) Q_UNUSED(tupelSize) }
