
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

    if (m_identifier.isEmpty()) {
        throw NoValueException("Missing identifier.");
    }
    info.identifier = m_identifier;

    info.name = m_name.valueOrDefault(m_identifier);
    info.description = m_description.valueOrDefault("");

    info.defaultValue = GLVariant(*m_type, m_defaultValue);
    info.minimum = GLVariant(*m_type,
            m_minimum.valueOrDefault(GLVariant::minOfType(*m_type)));
    info.maximum = GLVariant(*m_type,
            m_maximum.valueOrDefault(GLVariant::maxOfType(*m_type)));

    info.preferredUIControls = m_preferredUIControls;

    return info;
}

