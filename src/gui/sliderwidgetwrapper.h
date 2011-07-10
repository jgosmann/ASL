#ifndef SLIDERWIDGETWRAPPER_H
#define SLIDERWIDGETWRAPPER_H

#include <QSlider>

#include "widgetwrapper.h"

namespace gui
{

  template<class ParamT>
  class SliderWidgetWrapper : WidgetWrapper< QSlider, ParamT >
  {
    Q_OBJECT

  public:
    SliderWidgetWrapper() {}
    ~SliderWidgetWrapper() {}

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

#endif /* SLIDERWIDGETWRAPPER_H */
