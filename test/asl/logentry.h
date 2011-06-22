#ifndef LOGENTRY_H
#define LOGENTRY_H

#include "../src/common/defaultable.h"

#include <QRegExp>
#include <QString>

namespace asl
{
class LogEntry
{
    public:
        LogEntry();

        LogEntry & withType(const QString &type);
        LogEntry & occuringIn(unsigned int sourceStringNo);
        LogEntry & occuringAt(unsigned int sourceStringNo, unsigned int line);
        LogEntry & withMessageMatching(const QRegExp &matchesMsg);

        inline const QString &type() const { return m_type; }
        inline unsigned int sourceStringNo() const { return m_sourceStringNo; }
        inline unsigned int line() const { return m_line.value(); }
        inline unsigned int isLineSpecified() const
             { return m_line.isSet(); }
        inline QRegExp matchesMsg() const { return m_matchesMsg; }

    private:
        QString m_type;
        unsigned int m_sourceStringNo;
        common::Defaultable<unsigned int> m_line;
        QRegExp m_matchesMsg;
};
} /* namespace asl */

#endif /* LOGENTRY_H */

