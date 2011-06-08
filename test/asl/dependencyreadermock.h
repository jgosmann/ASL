#ifndef DEPENDENCYREADERMOCK_H
#define DEPENDENCYREADERMOCK_H

#include "../../src/asl/dependencyreader.h"

#include <gmock/gmock.h>

namespace asl
{
class DependencyReaderMock : public DependencyReader
{
    public:
        MOCK_METHOD2(readDependency, QString(const QString &name,
                    const QString &includingFile));
};
} /* namespace asl */

#endif /* DEPENDENCYREADERMOCK_H */

