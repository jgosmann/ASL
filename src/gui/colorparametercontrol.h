#ifndef COLORPARAMETERCONTROL_H
#define COLORPARAMETERCONTROL_H

#include <QWidget>
#include <QPushButton>
#include <QColorDialog>
#include <QColor>
#include <QString>
#include <QVBoxLayout>
#include <QSharedPointer>

#include "../asl/annotatedglshaderprogram.h"
#include "../asl/shaderparameterinfo.h"
#include "../asl/gltypeinfo.h"

#include "shaderparametercontrolhandle.h"
#include "widgetwrapper.h"


namespace gui
{
  typedef class asl::AnnotatedGLShaderProgram Shader;

  class ColorParameterControl : public QObject, public ShaderParameterControlHandle
  {
    Q_OBJECT
  
  public:
    ColorParameterControl(asl::ShaderParameterInfo &info, QObject *listener);
    ~ColorParameterControl();

    QWidget* widget()
    {
      return m_widget;
    }

    void setParameterFromControls(QSharedPointer< Shader > shaderProgram);

  private:
    GLfloat *m_colorArray;

    QWidget *m_widget;

    asl::ShaderParameterInfo m_info;

  private slots:
    void showColorDialog();

  signals:
    void colorChanged();
  };

} // namespace gui

#endif /* COLORPARAMETERCONTROL_H */
