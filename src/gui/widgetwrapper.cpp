#include "widgetwrapper.h"
 
using namespace gui;

WidgetWrapper::WidgetWrapper(asl::ShaderParameterInfo &info)
  : m_info( info ),
    m_widget( new QWidget() )
{
  QStringList preferredUIControls( m_info.preferredUIControls );

  /* 
   * Set to default value, if user specified a preset control it will be set
   * in the following while-loop.
   */
  m_preset = WidgetWrapper::DEFAULT;

  while( !preferredUIControls.empty() )
  {
    // ignore the case when check for support, just to be sure
    if( m_supportedUIControls_.contains( preferredUIControls.front(), 
          Qt::CaseInsensitive ) )
    {
      // check for negation because case of 0 / false means equal
      if( !preferredUIControls.front().compare("color", Qt::CaseInsensitive) )
      {
        m_preset = COLOR;
        break;
      } 
      else if( !preferredUIControls.front().compare("slider", 
            Qt::CaseInsensitive) )
      {
        m_preset = SLIDER;
      } 
      else if( !preferredUIControls.front().compare("spinbox",
            Qt::CaseInsensitive) )
      {
        m_preset = SPINBOX;
      } 
      else
      {
        // use default control
        break;
      }
    }
    else
    {
      m_info.preferredUIControls.pop_front();
    }
  }

  /*
   * All controls will be placed in a vBoxLayout that contains a label with the
   * name of the uniform and the control-widget.
   */
  QVBoxLayout *vBoxLayout = new QVBoxLayout( m_widget );
  vBoxLayout->addWidget( QLabel( m_info.name ) );

  switch( m_preset )
  {
  case COLOR:
    {
    QPushButton *button = new QPushButton( tr("Select color...") );
    connect( button, SIGNAL( pressed() ), this, SLOT( showColorDialog() ) );
    vBoxLayout->addWidget( button );
    break;
    }
  default:
    {
    if( m_info.type->type() == asl::GLTypeInfo::SCALAR )
    {
      m_value = m_info.defaultValue;

      switch( m_preset )
      {
      case SLIDER:
        {
          if( !m_info.type->isFloat() )
          {
            QSlider *slider = new QSlider( m_widget );
            slider->setValue( m_info.defaultValue.asInt() );
            slider->setRange( m_info.minimum.asInt(), m_info.maximum.asInt() );

            connect( slider, SIGNAL( valueChanged( int ) ), 
                this, changeValue<int>( int ) );

            vBoxLayout->addWidget( slider );
          }
          else // is int, uint or bool
          {
            // TODO: Implement DoubleSlider
            QDoubleSpinBox *spinBox = new QDoubleSpinBox( m_widget );
            spinBox->setValue( m_info.defaultValue.asFloat() );
            spinBox->setRange( m_info.minimum.asFloat(), 
                m_info.maximum.asFloat() );
            
            connect( spinBox, SIGNAL( valueChanged( double ) ),
                this, changeValue<double>( double ) );

            vBoxLayout->addWidget( spinBox );
          }
        }
      case SPINBOX:
        // no "break;"-statement will lead to default case
      default:
        {
          if( m_info.type->isFloat() )
          {
            QDoubleSpinBox *spinBox = new QDoubleSpinBox( m_widget );
            spinBox->setValue( m_info.defaultValue.asFloat() );
            spinBox->setRange( m_info.minimum.asFloat(), 
                m_info.maximum.asFloat() );
            
            connect( spinBox, SIGNAL( valueChanged( double ) ),
                this, changeValue<double>( double ) );

            vBoxLayout->addWidget( spinBox );
          }
          else
          {
            QSpinBox *spinBox = new QSpinBox( m_widget );
            spinBox->setValue( m_info.defaultValue.asInt() );
            spinBox->setRange( m_info.minimum.asInt(), 
                m_info.maximum.asInt() );
            
            connect( spinBox, SIGNAL( valueChanged( int ) ),
                this, changeValue<int>( int ) );

            vBoxLayout->addWidget( spinBox );
          }
        }
      }
    }
    else // type is VECTOR or MATRIX
    {
      QWidget *dummy = new QWidget();
      QGridLayout *gridLayout = new QGridLayout();
      dummy->setLayout( gridLayout );
      
      for(j=0; j<m_info.type->columnDimensionality(); j++) 
      { 
        for(i=0; i<m_info.type->rowDimensionality(); i++)
        {
          WidgetWrapper *control = new WidgetWrapper( info );

          // set Minimum and Maximum value dependent to ParamT
          control->setRange( m_info.minimum, m_info.maximum);

          // set initial value
          control->setValue( m_info.defaultValue );

          // add control - layout will resize automatically
          gridLayout->addWidget( control->widget(), i, j);

          // create column major array
          m_controls[i + j*m_cols] = control;
        }
      }
    }
    break;
    }
  }

  // assign the content to the widget
  m_widget.layout()->addWidget( vBoxLayout )

  QGridLayout *gridLayout = new QGridLayout(&m_widget);
  m_widget.setLayout(gridLayout); // necessary?

  m_controls = new WidgetWrapper*[m_rows*m_cols];


  // register itself as a widget related to info.
  m_iter = m_instances.insertMulti( info, this );
}

WidgetWrapper::~WidgetWrapper()
{
  // erase this instance from the static list ...
  m_instances_.erase( m_iter );
  // ... and update the invalid iterators
  QHash::iterator iter;
  for(iter = m_instances.begin() ; iter != m_instances.end(); iter++)
    iter->setIterator( iter );


  delete m_widget;
}


void WidgetWrapper::showColorDialog()
{
  QColor colorChosen;
  qreal* color = new qreal[ m_info.rowDimensionality() ];

  qreal* defaultValues = m_info.defaultValue.as<qreal>();
  QColor defaultColor = QColor();
 
  if( m_info.rowDimensionality() == 3 )
  {
    defaultColor.setRgbF( defaultValues[0], defaultValues[1], defaultValues[2] );

    colorChosen = QColorDialog::getColor( defaultColor, m_widget,
        tr("Select a color please...") );
    color.getRgbF( color[0], color[1], color[2]);
  }
  else 
  {
    assert( m_info.rowDimensionality() == 4 );

    defaultColor.setRgbF( defaultValues[0], defaultValues[1], defaultValues[2],
        defaultValues[3] );

    /*
     * If the vector has 4 dimensions the alpha-channel must be selectable too.
     */
    colorChosen = QColorDialog::getColor( defaultColor, m_widget,
        tr("Select a color please..."), QColorDialog::ShowAlphaChannel);
    color.getRgbF( color[0], color[1], color[2], color[3] );
  }

  setValue( asl::GLVariant<GLfloat>( m_info, 3, color[0] ) );

  update<GLfloat>();
}


/**
 * This function treats the special case of ControlT as a QCheckBox, where the
 * value must be converted to standard boolean element of {0,1} and where the
 * getter for the value is not equal to "value()".
 */
template<>
const GLuint WidgetWrapper::value() const
{
  QCheckBox *widget = static_cast<QCheckBox*>( m_widget );
  GLuint value = widget->checkState();

  if( value == Qt::Checked )
    value = 1; // = true

  return value;
}


template<class ControlT, class ParamT>
void setValue(ParamT &value)
{
  ControlT *widget = static_cast<ControlT*>( m_widget );

  widget->setValue( value );
}
/**
 * This function treats the special case of ControlT as a QCheckBox, where
 * there is no setValue() procedure, but a setCheckState() procedure instead.
 */
template<class ParamT>
void setValue(ParamT &value)
{
  QCheckBox *widget = static_cast<QCheckBox*>( m_widget );

  widget->setCheckState( static_cast<Qt::CheckState>( value ) );
}


template<class ControlT, class ParamT>
void setRange(ParamT &min, ParamT &max)
{
  ControlT *widget = static_cast<ControlT*>( m_widget );
  widget->setRange(min, max);
}
/**
 * This function treats the special cas of ControlT as a QCheckBox, where there
 * is no range but a boolean state.
 */
template<class ParamT>
void setRange(ParamT &min, ParamT &max)
{
  QCheckBox *widget = static_cast<QCheckBox*>( m_widget );
  widget->setTristate( false );
}
