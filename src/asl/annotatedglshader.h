#ifndef ANNOTATEDGLSHADER_H
#define ANNOTATEDGLSHADER_H

#include <QGLShader>
#include <QList>
#include <QStringList>

#include "annotated.h"
#include "shaderinfo.h"

namespace asl
{
class AnnotatedGLShader : public QGLShader, public Annotated
{
public:
    AnnotatedGLShader(QGLShader::ShaderType type, const ShaderInfo &shaderInfo,
            QObject *parent = NULL)
        : QGLShader(type, parent), Annotated(shaderInfo) { };

signals:

public slots:

private:

};
}

#endif // ANNOTATEDGLSHADER_H
