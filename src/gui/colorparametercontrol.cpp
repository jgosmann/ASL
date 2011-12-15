#include "colorparametercontrol.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include <QVector3D>

using namespace gui;

ColorParameterControl::ColorParameterControl(asl::ShaderParameterInfo &info, 
    QObject *listener)
  : m_info( info )
{
  const GLfloat *defaultColor = m_info.defaultValue.asFloat();
  m_colorArray = new GLfloat[m_info.type->rowDimensionality()];
  for (unsigned int i = 0; i < m_info.type->rowDimensionality(); ++i) {
      m_colorArray[i] = defaultColor[i];
  }

  QLabel *caption = new QLabel( info.name );
  QPushButton *pushButton = new QPushButton( "Select color ..." );

  QVBoxLayout *vBoxLayout = new QVBoxLayout();
  vBoxLayout->addWidget( caption );
  vBoxLayout->addWidget( pushButton );
  m_widget.setLayout( vBoxLayout );

  QObject::connect( pushButton, SIGNAL( pressed(void) ),
      this, SLOT( showColorDialog(void) ) );
  QObject::connect( this, SIGNAL( valueChanged(void) ),
      listener, SLOT( renderImage(void) ) );
}


ColorParameterControl::~ColorParameterControl()
{
  delete [] m_colorArray;
}


void ColorParameterControl::showColorDialog()
{
  QColor color;
  QColor defaultColor = QColor();

  if( m_info.type->rowDimensionality() == 3 ) // vec3
  {
    defaultColor.setRgbF( m_colorArray[0], m_colorArray[1], 
        m_colorArray[2] );

    color = QColorDialog::getColor( defaultColor, &m_widget, 
        "Please select a color ..." );

  }
  else // vec4
  {
    defaultColor.setRgbF( m_colorArray[0], m_colorArray[1], 
        m_colorArray[2], m_colorArray[3] );

    color = QColorDialog::getColor( defaultColor, &m_widget, 
        "Please select a color ...",
        QColorDialog::ShowAlphaChannel );

    m_colorArray [3] = color.alphaF();
  }

  m_colorArray [0] = color.redF();
  m_colorArray [1] = color.greenF();
  m_colorArray [2] = color.blueF();

  if (color != defaultColor) {
    emit valueChanged();
  }
}


void ColorParameterControl::setParameterFromControls(
    QSharedPointer< Shader > shaderProgram )
{
  if( m_info.type->rowDimensionality() == 3 )
  {
    shaderProgram->setUniformValueArray( qPrintable( m_info.identifier ), 
        new QVector3D( m_colorArray[0], m_colorArray[1], m_colorArray[2] ), 1 );
  }
  else
  {
    shaderProgram->setUniformValueArray( qPrintable( m_info.identifier ), 
        new QVector4D( m_colorArray[0], m_colorArray[1], m_colorArray[2],
          m_colorArray[3] ), 1 );
  }
}
