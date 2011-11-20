#include "shaderparameterbundle.h"

using namespace gui;

void ShaderParameterBundle::append(QSharedPointer< Shader > shaderProgram,
    QList< QSharedPointer< ShaderParameterControlHandle > > 
        &shaderParameterControls )
{
  m_shaderPrograms.append( shaderProgram );
  m_shaderParameterControls.append( shaderParameterControls );
}


QSharedPointer< Shader > ShaderParameterBundle::getShaderProgram( unsigned short int index )
{
  return m_shaderPrograms.value( index );
}


QList< QSharedPointer< ShaderParameterControlHandle > > 
    ShaderParameterBundle::getShaderParameterControls( 
        unsigned short int index )
{
  return m_shaderParameterControls.value( index );
}
