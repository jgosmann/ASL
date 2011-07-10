#ifndef SHADERPARAMETERCONTROL_H
#define SHADERPARAMETERCONTROL_H

// for use of Q_FOREACH-macro
#include <QtGlobal>

#include <QObject>
#include <QSharedPointer>

#include "../asl/annotatedglshaderprogram.h"
#include "../asl/shaderparameterinfo.h"
#include "../asl/gltypeinfo.h"

#include "shaderparametercontrolhandle.h"
#include "widgetwrapper.h"


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
    WidgetWrapper<ControlT, ParamT> **m_controls;

    unsigned short int m_rows, m_cols;

    asl::ShaderParameterInfo m_info;
  };

  #include "shaderparametercontrol.cpp"

} // namespace gui

#endif /* SHADERPARAMETERCONTROL_H */
