#include "aslpreprocessor.h"

#include <QTextStream>

using namespace asl;

/* Defined in aslpreprocessor.y. */
QString aslPreprocessorParse(const QString &sourcecode);
void aslPreprocessorReset();

ASLPreprocessor::ASLPreprocessor()
{
}

QString ASLPreprocessor::process(const QString &sourcecode)
{
    return aslPreprocessorParse(sourcecode);
}

void ASLPreprocessor::reset() {
    aslPreprocessorReset();
}

