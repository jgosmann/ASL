#ifndef ASLPARSER_INTERNAL_H
#define ASLPARSER_INTERNAL_H

#include "annotatedglshaderprogram.h"

#include <QString>

namespace asl
{
namespace parserinternal
{
    void clearLog();
    AnnotatedGLShaderProgram * parse(const QString &sourcecode,
            const QString &pathOfSource = QString());
    void setInput(const QString &sourcecode);
    void resetAndClean();
} /* namespace parserinternal */
} /* namespace asl */

#endif /* ASLPARSER_INTERNAL_H */

