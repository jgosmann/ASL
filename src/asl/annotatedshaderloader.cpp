#include "annotatedshaderloader.h"

using namespace asl;

AnnotatedShaderLoader::AnnotatedShaderLoader(QObject *parent) :
    QObject(parent)
{
}

AnnotatedGLShaderProgram *
AnnotatedShaderLoader::compile(const QString &source)
{
    return NULL;
}
