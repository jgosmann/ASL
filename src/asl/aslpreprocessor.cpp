#include "aslpreprocessor.h"

#include <QTextStream>

using namespace asl;

/* Defined in aslpreprocessor.y. */
QString aslPreprocessorParse(const QString &sourcecode);

ASLPreprocessor::ASLPreprocessor()
{
}

QString ASLPreprocessor::process(const QString &sourcecode)
{
    return aslPreprocessorParse(sourcecode);
}

