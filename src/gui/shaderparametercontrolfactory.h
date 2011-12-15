#ifndef SHADERPARAMETERCONTROLFACTORY_H
#define SHADERPARAMETERCONTROLFACTORY_H

#include <QList>
#include <QObject>
#include <QString>
#include <QWidget>

#include "../asl/gltypeinfo.h"
#include "../asl/shaderparameterinfo.h"

#include "shaderparametercontrolhandle.h"


namespace gui {

  class ShaderParameterControlFactory : public QObject
  {
    Q_OBJECT

  public:
    void generateControls( const QSharedPointer< Shader > shaderProgram, 
        QObject *listener, QWidget &widget, 
        QList< QSharedPointer< ShaderParameterControlHandle > > 
            &shaderParameterControls );

  private:

    QSharedPointer< ShaderParameterControlHandle > createControlType(
        asl::GLTypeInfo::Type type, QObject *listener, 
        asl::ShaderParameterInfo &info );
  };

} // namespace gui

#endif // SHADERPARAMETERCONTROLFACTORY_H
