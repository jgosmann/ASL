
template<class ControlT, class ParamT>
ShaderParameterControl<ControlT, ParamT>::ShaderParameterControl(
  const asl::ShaderParameterInfo &info, 
  QSharedPointer<QGLShaderProgram> shaderProgram)
  : m_info(info),
    m_shaderProgram(shaderProgram)
{
  unsigned short int i, j;
  m_rows = m_info.type->rowDimensionality();
  m_cols = m_info.type->columnDimensionality();

  QGridLayout *gridLayout = new QGridLayout(&m_widget);

  m_controls = static_cast< ControlT** >(malloc( 
      sizeof(ControlT*)*m_rows*m_cols ));
  for(i=0; i<m_rows; i++) {

    for(j=0; j<m_cols; j++) {

      ControlT *control = new ControlT(&m_widget);
      // set Minimum and Maximum value dependent to ParamT
      control->setRange(*m_info.minimum.as<ParamT>(), 
          *m_info.maximum.as<ParamT>());

      // set initial value
      control->setValue( static_cast<ParamT>(0) );

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

      // add control - layout will resize automatically
      gridLayout->addWidget(control, i, j);

      // create row major array
      m_controls[i + j*m_cols] = control;
    }
  }

  m_widget.setLayout(gridLayout);
}

template<class ControlT, class ParamT>
ShaderParameterControl<ControlT, ParamT>::~ShaderParameterControl()
{
  free(m_controls);
}
