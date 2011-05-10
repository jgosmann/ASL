#ifndef ASLPARSER_INTERNAL_H
#define ASLPARSER_INTERNAL_H

#include <QString>

namespace asl
{
namespace parserinternal
{
    void clearLog();
    void parse(const QString &sourcecode);
    void setInput(const QString &sourcecode);
    void resetAndClean();
} /* namespace parserinternal */
} /* namespace asl */

#endif /* ASLPARSER_INTERNAL_H */

