#ifndef LOGASSERTIONS_H
#define LOGASSERTIONS_H

#include "logentry.h"

#include <QString>

namespace asl
{

void assertLogContains(const QString &log, const LogEntry &entry);

} /* namespace asl */

#endif /* LOGASSERTIONS_H */

