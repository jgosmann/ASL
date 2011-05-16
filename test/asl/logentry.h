#ifndef LOGENTRY_H
#define LOGENTRY_H

#include <QRegExp>
#include <QString>

namespace asl
{
class LogEntry
{
    public:
        LogEntry();

        LogEntry & withType(const QString &type);
        LogEntry & occuringAt(unsigned int sourceStringNo, unsigned int line);
        LogEntry & withMessageMatching(const QRegExp &matchesMsg);

        inline const QString &type() const { return m_type; }
        inline unsigned int sourceStringNo() const { return m_sourceStringNo; }
        inline unsigned int line() const { return m_line; }
        inline QRegExp matchesMsg() const { return m_matchesMsg; }

    private:
        QString m_type;
        unsigned int m_sourceStringNo;
        unsigned int m_line;
        QRegExp m_matchesMsg;
};
} /* namespace asl */

#endif /* LOGENTRY_H */

