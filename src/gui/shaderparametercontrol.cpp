#include <QGenericMatrix>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
  
template<class ControlT, class ParamT>
ShaderParameterControl<ControlT, ParamT>::ShaderParameterControl(asl::ShaderParameterInfo &info,
    QObject *listener)
  : m_rows( info.type->rowDimensionality() ),
    m_cols( info.type->columnDimensionality() ),
    m_info( info )
{
  m_controls = new ControlT*[ m_rows*m_cols ];

  unsigned short int i;
  for( i = 0; i < (m_rows*m_cols); i++)
  {
    m_controls[i] = new ControlT();

    m_controls[i]->setValue( info.defaultValue.as<ParamT>()[i] );
    m_controls[i]->setRange( info.minimum.as<ParamT>()[i], 
        info.maximum.as<ParamT>()[i] );

    QObject::connect( m_controls[i], SIGNAL( valueChanged(void) ),
        listener, SLOT( renderImage(void) ) );
  }
}

template<class ControlT, class ParamT>
ShaderParameterControl<ControlT, ParamT>::~ShaderParameterControl()
{
  delete [] m_controls;
  m_controls = 0;
}

/*
 * This getter gathers all single control-widgets and puts them compliant
 * to a column-major array into a QGridLayout.
 */
template<class ControlT, class ParamT>
QWidget* ShaderParameterControl<ControlT, ParamT>::widget()
{
  QWidget *widget = new QWidget();
  QGridLayout *gridLayout = new QGridLayout();

  unsigned short int i, j;
  for(i = 0; i < m_rows; i++)
  {
    for(j = 0; j < m_cols; j++)
    {
      gridLayout->addWidget( m_controls[ i*m_rows + j ], i, j , 
          Qt::AlignCenter);
    }
  }

  widget->setLayout( gridLayout );

  return widget;
}


template<class ControlT, class ParamT>
void ShaderParameterControl<ControlT, ParamT>::setParameterFromControls(
    QSharedPointer< Shader > shaderProgram )
{
  ParamT *values = new ParamT[ m_rows*m_cols ];

  unsigned short int i;
  for(i = 0; i < (m_rows*m_cols); i++)
  {
    values[ i ] = m_controls[ i ]->value();
  }

  // FIXME: change to glUniform[1,2,3,4][i,f][v](..)
  if(m_rows == 1)
  {
    shaderProgram->setUniformValue(qPrintable(m_info.identifier), values[0]);
  }
  else if(m_cols == 1 && m_rows == 2)
  {
    qreal valuesAsFloat[m_rows];
    for(unsigned int i = 0; i < m_rows; ++i)
    {
      valuesAsFloat[i] = values[i];
    }

    QVector2D vec2(valuesAsFloat[0], valuesAsFloat[1]);
    shaderProgram->setUniformValue(qPrintable(m_info.identifier), vec2);
  }
  else if(m_cols == 1 && m_rows == 3)
  {
    qreal valuesAsFloat[m_rows];
    for(unsigned int i = 0; i < m_rows; ++i)
    {
      valuesAsFloat[i] = values[i];
    }

    QVector3D vec3(valuesAsFloat[0], valuesAsFloat[1], valuesAsFloat[2]);
    shaderProgram->setUniformValue(qPrintable(m_info.identifier), vec3);
  }
  else if(m_cols == 1 && m_rows == 4)
  {
    qreal valuesAsFloat[m_rows];
    for(unsigned int i = 0; i < m_rows; ++i)
    {
      valuesAsFloat[i] = values[i];
    }

    QVector4D vec4(valuesAsFloat[0], valuesAsFloat[1], valuesAsFloat[2],
        valuesAsFloat[3]);
    shaderProgram->setUniformValue(qPrintable(m_info.identifier), vec4);
  }
  // FIXME: QGenericMatrix is row-major, isn't OpenGL column-major
  // FIXME: This case shall also handle vector-types, so look for conversion
  //        in annotatedglshaderprogram.*!
  else ifMatchesMatDimSetUniform(2,2)
  else ifMatchesMatDimSetUniform(2,3)
  else ifMatchesMatDimSetUniform(2,4)
  else ifMatchesMatDimSetUniform(3,2)
  else ifMatchesMatDimSetUniform(3,3)
  else ifMatchesMatDimSetUniform(3,4)
  else ifMatchesMatDimSetUniform(4,2)
  else ifMatchesMatDimSetUniform(4,3)
  else ifMatchesMatDimSetUniform(4,4)
  else
  {
    throw std::logic_error("This uniform-type is not supported yet!");
  }
}
