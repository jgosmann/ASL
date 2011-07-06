#ifndef SHADERPARAMETERCONTROLHANDLE_H
#define SHADERPARAMETERCONTROLHANDLE_H

#include <QWidget>

namespace gui {

  /**
   *  Interface.
   */
  class ShaderParameterControlHandle
  {
  public:
    virtual ~ShaderParameterControlHandle() {}

    virtual QWidget& widget() = 0;
    virtual void setUniforms() = 0;
  };
  
} // namespace gui

#endif /* SHADERPARAMETERCONTROLHANDLE_H */
