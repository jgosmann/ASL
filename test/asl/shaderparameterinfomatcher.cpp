
#include "shaderparameterinfomatcher.h"

using namespace common;
using namespace asl;

ShaderParameterInfoMatcher::ShaderParameterInfoMatcher(
        const ShaderParameterInfo &other)
{
    m_identifier = other.identifier;
    m_name = other.name;
    m_description = other.description;
    if (other.type) {
        m_type = other.type;
    }
    m_defaultValue = other.defaultValue;
    m_minValue = other.minimum;
    m_maxValue = other.maximum;
    m_preferredUIControls = other.preferredUIControls;
}

bool ShaderParameterInfoMatcher::matches(const ShaderParameterInfo &matchWith)
        const
{
    return matchValueIfSpecified(m_identifier, matchWith.identifier)
        && matchValueIfSpecified(m_name, matchWith.name)
        && matchValueIfSpecified(m_description, matchWith.description)
        && matchValueIfSpecified(m_type, matchWith.type)
        && matchValueIfSpecified(m_defaultValue, matchWith.defaultValue)
        && matchValueIfSpecified(m_minValue, matchWith.minimum)
        && matchValueIfSpecified(m_maxValue, matchWith.maximum)
        && matchValueIfSpecified(m_preferredUIControls,
                matchWith.preferredUIControls);
}

template<class T> bool ShaderParameterInfoMatcher::matchValueIfSpecified(
        const Nullable<T> &expected, const T &actual) const
{
    return !expected.hasValue() || expected.value() == actual;
}

template<class T> bool ShaderParameterInfoMatcher::matchValueIfSpecified(
        const Nullable<const T *> &expected, const T *actual) const
{
    return !expected.hasValue() || (actual && *(expected.value()) == *actual);
}

bool operator==(const asl::ShaderParameterInfo &lhs,
        const asl::ShaderParameterInfo &rhs)
{
    asl::ShaderParameterInfoMatcher matcher(lhs);
    return matcher.matches(rhs);
}

