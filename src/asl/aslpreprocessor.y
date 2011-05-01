
%{
    #include <QHash>
    #include <QIODevice>
    #include <QString>
    #include <QStringList>
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
    long int integer;
    const char *string;
    QStringList *parsed;
}

%token <string> CHARACTERS
%token DEFINE UNDEF IF ELIF IFDEF IFNDEF ELSE ENDIF
%token <string> IDENTIFIER
%token <integer> INTEGER

%type <integer> expr
%type <integer> ifstart if ifdef ifndef elif
%type <parsed> part stmt pp
%type <parsed> ifclause elseclause

%left AND OR
%left EQ NE
%left '<' '>' LE GE
%nonassoc '!'

%%

program: part { m_outStream << $1->join(""); delete $1; }

part:
    part stmt { $$ = $1; $$->append(*$2); delete $2; }
    | { $$ = new QStringList(); }
    ;

stmt:
    pp { $$ = $1; }
    | CHARACTERS { $$ = new QStringList(QString($1)); }
    ;

pp:
    define { $$ = new QStringList(); }
    | undef { $$ = new QStringList(); }
    | ifclause { $$ = $1; }
    ;

expr:
    INTEGER { $$ = $1; }
    | '!' expr { $$ = !$2; }
    | expr EQ expr { $$ = $1 == $3; }
    | expr NE expr { $$ = $1 != $3; }
    | expr AND expr { $$ = $1 && $3; }
    | expr OR expr { $$ = $1 || $3; }
    | expr '<' expr { $$ = $1 < $3; }
    | expr '>' expr { $$ = $1 > $3; }
    | expr LE expr { $$ = $1 <= $3; }
    | expr GE expr { $$ = $1 >= $3; }
    | '(' expr ')' { $$ = $2; }
    ;

define: DEFINE IDENTIFIER { macroTable.insert(QString($2), QString()); };

undef: UNDEF IDENTIFIER { macroTable.remove(QString($2)); };

ifclause: ifstart part elseclause ENDIF {
        if ($1 != 0) {
            $$ = $2;
            delete $3;
        } else {
            $$ = $3;
            delete $2;
        } };

ifstart:
      if
      | ifdef
      | ifndef
      ;

elseclause:
    ELSE part { $$ = $2; }
    | elif part elseclause {
        if ($1 != 0) {
            $$ = $2;
            delete $3;
        } else {
            $$ = $3;
            delete $2;
        } }
    | { $$ = new QStringList(); }
    ;

if: IF expr { $$ = $2; };

ifdef: IFDEF IDENTIFIER { $$ = isDefined($2); };

ifndef: IFNDEF IDENTIFIER { $$ = !isDefined($2); };

elif: ELIF expr { $$ = $2; };

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

