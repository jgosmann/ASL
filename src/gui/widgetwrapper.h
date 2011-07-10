#ifndef WIDGETWRAPPER_H
#define WIDGETWRAPPER_H


namespace gui
{

  template<class ControlT, class ParamT>
  class WidgetWrapper : public ControlT
  {
  public:
    virtual ~WidgetWrapper<ControlT, ParamT>() {}

    virtual const ParamT value() const =0;

    virtual void setValue(ParamT value)=0;
    virtual void setRange(ParamT min, ParamT max)=0;

    virtual void valueChanged(void)=0;
  };

} // namespace gui

#endif /* WIDGETWRAPPER_H */
