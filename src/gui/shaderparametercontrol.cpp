
template<class ControlT, class ParamT>
ShaderParameterControl<ControlT, ParamT>::ShaderParameterControl(
  const asl::ShaderParameterInfo &info, 
  QSharedPointer<QGLShaderProgram> shaderProgram)
  : ShaderParameterControlHandle(),
    m_info(info),
    m_shaderProgram(shaderProgram)
{
  unsigned short int i, j;

  m_rows = m_info.type->rowDimensionality();
  m_cols = m_info.type->columnDimensionality();

  QGridLayout *gridLayout = new QGridLayout(&m_widget);
  m_widget.setLayout(gridLayout); // necessary?

  m_controls = new ControlT*[m_rows*m_cols];

  for(j=0; j<m_cols; i++) 
  { 
    for(i=0; i<m_rows; i++)
    {
      ControlT *control = new ControlT(&m_widget);

      // set Minimum and Maximum value dependent to ParamT
      control->setRange(*m_info.minimum.as<ParamT>(), 
          *m_info.maximum.as<ParamT>());

      // set initial value
      control->setValue( m_info.defaultValue.as<ParamT>() );

      // add control - layout will resize automatically
      gridLayout->addWidget(control, i, j);

      // create column major array
      m_controls[i + j*m_cols] = control;

      // connect signal and slot
      // FIXME: this won't work as this class is not derived from QObject,
      // because template classes cannot derive from QObject.
      // Solution ideas:
      // 1) Write an inner class derived from QObject implementing the
      // setParameterFromControlsFunction().
      // 2) Do actively set the parameters on each change. Instead introduce
      // some kind of callback from the rendering method calling this function
      // and let the change of the parameters trigger a "rerendering"/redraw.
      // The triggering of the redraw has to be done anyways. So this might be
      // the best solution. However, it remains the question in which way to
      // implement the callback.
      //QObject::connect(control, SIGNAL( valueChanged(ParamT) ),
          //this, SLOT( setParameterFromControls(ParamT) ));
    }
  }

  // initialize uniforms
  setParametersFromControls();
}

template<class ControlT, class ParamT>
ShaderParameterControl<ControlT, ParamT>::~ShaderParameterControl()
{
  delete m_controls;
}

template<class ControlT, class ParamT>
void ShaderParameterControl<ControlT, ParamT>::setParameterFromControls()
{
  unsigned short int length = m_rows*m_cols;

  if( length > 1 )
  {
    ParamT multiValue[] = new ParamT[length];

    for(int i=0; i<length; i++)
      values[i] = m_controls[i]->value();

    m_shaderProgram->setUniformValueArray( m_info.identifier, multiValue, 
        m_cols, m_rows);
  }
  else
  {
    ParamT singleValue = m_controls[0]->value();

    m_shaderProgram->setUniformValue( m_info.identifier, singleValue);
  }
}
