#ifndef SHADERPARAMETERINFOBUILDER_H
#define SHADERPARAMETERINFOBUILDER_H

#include "../common/nullable.h"
#include "../common/novalueexception.h"
#include "gltypeinfo.h"
#include "glvariant.h"
#include "shaderparameterinfo.h"

#include <QString>

namespace asl
{
class ShaderParameterInfoBuilder
{
public:
    ShaderParameterInfoBuilder() : m_type(NULL) { }

    inline void withIdentifier(const QString &identifier)
    {
        m_identifier = identifier;
    }

    inline void withName(const QString &name) { m_name = name; }

    inline void withDescription(const QString &description)
    {
        m_description = description;
    }

    inline void withType(const GLTypeInfo &type) { m_type = &type; }

    inline void withDefaultValue(const GLVariant &def) { m_defaultValue = def; }

    asl::ShaderParameterInfo build() const throw(common::NoValueException);

    inline void reset() { *this = ShaderParameterInfoBuilder(); }

private:
    QString m_identifier;
    common::Nullable<QString> m_name;
    common::Nullable<QString> m_description;
    const GLTypeInfo *m_type;
    GLVariant m_defaultValue;
};
} /* namespace asl */


#endif /* SHADERPARAMETERINFOBUILDER_H */

