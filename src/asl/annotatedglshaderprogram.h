#ifndef ANNOTATEDGLSHADERPROGRAM_H
#define ANNOTATEDGLSHADERPROGRAM_H

#include <QGLShaderProgram>
#include <QList>
#include <QSharedPointer>
#include <QStringList>
#include <QGenericMatrix>

#include "annotated.h"
#include "shaderinfo.h"

namespace asl
{
class AnnotatedGLShaderProgram : public QGLShaderProgram, public Annotated
{

public:
    AnnotatedGLShaderProgram(const ShaderInfo &shaderInfo)
            : QGLShaderProgram(static_cast<QObject *>(NULL)),
            Annotated(shaderInfo) { }

    bool addSharedShader(QSharedPointer<QGLShader> shader);

    void setUniformValue(const char* name, const QGenericMatrix<1,2,GLint> &mat, 
        int tupelSize); // ivec2
    void setUniformValue(const char* name, const QGenericMatrix<1,2,GLuint> &mat, 
        int tupelSize); // uivec2
    void setUniformValue(const char* name, const QGenericMatrix<1,2,GLfloat> &mat, 
        int tupelSize); // vec2

    void setUniformValue(const char* name, const QGenericMatrix<1,3,GLint> &mat, 
        int tupelSize); // ivec3
    void setUniformValue(const char* name, const QGenericMatrix<1,3,GLuint> &mat, 
        int tupelSize); // uivec3
    void setUniformValue(const char* name, const QGenericMatrix<1,3,GLfloat> &mat, 
        int tupelSize); // vec3

    void setUniformValue(const char* name, const QGenericMatrix<1,4,GLint> &mat, 
        int tupelSize); // ivec4
    void setUniformValue(const char* name, const QGenericMatrix<1,4,GLuint> &mat, 
        int tupelSize); // uivec4
    void setUniformValue(const char* name, const QGenericMatrix<1,4,GLfloat> &mat, 
        int tupelSize); // vec4

    void setUniformValue(const char* name, const QGenericMatrix<2,2,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<2,2,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<2,2,GLfloat> &mat, 
        int tupelSize);

    void setUniformValue(const char* name, const QGenericMatrix<2,3,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<2,3,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<2,3,GLfloat> &mat, 
        int tupelSize);

    void setUniformValue(const char* name, const QGenericMatrix<2,4,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<2,4,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<2,4,GLfloat> &mat, 
        int tupelSize);

    void setUniformValue(const char* name, const QGenericMatrix<3,2,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<3,2,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<3,2,GLfloat> &mat, 
        int tupelSize);

    void setUniformValue(const char* name, const QGenericMatrix<3,3,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<3,3,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<3,3,GLfloat> &mat, 
        int tupelSize);

    void setUniformValue(const char* name, const QGenericMatrix<3,4,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<3,4,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<3,4,GLfloat> &mat, 
        int tupelSize);

    void setUniformValue(const char* name, const QGenericMatrix<4,2,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<4,2,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<4,2,GLfloat> &mat, 
        int tupelSize);

    void setUniformValue(const char* name, const QGenericMatrix<4,3,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<4,3,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<4,3,GLfloat> &mat, 
        int tupelSize);

    void setUniformValue(const char* name, const QGenericMatrix<4,4,GLint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<4,4,GLuint> &mat, 
        int tupelSize);
    void setUniformValue(const char* name, const QGenericMatrix<4,4,GLfloat> &mat, 
        int tupelSize);


private:
    QList<QSharedPointer<QGLShader> > m_shadersInUse;

};
}

#endif // ANNOTATEDGLSHADERPROGRAM_H
