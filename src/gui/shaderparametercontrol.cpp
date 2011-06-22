#include "shaderparametercontrol.h"

namespace gui {

  ShaderParameterControl::ShaderParameterControl(ShaderParameterInfo &info, 
      QGLShaderProgram &shaderProgram)
  {
    m_controls = malloc(sizeof(ControlT) * elems);
  }
}
