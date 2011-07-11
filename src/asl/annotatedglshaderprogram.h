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

      SET_UNIFORM_VALUE(2,1,GLint)
      SET_UNIFORM_VALUE(2,1,GLuint)
      SET_UNIFORM_VALUE(2,1,GLfloat)
      SET_UNIFORM_VALUE(3,1,GLint)
      SET_UNIFORM_VALUE(3,1,GLuint)
      SET_UNIFORM_VALUE(3,1,GLfloat)
      SET_UNIFORM_VALUE(4,1,GLint)
      SET_UNIFORM_VALUE(4,1,GLuint)
      SET_UNIFORM_VALUE(4,1,GLfloat)
      SET_UNIFORM_VALUE(2,2,GLint)
      SET_UNIFORM_VALUE(2,3,GLuint)
      SET_UNIFORM_VALUE(2,4,GLfloat)
      SET_UNIFORM_VALUE(3,2,GLint)
      SET_UNIFORM_VALUE(3,3,GLuint)
      SET_UNIFORM_VALUE(3,4,GLfloat)
      SET_UNIFORM_VALUE(4,2,GLint)
      SET_UNIFORM_VALUE(4,3,GLuint)
      SET_UNIFORM_VALUE(4,4,GLfloat)


  private:
      QList<QSharedPointer<QGLShader> > m_shadersInUse;

    #define SET_UNIFORM_VALUE(m,n,type) \
      void setUniformValue(const char* name, const QGenericMatrix<n,m,type> &mat); \

  };
}

#endif // ANNOTATEDGLSHADERPROGRAM_H
