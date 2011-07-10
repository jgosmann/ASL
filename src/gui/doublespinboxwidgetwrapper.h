#ifndef DOUBLESPINBOXWIDGETWRAPPER_H
#define DOUBLESPINBOXWIDGETWRAPPER_H

#include <QDoubleSpinBox>
#include <GL/gl.h>

#include "widgetwrapper.h"

namespace gui
{

  class DoubleSpinBoxWidgetWrapper : WidgetWrapper< GLfloat >
  {
  public:
    DoubleSpinBoxWidgetWrapper() {}
    ~DoubleSpinBoxWidgetWrapper() {}

    GLfloat value()
    {
        //FIXME
        return 0;
      //return static_cast<GLfloat>( value() );
    }

    void setValue(GLfloat value)
    {
        // FIXME
        Q_UNUSED(value);
      //setValue( static_cast<double>( value ) );
    }

    void setRange(GLfloat min, GLfloat max)
    {
        // FIXME
        Q_UNUSED(min);
        Q_UNUSED(max);
      //setRange( static_cast<double>( min ), static_cast<double> ( max ) );
    }

  };

} // namespace gui

#endif /* DOUBLESPINBOXWIDGETWRAPPER_H */
