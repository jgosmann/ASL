#ifndef ANNOTATED_H
#define ANNOTATED_H

#include <QList>
#include <QStringList>

#include "shaderinfo.h"

namespace asl
{
class Annotated
{
public:
    Annotated(const ShaderInfo &shaderInfo) : m_shaderInfo(shaderInfo) { }
    virtual ~Annotated() { }

    inline const QString & name() const {
        return m_shaderInfo.name;
    }

    inline const QString & description() const {
        return m_shaderInfo.description;
    }

    inline const QStringList & dependencies() const {
        return m_shaderInfo.dependencies;
    }

    inline const QList<asl::ShaderParameterInfo> & parameters() const {
        return m_shaderInfo.parameters;
    }

private:
    const ShaderInfo m_shaderInfo;
};
}

#endif // ANNOTATED_H
