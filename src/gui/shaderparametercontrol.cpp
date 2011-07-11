#include <QGenericMatrix>
  
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
        listener, SLOT( update(void) ) );
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
  // FIXME: QGenericMatrix is row-major, isn't OpenGL column-major
  // FIXME: This case shall also handle vector-types, so look for conversion
  //        in annotatedglshaderprogram.*!
  ifMatchesMatDimSetUniform(2,2)
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
