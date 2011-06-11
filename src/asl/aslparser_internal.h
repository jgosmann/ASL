#ifndef ASLPARSER_INTERNAL_H
#define ASLPARSER_INTERNAL_H

#include "annotatedglshaderprogram.h"

#include <QString>

namespace asl
{
namespace parserinternal
{
    extern QString log;
    extern unsigned int sourceStringNo;

    void clearLog();
    const ShaderInfo parse(const QString &sourcecode,
            const QString &pathOfSource = QString());
    void setInput(const QString &sourcecode);
    void reset();
} /* namespace parserinternal */
} /* namespace asl */

#endif /* ASLPARSER_INTERNAL_H */

