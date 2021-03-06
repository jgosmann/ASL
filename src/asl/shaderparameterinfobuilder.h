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
    inline void withMinimum(const GLVariant &min) { m_minimum = min; }
    inline void withMaximum(const GLVariant &max) { m_maximum = max; }
    inline void withNoMinimum() { m_minimum.unset(); }
    inline void withNoMaximum() { m_maximum.unset(); }
    inline void withPreferredUIControls(const QStringList &controls)
    {
        m_preferredUIControls = controls;
    }

    asl::ShaderParameterInfo build() const throw(common::NoValueException);

    inline void reset() { *this = ShaderParameterInfoBuilder(); }

private:
    QString m_identifier;
    common::Nullable<QString> m_name;
    common::Nullable<QString> m_description;
    const GLTypeInfo *m_type;
    GLVariant m_defaultValue;
    common::Nullable<GLVariant> m_minimum;
    common::Nullable<GLVariant> m_maximum;
    QStringList m_preferredUIControls;
};
} /* namespace asl */


#endif /* SHADERPARAMETERINFOBUILDER_H */

