#ifndef ANNOTATEDGLSHADER_H
#define ANNOTATEDGLSHADER_H

#include <QGLShaderProgram>
#include <QList>
#include <QStringList>

#include "annotated.h"
#include "shaderinfo.h"

namespace asl
{
class AnnotatedGLShaderProgram : public QGLShaderProgram, public Annotated
{

public:
    AnnotatedGLShaderProgram(const ShaderInfo &shaderInfo)
            : QGLShaderProgram(static_cast<QObject *>(NULL)),
            Annotated(shaderInfo) { }

signals:

public slots:

private:

};
}

#endif // ANNOTATEDGLSHADER_H
