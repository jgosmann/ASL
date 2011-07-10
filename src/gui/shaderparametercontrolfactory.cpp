#include "shaderparametercontrolfactory.h"

using namespace gui;

static void ShaderParameterControlFactory::generateControls( 
    const QSharedPointer< Shader > shaderProgram, QWidget *listener,
    QWidget &widget, 
    QList< QSharedPointer< ShaderParameterControlHandle > > 
        &shaderParameterControls );
{
  QVBoxLayout *container = new QVBoxLayout();

  // create the single controls
  foreach( asl::ShaderParameterInfo info, shaderProgram->parameters() )
  {

    switch( info.type->type() )
    {
    case GLTypeInfo::BOOL:
      shaderParameterControls.append( createControlType<GLuint>( listener, info ) );
      break;
    case GLTypeInfo::UINT:
      shaderParameterControls.append( createControlType<GLuint>( listener, info ) );
      break;
    case GLTypeInfo::INT:
      shaderParameterControls.append( createControlType<GLint>( listener, info ) );
      break;
    case GLTypeInfo::FLOAT:
      shaderParameterControls.append( createControlType<GLfloat>( listener, info ) );
      break;
    default:
      shaderParameterControls.append( createControlType<GLint>( listener, info ) );
    }

    QWidget *control = new QWidget();
    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget( new QLabel( tr( info.name ) ) );
    vBoxLayout->addWidget( shaderParameterControls.back()->widget() );
    control->setLayout( vBoxLayout );
    
    container->addWidget( control );
  }

  if( shaderProgram->parameters().empty() )
  {
    container->addWidget( new QLabel( tr("There are no custom parameters \
            available for this uniform.") ) );
  }

  // now unify all controls in one container-widget
  widget.setLayout( container );
}


template<ParamT>
static QSharedPointer< ShaderParameterControlHandle > 
    ShaderParameterControlFactory::createControlType<ParamT>( QWidget *listener,
        asl::ShaderParameterInfo &info )
{
  foreach( QString control, info.preferredUIControls )
  {
    if( control == "slider" )
    {
      return QSharedPointer< ShaderParameterControlHandle >(
          new ShaderParameterControl<SliderWidgetWrapper, ParamT>( 
              info, listener ) );
    } 
    else if( control == "spinbox" )
    {
      return QSharedPointer< ShaderParameterControlHandle >(
          new ShaderParameterControl<SpinBoxWidgetWrapper, ParamT>( 
              info, listener ) );
    }
  }
  // default case
  return QSharedPointer< ShaderParameterControlHandle >(
      new ShaderParameterControl< SpinBoxWidgetWrapper, ParamT>( 
          info, listener ) );
}


template<>
static QSharedPointer< ShaderParameterControlHandle > 
    ShaderParameterControlFactory::createControlType<GLuint>( QWidget *listener,
        asl::ShaderParameterInfo &info )
{
  foreach( QString control, info.preferredUIControls )
  {
    if( control == "slider" )
    {
      return QSharedPointer< ShaderParameterControlHandle >(
          new ShaderParameterControl<SliderWidgetWrapper, GLuint>( 
              info, listener ) );
    } 
    else if( control == "spinbox" )
    {
      return QSharedPointer< ShaderParameterControlHandle >(
          new ShaderParameterControl<SpinBoxWidgetWrapper, GLuint>( 
              info, listener ) );
    }
  }
  // default case
  return QSharedPointer< ShaderParameterControlHandle >(
      new ShaderParameterControl< CheckBoxWidgetWrapper, GLuint >( 
          info, listener ) );
}


template<>
static QSharedPointer< ShaderParameterControlHandle > 
    ShaderParameterControlFactory::createControlType<GLfloat>( QWidget *listener,
        asl::ShaderParameterInfo &info )
{
  foreach( QString control, info.preferredUIControls )
  {
    if( control == "slider" )
    {
      return QSharedPointer< ShaderParameterControlHandle >( 
          new ShaderParameterControl< DoubleSliderWidgetWrapper, GLfloat >( 
              info, listener ) );
    } 
    else if( control == "spinbox" )
    {
      return QSharedPointer< ShaderParameterControlHandle >( 
          new ShaderParameterControl< DoubleSpinBoxWidgetWrapper, GLfloat >( 
              info, listener ) );
    }
    else if( control == "color" )
    {
      // very special case
      //return new ColorParameterControl( info, listener );
    }
  }
  // default case
  return QSharedPointer< ShaderParameterControlHandle >( 
      new ShaderParameterControl< DoubleSpinBoxWidgetWrapper, GLfloat >( 
        info, listener ) );
}
