
#include "shaderparameterinfomatcher.h"

using namespace asl;

bool ShaderParameterInfoMatcher::matches(const ShaderParameterInfo &matchWith)
        const
{
    return matchValueIfSpecified(m_name, matchWith.name)
        && matchValueIfSpecified(m_description, matchWith.description);
}

template<class T> bool ShaderParameterInfoMatcher::matchValueIfSpecified(
        const DefaultProvider<T> &expected, const T &actual) const
{
    return !expected.isSet() || expected.value() == actual;
}
