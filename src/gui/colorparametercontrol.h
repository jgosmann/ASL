#ifndef COLORPARAMETERCONTROL_H
#define COLORPARAMETERCONTROL_H

#include <QWidget>
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
  class ColorParameterControl : public ShaderParameterControlHandle
  {
  
  public:
    ColorParameterControl(QObject *listener);

    void setParameterFromControls(QSharedPointer< Shader > shaderProgram);

  private:
    WidgetWrapper<ControlT, ParamT> **m_controls;

    asl::ShaderParameterInfo m_info;
  };

} // namespace gui

#endif /* COLORPARAMETERCONTROL_H */
