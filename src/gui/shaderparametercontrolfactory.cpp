#include "shaderparametercontrolfactory.h"

#include <GL/gl.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>

#include "shaderparametercontrol.h"
#include "colorparametercontrol.h"
#include "checkboxwrapper.h"
#include "sliderwrapper.h"
#include "doublesliderwrapper.h"

using namespace gui;


void ShaderParameterControlFactory::generateControls( 
    const QSharedPointer< Shader > shaderProgram, QWidget *listener,
    QWidget &widget, QList< QSharedPointer< ShaderParameterControlHandle > > 
        &shaderParameterControls )
{
  QVBoxLayout *container = new QVBoxLayout();

  // create the single controls
  foreach( asl::ShaderParameterInfo info, shaderProgram->parameters() )
  {

    shaderParameterControls.append( createControlType( info.type->type(), 
          listener, info ) );

    QWidget *control = new QWidget();
    QVBoxLayout *vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget( new QLabel(info.name) );
    vBoxLayout->addWidget( shaderParameterControls.back()->widget() );
    control->setLayout( vBoxLayout );
    
    container->addWidget( control );
  }

  if( shaderProgram->parameters().empty() )
  {
    container->addWidget( new QLabel("There are no custom parameters \
            available for this uniform.") );
  }

  // now unify all controls in one container-widget
  widget.setLayout( container );
}


QSharedPointer<ShaderParameterControlHandle> 
    ShaderParameterControlFactory::createControlType( 
      asl::GLTypeInfo::Type type, QWidget *listener,
      asl::ShaderParameterInfo &info)
{
  switch( type )
  {
  case asl::GLTypeInfo::INT:
    { 
    foreach(QString name, info.preferredUIControls)
    {
      if( name == "slider" )
      {
        return QSharedPointer<ShaderParameterControlHandle>( 
            new ShaderParameterControl<SliderWrapper, GLint>(info, listener));
      }
    }
    // if( name == "spinbox" )
    return QSharedPointer<ShaderParameterControlHandle>( 
        new ShaderParameterControl<QSpinBox, GLint>(info, listener));
   
    break;
    }

  case asl::GLTypeInfo::UINT:
    {
    foreach(QString name, info.preferredUIControls)
    {
      if( name == "slider" )
      {
        return QSharedPointer<ShaderParameterControlHandle>( 
            new ShaderParameterControl<SliderWrapper, GLuint>(info, listener));
      }
    }
    // default is "spinbox"
    return QSharedPointer<ShaderParameterControlHandle>( 
        new ShaderParameterControl<QSpinBox, GLuint>(info, listener));
   
    break;
    }

  case asl::GLTypeInfo::BOOL:
    {
    /*
    foreach(QString name, info.preferredUIControls)
    {
       *
       * There are no presets for a boolean yet, so there will be no preset
       * available at the moment. A checkbox will be the default widget.
       *
    }
    */
    // default is "checkbox"
    return QSharedPointer<ShaderParameterControlHandle>( 
        new ShaderParameterControl<CheckBoxWrapper, GLuint>(info, listener));
   
    break;
    }

  case asl::GLTypeInfo::FLOAT:
    // pass through to default case
 
  default:
    {
    foreach(QString name, info.preferredUIControls)
    {
      if( name == "slider" )
      {
        return QSharedPointer<ShaderParameterControlHandle>( 
            new ShaderParameterControl<SliderWrapper, GLuint>(info, listener));
      }
      if( name == "color" )
      {
        return QSharedPointer<ShaderParameterControlHandle>( 
            new ColorParameterControl(info, listener));
      }
    }
    // default is "spinbox"
    return QSharedPointer<ShaderParameterControlHandle>( 
        new ShaderParameterControl<QDoubleSpinBox, GLfloat>(info, listener));
    }
  }
}

// FIXME
//template<class ParamT>
//QSharedPointer< ShaderParameterControlHandle > 
//    ShaderParameterControlFactory::createControlType( QWidget *listener,
//        asl::ShaderParameterInfo &info )
//{
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
//return QSharedPointer< ShaderParameterControlHandle >(NULL);
//}

//template<>
//QSharedPointer< ShaderParameterControlHandle > 
//    ShaderParameterControlFactory::createControlType( QWidget *listener,
//        asl::ShaderParameterInfo &info )
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
//  return QSharedPointer< ShaderParameterControlHandle >( NULL );
//}

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


