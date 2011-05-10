
#include "common.h"

using namespace std;

ostream & operator<<(ostream &output, const QString &str)
{
    return (output << qPrintable(str));
}

