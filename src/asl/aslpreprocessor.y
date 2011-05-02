
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

    QTextStream outStream;

    void pushInput(const QString &sourcecode);
    void reset();

    extern int aslpreprocessorlineno;

    bool isDefined(const char *macroName);

    void aslpreprocessorerror(char *msg);
    int yylex(void);

    /* Macro replacment stuff. */
    typedef struct macroPart_t {
        bool isParameter;
        QString text;
    } macroPart;

    typedef struct macro_t {
        QStringList *argumentList;
        QList<macroPart> parts;
    } macro;

    QHash<QString, macro> macroTable;

    void expandMacro(const QString &macroName);
%}

%union {
    long int integer;
    const char *string;
    QStringList *parsed;
    QStringList *argumentList;
    QList<struct macroPart_t> *macroParts;
}

%token <string> CHARACTERS
%token DEFINE ENDPP UNDEF IF ELIF IFDEF IFNDEF ELSE ENDIF
%token <string> IDENTIFIER
%token <integer> INTEGER

%type <integer> expr
%type <integer> ifstart if ifdef ifndef elif
%type <argumentList> optargs arglist
%type <macroParts> macrodef
%type <parsed> part stmt pp
%type <parsed> ifclause elseclause

%left AND OR
%left '&' '^' '|'
%left EQ NE
%left '<' '>' LE GE
%left SL SR
%left '+' '-'
%left '*' '/' '%'
%nonassoc '!' UPLUS UMINUS '~' DEFINED

%%

program: part { outStream << $1->join(""); delete $1; }

part:
    part stmt { $$ = $1; $$->append(*$2); delete $2; }
    | { $$ = new QStringList(); }
    ;

stmt:
    pp { $$ = $1; }
    | CHARACTERS { $$ = new QStringList(QString($1)); delete[] $1; }
    ;

pp:
    define { $$ = new QStringList(); }
    | undef { $$ = new QStringList(); }
    | ifclause { $$ = $1; }
    ;

expr:
    INTEGER { $$ = $1; }
    | '+' expr %prec UPLUS { $$ = +$2; }
    | '-' expr %prec UMINUS { $$ = -$2; }
    | '~' expr { $$ = ~$2; }
    | '!' expr { $$ = !$2; }
    | DEFINED IDENTIFIER { $$ = isDefined($2); delete[] $2; }
    | expr EQ expr { $$ = $1 == $3; }
    | expr NE expr { $$ = $1 != $3; }
    | expr AND expr { $$ = $1 && $3; }
    | expr OR expr { $$ = $1 || $3; }
    | expr '<' expr { $$ = $1 < $3; }
    | expr '>' expr { $$ = $1 > $3; }
    | expr LE expr { $$ = $1 <= $3; }
    | expr GE expr { $$ = $1 >= $3; }
    | expr '&' expr { $$ = $1 & $3; }
    | expr '^' expr { $$ = $1 ^ $3; }
    | expr '|' expr { $$ = $1 | $3; }
    | expr SL expr { $$ = $1 << $3; }
    | expr SR expr { $$ = $1 >> $3; }
    | expr '+' expr { $$ = $1 + $3; }
    | expr '-' expr { $$ = $1 - $3; }
    | expr '*' expr { $$ = $1 * $3; }
    | expr '/' expr { $$ = $1 / $3; }
    | expr '%' expr { $$ = $1 % $3; }
    | '(' expr ')' { $$ = $2; }
    ;

define:
    DEFINE IDENTIFIER optargs macrodef ENDPP {
            macro m;
            m.argumentList = $3;
            m.parts = *$4;
            macroTable.insert(QString($2), m);
            delete[] $2;
            delete $4;
        }
    ;

optargs:
    '(' arglist ')' { $$ = $2; }
    | { $$ = NULL; }
    ;

arglist:
    { $$ = new QStringList(); }
    | arglist ',' IDENTIFIER {
            $$ = $1;
            $$->append($3);
            delete[] $3;
        }
    | IDENTIFIER { $$ = new QStringList(QString($1)); delete $1; }
    ;

macrodef:
    macrodef CHARACTERS {
            $$ = $1;
            if ($$->isEmpty() || $$->back().isParameter) {
                macroPart part;
                part.isParameter = false;
                part.text = $2;
                $$->append(part);
            } else {
                $$->back().text += $2;
            }
            delete $2;
        }
    | { $$ = new QList<struct macroPart_t>(); };

undef: UNDEF IDENTIFIER ENDPP { macroTable.remove(QString($2)); delete[] $2; };

ifclause: ifstart part elseclause ENDIF ENDPP {
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
    ELSE ENDPP part { $$ = $3; }
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

if: IF expr ENDPP { $$ = $2; };

ifdef: IFDEF IDENTIFIER ENDPP { $$ = isDefined($2); delete[] $2; };

ifndef: IFNDEF IDENTIFIER ENDPP { $$ = !isDefined($2); delete[] $2; };

elif: ELIF expr ENDPP { $$ = $2; };

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

void aslPreprocessorReset()
{
    foreach (macro m, macroTable) {
        if (m.argumentList) {
            delete m.argumentList;
        }
    }
    macroTable.clear();
    reset();
}

QString aslPreprocessorParse(const QString &sourcecode)
{
    aslPreprocessorReset();

    QString out;
    outStream.setString(&out, QIODevice::WriteOnly);

    pushInput(sourcecode);
    yyparse();

    outStream.flush();
    return out;
}

