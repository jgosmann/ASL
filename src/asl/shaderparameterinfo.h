#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <QString>
#include <QStringList>

#include "gltypeinfo.h"
#include "glvariant.h"

namespace asl
{
struct ShaderParameterInfo
{
    QString name;
    QString description;
    const asl::GLTypeInfo * type;
    asl::GLVariant defaultValue;
    asl::GLVariant minimum;
    asl::GLVariant maximum;
    QStringList preferredUIControls;
};
}

#endif // SHADERPARAMETER_H
