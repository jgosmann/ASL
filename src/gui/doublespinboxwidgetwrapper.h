#ifndef DOUBLESPINBOXWIDGETWRAPPER_H
#define DOUBLESPINBOXWIDGETWRAPPER_H

#include <QDoubleSpinBox>

#include "widgetwrapper.h"

namespace gui
{

  class DoubleSpinBoxWidgetWrapper : WidgetWrapper< QDoubleSpinBox, GLfloat >
  {
    Q_OBJECT

  public:
    DoubleSpinBoxWidgetWrapper() {}
    ~DoubleSpinBoxWidgetWrapper() {}

    GLfloat value()
    {
      return static_cast<GLfloat>( value() );
    }

    void setValue(GLfloat value)
    {
      setValue( static_cast<double>( value ) );
    }

    void setRange(GLfloat min, GLfloat max)
    {
      setRange( static_cast<double>( min ), static_cast<double> ( max ) );
    }

  };

} // namespace gui

#endif /* DOUBLESPINBOXWIDGETWRAPPER_H */
