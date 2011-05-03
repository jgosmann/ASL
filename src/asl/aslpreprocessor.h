#ifndef ASLPREPROCESSOR_H
#define ASLPREPROCESSOR_H

#include <QString>
#include <QTextStream>

namespace asl
{

class ASLPreprocessor
{
public:
    ASLPreprocessor();

    void process(const QString &sourcecode, QTextStream *out);
    QString process(const QString &sourcecode);

    void resetAndClean();
};
}

#endif // ASLPREPROCESSOR_H
