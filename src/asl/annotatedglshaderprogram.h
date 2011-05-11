#ifndef ANNOTATEDGLSHADER_H
#define ANNOTATEDGLSHADER_H

#include <QGLShaderProgram>
#include <QList>

#include "shaderparameterinfo.h"

namespace asl
{
class AnnotatedGLShaderProgram : public QGLShaderProgram
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString description READ description)

public:
    explicit AnnotatedGLShaderProgram(const QString &name,
            const QString &description);

    inline const QString & name() const {
        return m_name;
    }

    inline const QString & description() const {
        return m_description;
    }

    inline const QList<asl::ShaderParameterInfo> & parameterInfo() const {
        return m_parameterInfo;
    }

signals:

public slots:

private:
    const QString m_name;
    const QString m_description;
    const QList<asl::ShaderParameterInfo> m_parameterInfo;

};
}

#endif // ANNOTATEDGLSHADER_H
