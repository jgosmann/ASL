#ifndef SHADERPARAMETERINFOMATCHER_H
#define SHADERPARAMETERINFOMATCHER_H

#include "../../src/common/nullable.h"
#include "../../src/asl/shaderparameterinfo.h"

namespace asl
{
class ShaderParameterInfoMatcher
{
    public:
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

        inline ShaderParameterInfoMatcher & withType(const GLTypeInfo *type)
        {
            m_type = type;
            return *this;
        }

        bool matches(const ShaderParameterInfo &matchWith) const;

    private:
        template<class T> bool matchValueIfSpecified(
                const common::Nullable<T> &expected, const T &actual) const;
        template<class T> bool matchValueIfSpecified(
                const common::Nullable<const T *> &expected, const T *actual)
                const;

        common::Nullable<QString> m_name;
        common::Nullable<QString> m_description;
        common::Nullable<const asl::GLTypeInfo *> m_type;
};
} /* namespace asl */

#endif /* SHADERPARAMETERINFOMATCHER_H */

