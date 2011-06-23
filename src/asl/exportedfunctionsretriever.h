#ifndef EXPORTEDFUNCTIONSRETRIEVER_H
#define EXPORTEDFUNCTIONSRETRIEVER_H

#include <QGLShader>
#include <QStringList>

namespace asl
{
class ExportedFunctionsRetriever
{
public:
    virtual ~ExportedFunctionsRetriever() { }

    virtual QStringList getExportedFunctionsForDependency(
            QGLShader::ShaderType type, const QString &dependency,
            const QString &includingFile) = 0;
};

} /* namespace asl */

#endif /* EXPORTEDFUNCTIONSRETRIEVER_H */

