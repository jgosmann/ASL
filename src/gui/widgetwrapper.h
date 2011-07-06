#ifndef WIDGETWRAPPER_H
#define WIDGETWRAPPER_H

namespace gui
{

  class WidgetWrapper
  {
  public:
    WidgetWrapper(QStringList &preferredUIControls);
    ~WidgetWrapper();

    /**
     * Access the original widget with this getter 
     */
    template<class ControlT>
    inline const ControlT& widget() const { 
      return *static_cast<ControlT*>( m_widget ); 
    }

    template<class ControlT, class ParamT>
    const ParamT& value() const;

    template<class ControlT, class ParamT>
    void setValue(ParamT &value);

    template<class ControlT, class ParamT>
    void setRange(ParamT &min, ParamT &max);

  private:
    /**
     * This void* will be casted to a special type in the getter.
     */
    void *m_widget;
  };

  #include "widgetwrapper.cpp"

} // namespace gui

#endif /* WIDGETWRAPPER_H */
