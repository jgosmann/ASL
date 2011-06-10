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
    AnnotatedGLShader(const ShaderInfo &shaderInfo) : Annotated(shaderInfo) { };

signals:

public slots:

private:

};
}

#endif // ANNOTATEDGLSHADER_H
