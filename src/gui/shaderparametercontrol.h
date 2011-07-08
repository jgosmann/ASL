#ifndef SHADERPARAMETERCONTROL_H
#define SHADERPARAMETERCONTROL_H

#include <QObject>
#include <QSharedPointer>
#include <QGLShaderProgram>
#include <QGridLayout>

#include "../asl/shaderparameterinfo.h"
#include "../asl/gltypeinfo.h"

#include "shaderparametercontrolhandle.h"
#include "widgetwrapper.h"

namespace gui
{

  class ShaderParameterControl : public ShaderParameterControlHandle, QObject
  {
    Q_OBJECT

  public:
      ShaderParameterControl(const asl::ShaderParameterInfo &info,
          QSharedPointer<QGLShaderProgram> shaderProgram);

      virtual ~ShaderParameterControl();

      /**
       * Returns the WidgetWrapper's widget. It can be either a singleField,
       * f.e. a QSpinBox, a multiField, f.e. a vector or matrix of 
       * QDoubleSpinBoxes or a preset-widget, f.e. a QButton that will open
       * a QColorDialog.
       */
      QWidget &widget() { return m_control->widget(); }

  public slots:
      /**
       * When the widget of the control has changed it will call this update
       * slot to force the whole control to update its related uniform.
       */
      template<class ParamT>
      void setParameterFromControls(ParamT *values);

  private:
      /**
       * This is a placeholder for the parameters widget.
       */
      WidgetWrapper *m_control;

      /** All important information for the related shader program. */
      asl::ShaderParameterInfo m_info;

      /** 
       * The instance of the related shader program. This class will run 
       * setUniformValue[Array]() on that.
       */
      QSharedPointer<QGLShaderProgram> m_shaderProgram; 

      unsigned short int m_rows, m_cols;
  };

  template<class ParamT>
  void ShaderParameterControl::setParameterFromControls(ParamT *values)
  {
    if( sizeof(values) == sizeof(ParamT*) ) // type singleValue
    {
      m_shaderProgram->setUniformValue( m_info.identifier, values );
    }
    else  // type multiValue
    {
      m_shaderProgram->setUniformValueArray( m_info.identifier, values, m_rows, 
          m_cols );
    }
  }

} // namespace gui

#endif /* SHADERPARAMETERCONTROL_H */
