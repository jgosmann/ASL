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
              Annotated(shaderInfo) {
          m_parameters.append(shaderInfo.parameters);
      }

      bool addSharedShader(QSharedPointer<QGLShader> shader);

      inline void addParameters(const QList<asl::ShaderParameterInfo>
              &parameters) {
          m_parameters.append(parameters);
      }

      inline const QList<asl::ShaderParameterInfo> & parameters() const {
          return m_parameters;
      }

  private:
      QList<QSharedPointer<QGLShader> > m_shadersInUse;
      QList<asl::ShaderParameterInfo> m_parameters;

  };
}

#endif // ANNOTATEDGLSHADERPROGRAM_H
