#ifndef RELATIVEDEPENDENCYLOCATOR_H
#define RELATIVEDEPENDENCYLOCATOR_H

#include "dependencylocator.h"

namespace asl
{
class RelativeDependencyLocator : public DependencyLocator
{
public:
    QString locate(const QString &name,
            const QString &includingFile = QString());
};
} /* namespace asl */


#endif /* RELATIVEDEPENDENCYLOCATOR_H */

