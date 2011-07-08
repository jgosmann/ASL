#ifndef SHADERPARAMETERCONTROLHANDLE_H
#define SHADERPARAMETERCONTROLHANDLE_H

#include <QWidget>

namespace gui {

  /**
   *  Interface. FIXME Needful anymore?
   */
  class ShaderParameterControlHandle
  {
  public:
    virtual ~ShaderParameterControlHandle() {}

    /**
     * This function must be reimplemented by a ShaderParameterControl.
     */
    /*template<class ParamT> virtual void setParameterFromControls(ParamT* values) = 0;*/
  };
  
} // namespace gui

#endif /* SHADERPARAMETERCONTROLHANDLE_H */
