#include "aslpreprocessor.h"

#include "aslpreprocessor_parserinternal.h"

#include <exception>

using namespace asl;

ASLPreprocessor::ASLPreprocessor()
{
}

const QString & ASLPreprocessor::log()
{
    return ppinternal::log;
}

void ASLPreprocessor::process(const QString &sourcecode, QTextStream *out,
        unsigned int sourceStringNo)
{
    ppinternal::clearLog();
    ppinternal::sourceStringNo = sourceStringNo;
    ppinternal::parse(sourcecode, out);
}

QString ASLPreprocessor::process(const QString &sourcecode,
        unsigned int sourceStringNo)
{
    QString out;
    QTextStream outStream(&out, QIODevice::WriteOnly);
    process(sourcecode, &outStream, sourceStringNo);
    return out;
}

void ASLPreprocessor::resetAndClean()
{
    ppinternal::resetAndClean();
}

