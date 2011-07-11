#ifndef SHADERPARAMETERCONTROL_H
#define SHADERPARAMETERCONTROL_H

#include <stdexcept>

#include <QGenericMatrix>
#include <QObject>
#include <QSharedPointer>
#include <QWidget>
#include <QGridLayout>

#include "../asl/annotatedglshaderprogram.h"
#include "../asl/shaderparameterinfo.h"
#include "../asl/gltypeinfo.h"

#include "shaderparametercontrolhandle.h"


namespace gui
{
  typedef class asl::AnnotatedGLShaderProgram Shader;

  template<class ControlT, class ParamT>
  class ShaderParameterControl : public ShaderParameterControlHandle
  {
  
  public:
    ShaderParameterControl(asl::ShaderParameterInfo &info, QObject *listener);
    ~ShaderParameterControl();

    QWidget* widget();

    void setParameterFromControls(QSharedPointer< Shader > shaderProgram);

  private:
    ControlT **m_controls;

    unsigned short int m_rows, m_cols;

    asl::ShaderParameterInfo m_info;
  };

  #define ifMatchesMatDimSetUniform(rows, cols) \
  if (m_rows == (rows) && m_cols == (cols)) { \
    qreal valuesAsFloat[(rows) * (cols)]; \
    for (unsigned int i = 0; i < (rows) * (cols); ++i) { \
      valuesAsFloat[i] = values[i]; \
    } \
    \
    QMatrix##cols##x##rows mat(valuesAsFloat); \
    shaderProgram->setUniformValue(qPrintable(m_info.identifier), mat); \
  }

  #include "shaderparametercontrol.cpp"

} // namespace gui

#endif /* SHADERPARAMETERCONTROL_H */
