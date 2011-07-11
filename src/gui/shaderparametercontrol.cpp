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
    m_controls[ i ] = new ControlT();

    m_controls[ i ]->setValue( info.defaultValue.as<ParamT>() );
    m_controls[ i ]->setRange( info.minimum.as<ParamT>(), info.maximum.as<ParamT>() );

    QObject::connect( m_controls[ i ], SIGNAL( valueChanged(void) ),
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
  ParamT values[] = new ParamT[ m_rows*m_cols ];

  unsigned short int i;
  for(i = 0; i < (m_rows*m_cols); i++)
  {
    values[ i ] = m_controls[ i++ ]->value();
  }

  // FIXME Here we have to use the standard OpenGL uniform-setter
  shaderProgram->setUniformValueArray( m_info.identifier, values, 
      m_rows*m_cols);
}

// FIXME Why can't I specify the template function for GLfloat???
//template<class ControlT>
//void ShaderParameterControl<ControlT, GLfloat>::setParameterFromControls(
//    QSharedPointer< Shader > shaderProgram )
//{
//  GLfloat values[] = new GLfloat[ m_rows*m_cols ];
//
//  unsigned short int i = 0;
//  foreach( WidgetWrapper *widget, m_controls )
//  {
//    values[ i++ ] = widget->value();
//  }
//
//  /*
//   * The uniform-setter for the type GLfloat is a special case that needs both
//   * dimensions.
//   */
//  shaderProgram->setUniformValueArray( m_info.identifier, values, m_cols,
//      m_rows);
//}
