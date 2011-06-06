#ifndef DEPENDENCYREADERMOCK_H
#define DEPENDENCYREADERMOCK_H

#include "../../src/asl/dependencyreader.h"

#include <gmock/gmock.h>

namespace asl
{
class DependencyReaderMock : public DependencyReader
{
    public:
        MOCK_METHOD1(readDependency, QString(const QString &name));
};
} /* namespace asl */

#endif /* DEPENDENCYREADERMOCK_H */

