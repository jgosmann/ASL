
%{
    #include <QHash>
    #include <QIODevice>
    #include <QString>
    #include <QTextStream>

    #ifdef TEST
        #include "../src/asl/compilationexception.h"
    #else
        #include "asl/compilationexception.h"
    #endif

    QTextStream m_outStream;
    QHash<QString, QString> macroTable;
    bool m_stripCode = false;
    unsigned int ifNestingLevel = 0;
    unsigned int nestingLevelToStrip;

    void setInput(const QString &sourcecode);

    extern int aslpreprocessorlineno;

    bool isDefined(const char *macroName);

    void aslpreprocessorerror(char *msg);
    int yylex(void);

%}

%union {
    const char *string;
}

%token <string> CHARACTERS
%token DEFINE UNDEF IFDEF IFNDEF ELSE ENDIF
%token <string> IDENTIFIER

%%

program:
       program part
        |
        ;

part:
    pp
    | CHARACTERS {
            if (!m_stripCode) {
                m_outStream << $1;
            }
        }
    ;

pp:
  define
    | undef
    | ifclause
    ;

define:
      DEFINE IDENTIFIER {
            macroTable.insert(QString($2), QString());
        }

undef:
     UNDEF IDENTIFIER {
            macroTable.remove(QString($2));
        }

ifclause:
        ifstart program endif
        | ifstart program else program endif
        ;

ifstart:
       ifStartKeyWord { ++ifNestingLevel; }

ifStartKeyWord:
       ifdef
        | ifndef
        ;

ifdef:
     IFDEF IDENTIFIER {
            if (!m_stripCode && !isDefined($2)) {
                m_stripCode = true;
                nestingLevelToStrip = ifNestingLevel;
            }
        }

ifndef:
      IFNDEF IDENTIFIER {
            if (isDefined($2)) {
                m_stripCode = true;
                nestingLevelToStrip = ifNestingLevel;
            }
        }

else:
    ELSE {
            if (ifNestingLevel - 1 <= nestingLevelToStrip) {
                m_stripCode = !m_stripCode;
            }
        }

endif:
     ENDIF {
            --ifNestingLevel;
            if (ifNestingLevel <= nestingLevelToStrip) {
                m_stripCode = false;
            }
        }

%%

using namespace asl;

bool isDefined(const char *macroName)
{
    return macroTable.contains(QString(macroName));
}

void aslpreprocessorerror(char *msg)
{
    throw CompilationException(CompilationException::PREPROCESSING,
        QString::number(aslpreprocessorlineno) + ": " + QString(msg));
}

void aslPreprocessorReset() {
    m_stripCode = false;
    ifNestingLevel = 0;
    macroTable.clear();
}

QString aslPreprocessorParse(const QString &sourcecode)
{
    aslPreprocessorReset();

    QString out;
    m_outStream.setString(&out, QIODevice::WriteOnly);

    setInput(sourcecode);
    yyparse();

    m_outStream.flush();
    return out;
}

