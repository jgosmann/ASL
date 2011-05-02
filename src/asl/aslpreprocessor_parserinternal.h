#ifndef ASLPREPROCESSOR_PARSERINTERNAL_H
#define ASLPREPROCESSOR_PARSERINTERNAL_H

#include <QHash>
#include <QString>
#include <QStringList>

#include "compilationexception.h"

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

    QString parse(const QString &sourcecode);
} /* namespace ppinternal */
} /* namespace asl */

#endif /* ASLPREPROCESSOR_PARSERINTERNAL_H */

