#ifndef ANNOTATEDSHADERLOADER_H
#define ANNOTATEDSHADERLOADER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QObject>

#include "annotatedglshaderprogram.h"

namespace asl
{
class AnnotatedShaderCompiler : public QObject
{
    Q_OBJECT
public:
    explicit AnnotatedShaderCompiler(QObject *parent = 0);

    /**
     * Use always `/' as an universal directory separator for the
     * \a pathOfSource argument.
     */
    asl::AnnotatedGLShaderProgram * compile(QGLShader::ShaderType type, const
            QString &source, const QString &pathOfSource = "");

signals:

public slots:

};
}

#endif // ANNOTATEDSHADERLOADER_H
