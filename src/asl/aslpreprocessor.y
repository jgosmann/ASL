
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
%token DEFINE IFDEF IFNDEF ENDIF
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
    | ifclause
    ;

define:
      DEFINE IDENTIFIER {
            macroTable.insert(QString($2), QString());
        }

ifclause:
        ifstart program endif;

ifstart:
       ifdef
        | ifndef
        ;

ifdef:
     IFDEF IDENTIFIER {
            if (!isDefined($2)) {
                m_stripCode = true;
            }
        }

ifndef:
      IFNDEF IDENTIFIER {
            if (isDefined($2)) {
                m_stripCode = true;
            }
        }

endif:
     ENDIF {
            m_stripCode = false;
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

QString aslPreprocessorParse(const QString &sourcecode)
{
    m_stripCode = false;

    QString out;
    m_outStream.setString(&out, QIODevice::WriteOnly);

    setInput(sourcecode);
    yyparse();

    m_outStream.flush();
    return out;
}

void aslPreprocessorReset() {
    m_stripCode = false;
    macroTable.clear();
}

