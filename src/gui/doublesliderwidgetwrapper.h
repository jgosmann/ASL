#ifndef DOUBLESLIDERWIDGETWRAPPER_H
#define DOUBLESLIDERWIDGETWRAPPER_H

#include <math.h>

#include <QSlider>

#include "widgetwrapper.h"

namespace gui
{

  class DoubleSliderWidgetWrapper : WidgetWrapper< QSlider, GLfloat >
  {
    Q_OBJECT

  public:
    DoubleSliderWidgetWrapper() {}
    ~DoubleSliderWidgetWrapper() {}

    GLfloat value()
    {
      GLfloat position = static_cast<GLfloat>( value() ) 
          * pow( 10.0f, -m_precision_);
      return static_cast<GLfloat>( position );
    }

    void setValue(GLfloat value)
    {
      setSliderPosition( static_cast<int>( value * pow( 10.0f, m_precision_ ) ) );
    }

    void setRange(GLfloat min, GLfloat max)
    {
      setRange( static_cast<int>( min * pow( 10.0f, m_precision_ ) ), 
          static_cast<int> ( max * pow( 10.0f, m_precision_ ) ) );
    }

  private:
    // number of decimal places
    static const GLfloat m_precision_ = 2.0f;

  };

} // namespace gui

#endif /* SLIDERWIDGETWRAPPER_H */
