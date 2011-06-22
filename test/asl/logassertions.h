#ifndef LOGASSERTIONS_H
#define LOGASSERTIONS_H

#include "logentry.h"

#include <QString>

namespace asl
{

void assertLogContains(const QString &log, const LogEntry &entry);
void assertLogContainsEntryOfType(const QString &log, const QString &type);
} /* namespace asl */

#endif /* LOGASSERTIONS_H */

