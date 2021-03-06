#ifndef ASLPREPROCESSOR_PARSERINTERNAL_H
#define ASLPREPROCESSOR_PARSERINTERNAL_H

#include <QHash>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QTextStream>

extern int aslpreprocessorlineno;

namespace asl
{
namespace ppinternal
{
    extern unsigned int sourceStringNo;
    extern unsigned int glslVersion;

    extern QString log;
    extern bool success;
    extern int ifNestingLevel;
    extern int excludeIfNestingLevel;

    typedef struct macroPart_t {
        bool isArgument;
        QString text;
    } MacroPart;

    typedef struct macro_t {
        QSharedPointer<QStringList> arguments;
        QSharedPointer<QList<MacroPart> > parts;
    } Macro;

    extern QHash<QString, Macro> macroTable;

    void clearLog();
    bool isDefined(const QString &macroName);
    void parse(const QString &sourcecode, QTextStream *out);

    void pushInput(const QString &sourcecode);
    void resetAndClean();

} /* namespace ppinternal */
} /* namespace asl */

#endif /* ASLPREPROCESSOR_PARSERINTERNAL_H */

