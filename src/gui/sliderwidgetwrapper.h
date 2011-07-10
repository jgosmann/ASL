#ifndef SLIDERWIDGETWRAPPER_H
#define SLIDERWIDGETWRAPPER_H

#include <QSlider>

#include "widgetwrapper.h"

namespace gui
{

  template<class ParamT>
  class SliderWidgetWrapper : WidgetWrapper< ParamT >
  {
  public:
    SliderWidgetWrapper() {}
    ~SliderWidgetWrapper() {}

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

#endif /* SLIDERWIDGETWRAPPER_H */
