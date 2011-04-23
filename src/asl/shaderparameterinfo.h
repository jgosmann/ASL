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
    public:
        const QString name;
        const QString description;
        const asl::GLTypeInfo * const type;
        const asl::GLVariant defaultValue;
        const asl::GLVariant minimum;
        const asl::GLVariant maximum;
        const QStringList preferredUIControls;

        /* TODO some working constructor */
        ShaderParameterInfo() : type(NULL) { }
    };
}

#endif // SHADERPARAMETER_H
