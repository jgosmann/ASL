#ifndef DEPENDENCYREADER_H
#define DEPENDENCYREADER_H

#include <QString>

namespace asl
{
class DependencyReader
{
    public:
        virtual ~DependencyReader() { }
        virtual QString readDependency(const QString &name) = 0;
};
} /* namespace asl */

#endif /* DEPENDENCYREADER_H */

