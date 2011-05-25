#ifndef SHADERPARAMETER_H
#define SHADERPARAMETER_H

#include <QString>
#include <QStringList>

#include "gltypeinfo.h"
#include "glvalue.h"

namespace asl
{
struct ShaderParameterInfo
{
    QString identifier;
    QString name;
    QString description;
    const asl::GLTypeInfo * type;
    asl::GLValue defaultValue;
    asl::GLValue minimum;
    asl::GLValue maximum;
    QStringList preferredUIControls;
};
}

#endif // SHADERPARAMETER_H
