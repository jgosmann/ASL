#ifndef ANNOTATEDGLSHADERPROGRAM_H
#define ANNOTATEDGLSHADERPROGRAM_H

#include <QGLShaderProgram>
#include <QList>
#include <QSharedPointer>
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

    bool addSharedShader(QSharedPointer<QGLShader> &shader);

signals:

public slots:

private:
    QList<QSharedPointer<QGLShader> > m_shadersInUse;

};
}

#endif // ANNOTATEDGLSHADERPROGRAM_H
