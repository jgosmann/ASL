
#include "logentry.h"

using namespace asl;

LogEntry::LogEntry() : m_type(QString()), m_sourceStringNo(0), m_line(0),
        m_isLineSpecified(false), m_matchesMsg(QRegExp(".*"))
{
}

LogEntry & LogEntry::withType(const QString &type)
{
    m_type = type;
    return *this;
}

LogEntry & LogEntry::occuringIn(unsigned int sourceStringNo)
{
    m_sourceStringNo = sourceStringNo;
    m_isLineSpecified = false;
    return *this;
}

LogEntry & LogEntry::occuringAt(unsigned int sourceStringNo, unsigned int line)
{
    m_sourceStringNo = sourceStringNo;
    m_line = line;
    m_isLineSpecified = true;
    return *this;
}

LogEntry & LogEntry::withMessageMatching(const QRegExp &matchesMsg)
{
    m_matchesMsg = matchesMsg;
    return *this;
}

