#ifndef COLORPARAMETERCONTROL_H
#define COLORPARAMETERCONTROL_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QSharedPointer>

#include "../asl/annotatedglshaderprogram.h"
#include "../asl/shaderparameterinfo.h"
#include "../asl/gltypeinfo.h"

#include "shaderparametercontrolhandle.h"


namespace gui
{
  typedef class asl::AnnotatedGLShaderProgram Shader;

  /**
   * This class is a special subclass of ShaderParameterControlHandle beside
   * the default ShaderParameterControl that treats the case of a user-
   * preferred control "color" specified with ASL.
   */
  class ColorParameterControl : public QObject, public ShaderParameterControlHandle
  {
    Q_OBJECT
  
  public:
    ColorParameterControl(asl::ShaderParameterInfo &info, QObject *listener);
    ~ColorParameterControl();

    QWidget* widget()
    {
      return &m_widget;
    }

    void setParameterFromControls(QSharedPointer< Shader > shaderProgram);

  private:
    GLfloat *m_colorArray;

    QWidget m_widget;

    asl::ShaderParameterInfo m_info;

  private slots:
    void showColorDialog();

  signals:
    void colorChanged();
  };

} // namespace gui

#endif /* COLORPARAMETERCONTROL_H */
