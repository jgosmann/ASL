#ifndef SLIDERWRAPPER_H
#define SLIDERWRAPPER_H

#include <qgl.h>

#include <QSlider>


namespace gui
{

  class SliderWrapper : public QSlider
  {
    Q_OBJECT

  public:
    SliderWrapper() 
    {
      connect(this, SIGNAL( valueChanged(int) ),
          this, SLOT( emitValueChanged(int) ));
    }
    ~SliderWrapper() {}

    void setValue(GLint value)
    {
      setSliderPosition( value );
    }

  public slots:
    void emitValueChanged(int value)
    {
      Q_UNUSED(value)
      emit valueChanged();
    }

  signals:
    void valueChanged();

  };

} // namespace gui

#endif /* SLIDERWRAPPER_H */
