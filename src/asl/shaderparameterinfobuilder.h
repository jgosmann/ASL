#ifndef SHADERPARAMETERINFOBUILDER_H
#define SHADERPARAMETERINFOBUILDER_H

#include "../common/nullable.h"
#include "shaderparameterinfo.h"

#include <QString>

namespace asl
{
class ShaderParameterInfoBuilder
{
public:
    inline void withName(const QString &name) { m_name = name; }

    asl::ShaderParameterInfo build() const;

    inline void reset() { *this = ShaderParameterInfoBuilder(); }

private:
    common::Nullable<QString> m_name;
};
} /* namespace asl */


#endif /* SHADERPARAMETERINFOBUILDER_H */

