ColorParameterControl::ColorParameterControl(asl::ShaderParameterInfo &info, 
    QObject *listener)
  : m_info( info )
{
  QLabel *caption = new QLabel( info.name );
  QPushButton *pushButton = new QPushButton( "Select color ..." );

  QVBoxLayout *vBoxLayout = new QVBoxLayout();
  vBoxLayout->addWidget( caption );
  vBoxLayout->addWidget( pushButton );
  m_widget->setLayout( vBoxLayout );

  QObject::connect( pushButton, SIGNAL( pressed(void) ),
      this, SLOT( showColorDialog(void) ) );

  QObject::connect( this, SIGNAL( colorChanged(void) ),
      listener, SLOT( update(void) ) );
}


ColorParameterControl::~ColorParameterControl()
{
  delete [] m_colorArray;
}


void ColorParameterControl::showColorDialog()
{
  QColor color;

  const GLfloat *defaultColorArray = m_info.defaultValue.asFloat();

  QColor defaultColor = QColor();

  if( m_info.type->rowDimensionality() == 3 ) // vec3
  {
    defaultColor.setRgbF( defaultColorArray[0], defaultColorArray[1], 
        defaultColorArray[2] );

    color = QColorDialog::getColor( defaultColor, m_widget, 
        "Please select a color ..." );

    m_colorArray = new GLfloat[3];
    m_colorArray[0] = color.redF(); 
    m_colorArray[1] = color.greenF(); 
    m_colorArray[2] = color.blueF(); 
  }
  else // vec4
  {
    defaultColor.setRgbF( defaultColorArray[0], defaultColorArray[1], 
        defaultColorArray[2], defaultColorArray[3] );

    color = QColorDialog::getColor( defaultColor, m_widget, 
        "Please select a color ...",
        QColorDialog::ShowAlphaChannel );

    m_colorArray = new GLfloat[4];
    m_colorArray [0] = color.redF();
    m_colorArray [1] = color.greenF();
    m_colorArray [2] = color.blueF();
    m_colorArray [3] = color.alphaF();
  }

  emit colorChanged();
}


void ColorParameterControl::setParameterFromControls(
    QSharedPointer< Shader > shaderProgram )
{
  shaderProgram->setUniformValueArray( m_info.identifier, m_colorArray, 1, 3 );
}
