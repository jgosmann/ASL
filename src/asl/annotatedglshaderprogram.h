#ifndef ANNOTATEDGLSHADER_H
#define ANNOTATEDGLSHADER_H

#include <QGLShaderProgram>
#include <QList>
#include <QStringList>

#include "shaderinfo.h"

namespace asl
{
class AnnotatedGLShaderProgram : public QGLShaderProgram
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QStringList dependencies READ dependencies)
    Q_PROPERTY(QList<asl::ShaderParameterInfo> parameters READ parameters)

public:
    explicit AnnotatedGLShaderProgram(const ShaderInfo &shaderInfo);

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

#endif // ANNOTATEDGLSHADER_H
