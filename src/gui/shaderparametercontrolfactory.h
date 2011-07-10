#ifndef SHADERPARAMETERCONTROLFACTORY_H
#define SHADERPARAMETERCONTROLFACTORY_H

#include <QList>
#include <QObject>
#include <QString>
#include <QWidget>
#include <QVBoxLayout>

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

  class ShaderParameterControlFactory : public QObject
  {
      Q_OBJECT

  public:
    void generateControls( const QSharedPointer< Shader > shaderProgram, 
        QWidget *listener, QWidget &widget, 
        QList< QSharedPointer< ShaderParameterControlHandle > > 
            &shaderParameterControls );

  private:
    // FIXME just a quickfix
    QString tr(const QString &str) { return tr(qPrintable(str)); }

    template<class ParamT>
    QSharedPointer< ShaderParameterControlHandle > createControlType(
        QWidget *listener, asl::ShaderParameterInfo &info );
  };

} // namespace gui

#endif // SHADERPARAMETERCONTROLFACTORY_H
