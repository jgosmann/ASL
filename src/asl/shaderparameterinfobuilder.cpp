
#include "shaderparameterinfobuilder.h"

using namespace common;
using namespace asl;

ShaderParameterInfo ShaderParameterInfoBuilder::build() const
{
    ShaderParameterInfo info;
    info.name = m_name;
    return info;
}

