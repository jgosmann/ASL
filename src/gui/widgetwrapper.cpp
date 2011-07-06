
using namespace gui;

template<class ControlT, class ParamT>
WidgetWrapper::WidgetWrapper(void *widget)
  : m_widget(widget)
{}

template<class ControlT, class ParamT>
WidgetWrapper::~WidgetWrapper(ControlT *widget)
{
  delete m_widget;
}

template<class ControlT, class ParamT>
const ParamT& WidgetWrapper::value<QSpinBox>() const
{
  QSpinBox *widget = static_cast<QSpinBox*>( m_widget );

  return static_cast<ParamT>( widget->value() );
}
/**
 * This function treats the special case of ControlT as a QCheckBox, where the
 * value must be converted to standard boolean element of {0,1} and where the
 * getter for the value is not equal to "value()".
 */
template<>
const GLuint WidgetWrapper::value<QCheckBox>() const
{
  GLuint value = m_widget->checkState();

  if( value == Qt::Checked )
    value = 1; // = true

  return value;
}


template<class ControlT, class ParamT>
void setValue<ControlT>(ParamT &value)
{
  ControlT *widget = static_cast<ControlT*>( m_widget );

  widget->setValue( value );
}
/**
 * This function treats the special case of ControlT as a QCheckBox, where
 * there is no setValue() procedure, but a setCheckState() procedure instead.
 */
template<class ParamT>
void setValue<QCheckBox>(ParamT &value)
{
  QCheckBox *widget = static_cast<QCheckBox*>( m_widget );

  widget->setCheckState( static_cast<Qt::CheckState>( value ) );
}


template<class ControlT, class ParamT>
void setRange<ControlT>(ParamT &min, ParamT &max)
{
  ControlT *widget = static_cast<ControlT*>( m_widget );
  widget->setRange(min, max);
}
/**
 * This function treats the special cas of ControlT as a QCheckBox, where there
 * is no range but a boolean state.
 */
template<class ParamT>
void setRange<QCheckBox>(ParamT &min, ParamT &max)
{
  QCheckBox *widget = static_cast<QCheckBox*>( m_widget );
  widget->setTristate( false );
}
