#ifndef SHADERPARAMETERCONTROLHANDLE_H
#define SHADERPARAMETERCONTROLHANDLE_H

#include <QWidget>

namespace gui {

  typedef class asl::AnnotatedGLShaderProgram Shader;

  class ShaderParameterControlHandle
  {
  public:
    virtual ~ShaderParameterControlHandle() {}

    /**
     * This getter puts all controls widgets into one grid-layout that will be 
     * returned in a QWidget.
     */
    virtual QWidget* widget() = 0;

    /**
     * This function must be reimplemented by a ShaderParameterControl.
     */
    virtual void setParameterFromControls(QSharedPointer< Shader > shaderProgram)=0;
  };
  
} // namespace gui

#endif /* SHADERPARAMETERCONTROLHANDLE_H */
