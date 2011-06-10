#ifndef ANNOTATED_H
#define ANNOTATED_H

#include <QList>
#include <QObject>
#include <QStringList>

#include "shaderinfo.h"

namespace asl
{
class Annotated : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QStringList dependencies READ dependencies)
    Q_PROPERTY(QList<asl::ShaderParameterInfo> parameters READ parameters)

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

signals:

public slots:

private:
    const ShaderInfo m_shaderInfo;

};
}

#endif // ANNOTATED_H
