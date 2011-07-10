#ifndef DOUBLESLIDERWIDGETWRAPPER_H
#define DOUBLESLIDERWIDGETWRAPPER_H

#include <math.h>

#include <QSlider>

#include "widgetwrapper.h"

namespace gui
{

  class DoubleSliderWidgetWrapper : WidgetWrapper<  GLfloat >
  {
  public:
    DoubleSliderWidgetWrapper() {}
    ~DoubleSliderWidgetWrapper() {}

    GLfloat value()
    {
        // FIXME
        return 0;
      //GLfloat position = static_cast<GLfloat>( value() ) 
          //* pow( 10.0f, -m_precision_);
      //return static_cast<GLfloat>( position );
    }

    void setValue(GLfloat value)
    {
        // FIXME
        Q_UNUSED(value);
      //setSliderPosition( static_cast<int>( value * pow( 10.0f, m_precision_ ) ) );
    }

    void setRange(GLfloat min, GLfloat max)
    {
        // FIXME
        Q_UNUSED(min);
        Q_UNUSED(max);
      //setRange( static_cast<int>( min * pow( 10.0f, m_precision_ ) ), 
          //static_cast<int> ( max * pow( 10.0f, m_precision_ ) ) );
    }

  private:
    // number of decimal places
    static const GLfloat m_precision_ = 2.0f;

  };

} // namespace gui

#endif /* SLIDERWIDGETWRAPPER_H */
