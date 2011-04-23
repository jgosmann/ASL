#ifndef ANNOTATEDGLSHADER_H
#define ANNOTATEDGLSHADER_H

#include <QGLShaderProgram>

#include "shaderparameterinfo.h"

class AnnotatedGLShaderProgram : public QGLShaderProgram
{
    Q_OBJECT
public:
    explicit AnnotatedGLShaderProgram(QObject *parent = 0);

    inline const QString & name() const {
        return m_name;
    }

    inline const QString & description() const {
        return m_description;
    }

    inline const asl::ShaderParameterInfo & parameterInfo() const {
        return m_parameterInfo;
    }

signals:

public slots:

private:
    const QString m_name;
    const QString m_description;
    const asl::ShaderParameterInfo m_parameterInfo;

};

#endif // ANNOTATEDGLSHADER_H
