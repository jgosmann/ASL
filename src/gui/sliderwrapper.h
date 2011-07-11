#ifndef SLIDERWRAPPER_H
#define SLIDERWRAPPER_H

#include <GL/gl.h>

#include <QSlider>


namespace gui
{

  class SliderWrapper : public QSlider
  {
    Q_OBJECT

  public:
    SliderWrapper() {}
    ~SliderWrapper() {}

    void setValue(GLint value)
    {
      setSliderPosition( value );
    }

  };

} // namespace gui

#endif /* SLIDERWRAPPER_H */
