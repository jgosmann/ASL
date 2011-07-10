#include "shaderparameterbundle.h"

using namespace gui;

ShaderParameterBundle::append(QSharedPointer< Shader > shaderProgram,
    QList< QSharedPointer< ShaderParameterControlHandle > > 
        &shaderParameterControls )
{
  shaderPrograms.append( shaderProgram );
  shaderParameterControls.append( shaderParameterControls );
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
