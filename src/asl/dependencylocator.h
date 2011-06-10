#ifndef DEPENDENCYLOCATOR_H
#define DEPENDENCYLOCATOR_H

#include <QString>

namespace asl
{
class DependencyLocator
{
    public:
        virtual ~DependencyLocator() { }
        virtual QString locate(const QString &name,
                const QString &includingFile = QString()) = 0;
};
} /* namespace asl */

#endif /* DEPENDENCYLOCATOR_H */

