#ifndef DEPENDENCYLOCATORMOCK_H
#define DEPENDENCYLOCATORMOCK_H

#include "../../src/asl/dependencylocator.h"

#include <gmock/gmock.h>

namespace asl
{
class DependencyLocatorMock : public DependencyLocator
{
    public:
        MOCK_METHOD2(locate, QString(const QString &name,
                const QString &includingFile));
};
} /* namespace asl */

#endif /* DEPENDENCYLOCATORMOCK_H */

