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
    virtual void setParametersFromControls() = 0;
  };
  
} // namespace gui

#endif /* SHADERPARAMETERCONTROLHANDLE_H */
