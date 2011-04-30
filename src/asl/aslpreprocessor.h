#ifndef ASLPREPROCESSOR_H
#define ASLPREPROCESSOR_H

#include <QString>

namespace asl
{

class ASLPreprocessor
{
public:
    ASLPreprocessor();

    QString process(const QString &sourcecode);
    void reset();
};
}

#endif // ASLPREPROCESSOR_H
