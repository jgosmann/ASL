#ifndef EXPORTEDFUNCTIONSRETRIEVERMOCK_H
#define EXPORTEDFUNCTIONSRETRIEVERMOCK_H

#include "../../src/asl/exportedfunctionsretriever.h"

#include <gmock/gmock.h>

namespace asl
{
class ExportedFunctionsRetrieverMock : public ExportedFunctionsRetriever
{
    public:
        MOCK_METHOD3(getExportedFunctionsForDependency,
                QStringList(QGLShader::ShaderType type,
                    const QString &dependency, const QString &includingFile));
};
} /* namespace asl */

#endif /* EXPORTEDFUNCTIONSRETRIEVERMOCK_H */

