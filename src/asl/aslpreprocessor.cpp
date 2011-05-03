#include "aslpreprocessor.h"

#include "aslpreprocessor_parserinternal.h"

#include <exception>

using namespace asl;

ASLPreprocessor::ASLPreprocessor()
{
}

void ASLPreprocessor::process(const QString &sourcecode, QTextStream *out)
{
    try {
        ppinternal::parse(sourcecode, out);
        ppinternal::resetAndClean();
    } catch (...) {
        ppinternal::resetAndClean();
        throw;
    }
}

QString ASLPreprocessor::process(const QString &sourcecode)
{
    QString out;
    QTextStream outStream(&out, QIODevice::WriteOnly);
    process(sourcecode, &outStream);
    return out;
}

void ASLPreprocessor::resetAndClean()
{
    ppinternal::resetAndClean();
}

