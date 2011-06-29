#ifndef SHADERPARAMETERCONTROLFACTORY_H
#define SHADERPARAMETERCONTROLFACTORY_H

namespace gui {

  class ShaderParameterControlFactory
  {
  public:
    static ShaderParameterControl<void, void> createControl();
  };

} // namespace gui

#endif // SHADERPARAMETERCONTROLFACTORY_H
