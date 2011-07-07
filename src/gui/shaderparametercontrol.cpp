#include "shaderparametercontrol.h"

using namespace gui;

ShaderParameterControl::ShaderParameterControl(
  const asl::ShaderParameterInfo &info, 
  QSharedPointer<QGLShaderProgram> shaderProgram)
  : ShaderParameterControlHandle(),
    m_info(info),
    m_shaderProgram(shaderProgram)
{
  // store the dimensionality for use in setParameterFromControls(..).
  m_rows = m_info.type->rowDimensionality();
  m_cols = m_info.type->columnDimensionality();

  m_control = new WidgetWrapper( m_info );
}

ShaderParameterControl::~ShaderParameterControl()
{
  delete m_control;
}
