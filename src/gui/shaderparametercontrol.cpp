#include "shaderparametercontrol.h"

namespace gui {

  template<class ControlT, class ParamT>
  ShaderParameterControl<ControlT, ParamT>::ShaderParameterControl(QWidget *parent, 
    const asl::ShaderParameterInfo &info, 
    QSharedPointer<QGLShaderProgram> shaderProgram)
    : m_widget(parent),
      m_info(info),
      m_shaderProgram(shaderProgram)
  {
    unsigned short int i, j;
    m_rows = m_info.type->rowDimensionality();
    m_cols = m_info.type->columnDimensionality();

    QGridLayout *gridLayout = new QGridLayout(this);

    m_controls = static_cast< ControlT* >(malloc( 
        sizeof(ControlT)*m_rows*m_cols ));
    for(i=0; i<m_rows; i++) {

      for(j=0; j<m_cols; j++) {

        ControlT control = new ControlT(this);
        // set Minimum and Maximum value dependent to ParamT
        control.setRange(std::numeric_limits<ParamT>::min(), 
            std::numeric_limits<ParamT>::min());
        // set initial value
        control.setValue( static_cast<ParamT>(0) );

        // connect signal and slot
        connect(control, SIGNAL( valueChanged(ParamT) ),
            this, SLOT( setParameterFromControls(ParamT) ));

        // add control - layout will resize automatically
        gridLayout->addWidget(&control, i, j);

        // create row major array
        m_controls[i + j*m_cols] = control;
      }
    }

    m_widget->setLayout(gridLayout);
  }

  template<class ControlT, class ParamT>
  ShaderParameterControl<ControlT, ParamT>::~ShaderParameterControl()
  {
    free(m_controls);
  }

  template<class ControlT, class ParamT>
  void ShaderParameterControl<ControlT, ParamT>::setParameterFromControls(ParamT value)
  {
    ParamT *values = static_cast< ParamT* >(malloc( 
        sizeof(ParamT)*m_rows*m_cols ));

    unsigned short int i;
    for(i=0; i<(m_rows*m_cols); i++) 
      values[i] = m_controls[i].value();
    
    m_shaderProgram->setUniformValueArray( qPrintable( m_info.identifier ),
        values, m_rows*m_cols, 1);

    free(values);
  }

} // namespace gui
