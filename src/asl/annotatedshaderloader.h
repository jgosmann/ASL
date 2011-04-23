#ifndef ANNOTATEDSHADERLOADER_H
#define ANNOTATEDSHADERLOADER_H

#include <QObject>

#include "annotatedglshaderprogram.h"

namespace asl
{
class AnnotatedShaderLoader : public QObject
{
    Q_OBJECT
public:
    explicit AnnotatedShaderLoader(QObject *parent = 0);

    asl::AnnotatedGLShaderProgram * compile(const QString &source);

signals:

public slots:

};
}

#endif // ANNOTATEDSHADERLOADER_H
