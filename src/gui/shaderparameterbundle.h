#ifndef SHADERPARAMETERBUNDLE_H
#define SHADERPARAMETERBUNDLE_H

#include <QList>

#include "../asl/annotatedglshaderprogram.h"

#include "shaderparametercontrol.h"

namespace gui
{

  typedef class asl::AnnotatedGLShaderProgram Shader;

  class ShaderParameterBundle
  {
  
  public:
    ShaderParameterBundle() {}
    ~ShaderParameterBundle() {}

    void append(QSharedPointer< Shader > shaderProgram, 
        QList< QSharedPointer< ShaderParameterControlHandle > > 
            &shaderParameterControls );

    QSharedPointer< Shader > getShaderProgram( unsigned short int index );

    QList< QSharedPointer< ShaderParameterControlHandle > > 
        getShaderParameterControls( unsigned short int index );

  private:
    QList< QSharedPointer< Shader > > m_shaderPrograms;

    QList< QList< QSharedPointer< ShaderParameterControlHandle > > > 
        m_shaderParameterControls;

  };

} // namespace gui

#endif /* SHADERPARAMETERBUNDLE_H */
