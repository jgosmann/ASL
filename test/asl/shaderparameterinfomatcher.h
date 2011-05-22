#ifndef SHADERPARAMETERINFOMATCHER_H
#define SHADERPARAMETERINFOMATCHER_H

#include "../../src/common/defaultable.h"
#include "../../src/asl/shaderparameterinfo.h"

namespace asl
{
class ShaderParameterInfoMatcher
{
    public:
        ShaderParameterInfoMatcher() : m_name(""), m_description("") { }

        inline ShaderParameterInfoMatcher & withName(const QString &name)
        {
            m_name = name;
            return *this;
        }

        inline ShaderParameterInfoMatcher & withDescription(
                const QString &description)
        {
            m_description = description;
            return *this;
        }

        bool matches(const ShaderParameterInfo &matchWith) const;

    private:
        template<class T> bool matchValueIfSpecified(
                const common::Defaultable<T> &expected, const T &actual) const;

        common::Defaultable<QString> m_name;
        common::Defaultable<QString> m_description;
};
} /* namespace asl */

#endif /* SHADERPARAMETERINFOMATCHER_H */

