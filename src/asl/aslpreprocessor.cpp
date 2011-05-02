#include "aslpreprocessor.h"

#include "aslpreprocessor_parserinternal.h"

using namespace asl;

ASLPreprocessor::ASLPreprocessor()
{
}

QString ASLPreprocessor::process(const QString &sourcecode)
{
    return ppinternal::parse(sourcecode);
}

