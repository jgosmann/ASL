#ifndef SPINBOXWIDGETWRAPPER_H
#define SPINBOXWIDGETWRAPPER_H

#include <QSpinBox>

#include "widgetwrapper.h"

namespace gui
{

    // FIXME: Probably we need two separate classes for SpinBoxes and
    // DoupleSpinBoxes.
    // Edit: There is alread a DoubleSpinBoxWidgetWrapper class. What's then the
    // purpose of the ParamT template parameter?
  template<class ParamT>
  class SpinBoxWidgetWrapper : WidgetWrapper< ParamT >
  {
  public:
    SpinBoxWidgetWrapper() {}
    ~SpinBoxWidgetWrapper() {}

    ParamT value()
    {
        // FIXME
        return 0;
      //return static_cast<ParamT>( value() );
    }

    void setValue(ParamT value)
    {
        // FIXME
      //setValue( static_cast<int>( value ) );
    }

    void setRange(ParamT min, ParamT max)
    {
        // FIXME
      //setRange( static_cast<int>( min ), static_cast<int> ( max ) );
    }

  };

} // namespace gui

#endif /* SPINBOXWIDGETWRAPPER_H */
