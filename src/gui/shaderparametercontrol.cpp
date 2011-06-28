#include "shaderparametercontrol.h"

namespace gui {

  ShaderParameterControl::ShaderParameterControl(QWidget *parent, 
      ShaderParameterInfo &info, QGLShaderProgram &shaderProgram)
    : QWidget(parent),
      m_shaderProgram(shaderProgram)
  {
    m_typeInfo = *info.type();

    unsigned short int i, j;
    m_rows = m_typeInfo.rowDimensionality();
    m_cols = m_typeInfo.columnDimensionality();

    QGridLayout gridLayout = QGridLayout(this);

    m_controls = static_cast< ControlT* > malloc( 
        sizeof(ControlT)*m_rows*m_cols );
    for(i=0; i<m_rows; i++) {

      for(j=0; j<m_cols; j++) {

        ControlT control = new ControlT(this);
        // set Minimum and Maximum value dependent to ParamT
        control.setRange(numeric_limits<ParamType>::min(), 
            numeric_limits<ParamType>::min());
        // set initial value
        control.setValue( static_cast<ParamT>(0) );

        // connect signal and slot
        connect(control, SIGNAL( valueChanged(ParamT) ),
            this, SLOT( setParameterFromControls(ParamT) ));

        // add control - layout will resize automatically
        gridLayout.addWidget(&control, i, j);

        // create row major array
        m_controls[i + j*m_cols] = control;
      }
    }

    setLayout(gridLayout);
  }

  ShaderParameterControl::~ShaderParameterControl()
  {
    free(m_controls);
  }

  ShaderParameterControl::setParameterFromControls(ParamT value)
  {
    ParamT *values = static_cast< ParamtT* > malloc( 
        sizeof(ParamT)*m_rows*m_cols );

    unsigned short int i;
    for(i=0; i<(m_rows*m_cols); i++) 
      values[i + j*m_cols] = m_controls[i + j*m_cols].value();
    
    m_shaderProgram.setUniformValueArray( qPrintable( m_typeInfo.identifier ),
        values, n_rows*n_cols, 1);

    free(values);
  }

} // namespace gui
