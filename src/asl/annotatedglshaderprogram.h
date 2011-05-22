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
    Q_PROPERTY(QList<asl::ShaderParameterInfo> parameters READ parameters)

public:
    explicit AnnotatedGLShaderProgram(const QString &name,
            const QString &description,
            const QList<asl::ShaderParameterInfo> &parameters);

    inline const QString & name() const {
        return m_name;
    }

    inline const QString & description() const {
        return m_description;
    }

    inline const QList<asl::ShaderParameterInfo> & parameters() const {
        return m_parameters;
    }

signals:

public slots:

private:
    const QString m_name;
    const QString m_description;
    const QList<asl::ShaderParameterInfo> m_parameters;

};
}

#endif // ANNOTATEDGLSHADER_H
