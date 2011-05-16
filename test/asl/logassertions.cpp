
#include "../common.h"
#include "logassertions.h"

#include <QRegExp>

using namespace asl;

void asl::assertLogContains(const QString &log, const LogEntry &entry)
{
    const QRegExp matcher(
            entry.type() + ":\\s+(\\d+):(\\d+):\\s+([^\r\n]*)");

    int pos = 0;
    while (0 <= (pos = matcher.indexIn(log, pos))) {
        bool fileCorrect = (matcher.cap(1).toUInt() == entry.sourceStringNo());
        bool lineCorrect = !entry.isLineSpecified()
            || (matcher.cap(2).toUInt() == entry.line());
        bool mustContainCorrect =
            (0 <= entry.matchesMsg().indexIn(matcher.cap(3)));

        if (fileCorrect && lineCorrect && mustContainCorrect) {
            return;
        }

        pos += matcher.matchedLength();
    }

    QString linePart = entry.isLineSpecified() ? QString::number(entry.line())
            : "*";
    CPPUNIT_FAIL(qPrintable("Expected " + entry.type() + " containing \""
                + entry.matchesMsg().pattern() + "\" in sourcestring and line "
                + QString::number(entry.sourceStringNo()) + ":" + linePart
                + ". Actual log:\n" + log));
}

