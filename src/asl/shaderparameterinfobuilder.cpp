
#include "shaderparameterinfobuilder.h"

using namespace common;
using namespace asl;

ShaderParameterInfo ShaderParameterInfoBuilder::build() const
        throw(NoValueException)
{
    ShaderParameterInfo info;

    if (!m_type) {
        throw NoValueException("Missing type.");
    }
    info.type = m_type;

    if (m_name.hasValue()) {
        info.name = m_name.value();
    } else {
        info.name = m_identifier;
    }

    return info;
}

