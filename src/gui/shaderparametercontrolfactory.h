#ifndef SHADERPARAMETERCONTROLFACTORY_H
#define SHADERPARAMETERCONTROLFACTORY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QList>

#include "../asl/shaderparameterinfo.h"

#include "shaderparametercontrol.h"
#include "colorparametercontrol.h"
#include "shaderparametercontrolhandle.h"
#include "checkboxwidgetwrapper.h"
#include "spinboxwidgetwrapper.h"
#include "doublespinboxwidgetwrapper.h"
#include "sliderwidgetwrapper.h"
#include "doublesliderwidgetwrapper.h"

namespace gui {

  class ShaderParameterControlFactory
  {
  public:
    static void generateControls( const QSharedPointer< Shader > shaderProgram, 
        QWidget *listener, QWidget &widget, 
        QList< QSharedPointer< ShaderParameterControlHandle > > 
            &shaderParameterControls );

  private:
    template<class ParamT>
    static QSharedPointer< ShaderParameterControlHandle > createControlType(
        QWidget *listener, asl::ShaderParameterInfo &info );
  };

} // namespace gui

#endif // SHADERPARAMETERCONTROLFACTORY_H
