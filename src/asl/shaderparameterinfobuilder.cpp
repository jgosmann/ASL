
#include "shaderparameterinfobuilder.h"

using namespace common;
using namespace asl;

ShaderParameterInfo ShaderParameterInfoBuilder::build() const
{
    ShaderParameterInfo info;
    if (m_name.hasValue()) {
        info.name = m_name.value();
    } else {
        info.name = m_identifier;
    }
    return info;
}

