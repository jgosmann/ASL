
#include <QLabel>

#include "shaderparametercontrolfactory.h"
#include "sliderwidgetwrapper.h"
#include "spinboxwidgetwrapper.h"

#include "../asl/gltypeinfo.h"

using namespace asl;
using namespace gui;

void ShaderParameterControlFactory::generateControls( 
    const QSharedPointer< Shader > shaderProgram, QWidget *listener,
    QWidget &widget, 
    QList< QSharedPointer< ShaderParameterControlHandle > > 
        &shaderParameterControls )
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

// FIXME
template<class ParamT>
QSharedPointer< ShaderParameterControlHandle > 
    ShaderParameterControlFactory::createControlType( QWidget *listener,
        asl::ShaderParameterInfo &info )
{
  //foreach( QString control, info.preferredUIControls )
  //{
    //if( control == "slider" )
    //{
      //return QSharedPointer< ShaderParameterControlHandle >(
          //new ShaderParameterControl<SliderWidgetWrapper<ParamT>, ParamT>( 
              //info, listener ) );
    //} 
    //else if( control == "spinbox" )
    //{
      //return QSharedPointer< ShaderParameterControlHandle >(
          //new ShaderParameterControl<SpinBoxWidgetWrapper<ParamT>, ParamT>( 
              //info, listener ) );
    //}
  //}
  // default case
  //return QSharedPointer< ShaderParameterControlHandle >(
      //new ShaderParameterControl< SpinBoxWidgetWrapper<ParamT>, ParamT>( 
          //info, listener ) );
return QSharedPointer< ShaderParameterControlHandle >(NULL);
}


//template<>
//QSharedPointer< ShaderParameterControlHandle > 
    //ShaderParameterControlFactory::createControlType<GLuint>( QWidget *listener,
        //asl::ShaderParameterInfo &info )
//{
  //foreach( QString control, info.preferredUIControls )
  //{
    //if( control == "slider" )
    //{
      //return QSharedPointer< ShaderParameterControlHandle >(
          //new ShaderParameterControl<SliderWidgetWrapper, GLuint>( 
              //info, listener ) );
    //} 
    //else if( control == "spinbox" )
    //{
      //return QSharedPointer< ShaderParameterControlHandle >(
          //new ShaderParameterControl<SpinBoxWidgetWrapper, GLuint>( 
              //info, listener ) );
    //}
  //}
  //// default case
  //return QSharedPointer< ShaderParameterControlHandle >(
      //new ShaderParameterControl< CheckBoxWidgetWrapper, GLuint >( 
          //info, listener ) );
//}


//template<>
//QSharedPointer< ShaderParameterControlHandle > 
    //ShaderParameterControlFactory::createControlType<GLfloat>( QWidget *listener,
        //asl::ShaderParameterInfo &info )
//{
  //foreach( QString control, info.preferredUIControls )
  //{
    //if( control == "slider" )
    //{
      //return QSharedPointer< ShaderParameterControlHandle >( 
          //new ShaderParameterControl< DoubleSliderWidgetWrapper, GLfloat >( 
              //info, listener ) );
    //} 
    //else if( control == "spinbox" )
    //{
      //return QSharedPointer< ShaderParameterControlHandle >( 
          //new ShaderParameterControl< DoubleSpinBoxWidgetWrapper, GLfloat >( 
              //info, listener ) );
    //}
    //else if( control == "color" )
    //{
      //// very special case
      ////return new ColorParameterControl( info, listener );
    //}
  //}
  //// default case
  //return QSharedPointer< ShaderParameterControlHandle >( 
      //new ShaderParameterControl< DoubleSpinBoxWidgetWrapper, GLfloat >( 
        //info, listener ) );
//}
