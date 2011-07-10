#ifndef SPINBOXWIDGETWRAPPER_H
#define SPINBOXWIDGETWRAPPER_H

#include <QSpinBox>

#include "widgetwrapper.h"

namespace gui
{

  template<class ParamT>
  class SpinBoxWidgetWrapper : WidgetWrapper< QSpinBox, ParamT >
  {
    Q_OBJECT

  public:
    SpinBoxWidgetWrapper() {}
    ~SpinBoxWidgetWrapper() {}

    ParamT value()
    {
      return static_cast<ParamT>( value() );
    }

    void setValue(ParamT value)
    {
      setValue( static_cast<int>( value ) );
    }

    void setRange(ParamT min, ParamT max)
    {
      setRange( static_cast<int>( min ), static_cast<int> ( max ) );
    }

  };

} // namespace gui

#endif /* SPINBOXWIDGETWRAPPER_H */
