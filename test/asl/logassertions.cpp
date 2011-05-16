
#include "../common.h"
#include "logassertions.h"

#include <QRegExp>

using namespace asl;

void asl::assertLogContains(const QString &log, const LogEntry &entry)
{
    const QRegExp matcher(
            "(\\d+):(\\d+):\\s+error:\\s+\\(preprocessor\\)\\s+([^\r\n]*)");

    int pos = 0;
    while (0 <= (pos = matcher.indexIn(log, pos))) {
        bool fileCorrect = (matcher.cap(1).toUInt() == entry.sourceStringNo());
        bool lineCorrect = (matcher.cap(2).toUInt() == entry.line());
        bool mustContainCorrect =
            (0 <= entry.matchesMsg().indexIn(matcher.cap(3)));

        if (fileCorrect && lineCorrect && mustContainCorrect) {
            return;
        }

        pos += matcher.matchedLength();
    }

    CPPUNIT_FAIL(("Expected error containing \"" + entry.matchesMsg().pattern()
                + "\" in sourcestring and line " + entry.sourceStringNo() + ":"
                + QString::number(entry.line()) + ". Actual log:\n"
                + log).toAscii().constData());
}

