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

void ASLPreprocessor::process(const QString &sourcecode, QTextStream *out)
{
    ppinternal::clearLog();
    ppinternal::parse(sourcecode, out);
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

