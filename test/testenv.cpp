
#include "testenv.h"

using namespace std;

ostream & operator<<(ostream &output, const QString &str)
{
    return (output << qPrintable(str));
}

ostream & operator<<(ostream &output, const QStringList &strList)
{
    return (output << qPrintable(strList.join(", ")));
}

