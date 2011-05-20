#ifndef SHADERPARAMETERINFOMATCHER_H
#define SHADERPARAMETERINFOMATCHER_H

#include "../../src/asl/defaultprovider.h"
#include "../../src/asl/shaderparameterinfo.h"

namespace asl
{
class ShaderParameterInfoMatcher
{
    public:
        ShaderParameterInfoMatcher() : m_name(""), m_description("") { }

        inline void withName(const QString &name)
        {
            m_name = name;
        }

        inline void withDescription(const QString &description)
        {
            m_description = description;
        }

        bool matches(const ShaderParameterInfo &matchWith) const;

    private:
        template<class T> bool matchValueIfSpecified(
                const DefaultProvider<T> &expected, const T &actual) const;

        DefaultProvider<QString> m_name;
        DefaultProvider<QString> m_description;
};
} /* namespace asl */

#endif /* SHADERPARAMETERINFOMATCHER_H */

