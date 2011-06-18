#ifndef SHADERINFO_H
#define SHADERINFO_H

#include <QList>
#include <QString>
#include <QStringList>

#include "shaderparameterinfo.h"

namespace asl
{
struct ShaderInfo
{
    QString name;
    QString description;
    QStringList dependencies;
    QList<asl::ShaderParameterInfo> parameters;
};
} /* namespace asl */

#endif /* SHADERINFO_H */

