#ifndef ASLPREPROCESSOR_PARSERINTERNAL_H
#define ASLPREPROCESSOR_PARSERINTERNAL_H

#include <QHash>
#include <QString>
#include <QStringList>

#include "compilationexception.h"

extern int aslpreprocessorlineno;

namespace asl
{
namespace ppinternal
{
    typedef struct macroPart_t {
        bool isArgument;
        QString text;
    } MacroPart;

    typedef struct macro_t {
        QStringList *arguments;
        QList<MacroPart> *parts;
    } Macro;

    extern QHash<QString, Macro> macroTable;

    bool isDefined(const QString &macroName);
    QString parse(const QString &sourcecode);

    void pushInput(const QString &sourcecode);
    void clean();
    void reset();

} /* namespace ppinternal */
} /* namespace asl */

#endif /* ASLPREPROCESSOR_PARSERINTERNAL_H */

