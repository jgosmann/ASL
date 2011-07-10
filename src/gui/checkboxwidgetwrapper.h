#ifndef CHECKBOXWIDGETWRAPPER_H
#define CHECKBOXWIDGETWRAPPER_H

#include <math.h>

#include <QCheckBox>

#include "widgetwrapper.h"

namespace gui
{

  class CheckBoxWidgetWrapper : WidgetWrapper< QCheckBox, GLuint >
  {
    Q_OBJECT

  public:
    CheckBoxWidgetWrapper() {}
    ~CheckBoxWidgetWrapper() {}

    const GLuint value() const
    {
      GLuint state;
      switch( checkState() )
      {
      case Qt::Unchecked:
        state = 0; // false
        break;
      case Qt::PartiallyChecked:
        // see below
      case Qt::Checked:
        state = 1; // true
        break;
      }

      return state;
    }

    void setValue(GLuint value)
    {
      if( value == 1 ) // true
        setCheckState( Qt::Checked );
      else // false
        setCheckState( Qt::Unchecked );
    }

    void setRange(GLuint min, GLuint max)
    {
      Q_UNUSED( min )
      Q_UNUSED( max )

      // No need to set range. But improvements for tri- or higher multistate
      // checkboxes migth be implemented.
    }
  };

} // namespace gui

#endif /* CHECKBOXWIDGETWRAPPER_H */
