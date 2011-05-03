
%{
    #ifdef TEST
        #include "../src/asl/aslpreprocessor_parserinternal.h"
    #else
        #include "asl/aslpreprocessor_parserinternal.h"
    #endif

    void yyerror(char *msg);
    int yylex();

    using namespace asl::ppinternal;

    QTextStream *outStream;
    QHash<QString, Macro> asl::ppinternal::macroTable;

%}

%union {
    long int integer;
    QString *string;
    QStringList *parsed;
    QStringList *argumentList;
    QList<struct macroPart_t> *macroParts;
}

%type <integer> expr
%type <integer> ifstart if ifdef ifndef elif
%type <argumentList> optargs arglist
%type <macroParts> macrodef
%type <parsed> part stmt pp
%type <parsed> ifclause elseclause

/* Do not reorder the tokens! The order matters! */
%token <string> CHARACTERS
%token DEFINE ENDPP UNDEF IF ELIF IFDEF IFNDEF ELSE ENDIF
%token <string> IDENTIFIER
%token <integer> INTEGER

%left AND OR
%left '&' '^' '|'
%left EQ NE
%left '<' '>' LE GE
%left SL SR
%left '+' '-'
%left '*' '/' '%'
%nonassoc '!' UPLUS UMINUS '~' DEFINED

%%

program: part { (*outStream) << $1->join(""); delete $1; }

part:
    part stmt { $$ = $1; $$->append(*$2); delete $2; }
    | { $$ = new QStringList(); }
    ;

stmt:
    pp { $$ = $1; }
    | CHARACTERS { $$ = new QStringList(*$1); delete $1; }
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
    | DEFINED IDENTIFIER { $$ = isDefined(*$2); delete $2; }
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
            Macro m;
            m.arguments = QSharedPointer<QStringList>($3);
            m.parts = QSharedPointer<QList<MacroPart> >($4);
            macroTable.insert(*$2, m);
            delete $2;
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
            $$->append(*$3);
            delete $3;
        }
    | IDENTIFIER { $$ = new QStringList(*$1); delete $1; }
    ;

macrodef:
    macrodef CHARACTERS {
            $$ = $1;
            if ($$->isEmpty() || $$->back().isArgument) {
                MacroPart part;
                part.isArgument = false;
                part.text = *$2;
                $$->append(part);
            } else {
                $$->back().text += *$2;
            }
            delete $2;
        }
    | { $$ = new QList<struct macroPart_t>(); };

undef: UNDEF IDENTIFIER ENDPP { macroTable.remove(*$2); delete $2; };

ifclause: ifstart part elseclause ENDIF ENDPP {
        if ($1 != 0) {
            $$ = $2;
            delete $3;
        } else {
            $$ = $3;
            delete $2;
        }
    };

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

ifdef: IFDEF IDENTIFIER ENDPP { $$ = isDefined(*$2); delete $2; };

ifndef: IFNDEF IDENTIFIER ENDPP { $$ = !isDefined(*$2); delete $2; };

elif: ELIF expr ENDPP { $$ = $2; };

%%

using namespace asl::ppinternal;

void yyerror(char *msg)
{
    throw asl::CompilationException(asl::CompilationException::PREPROCESSING,
        QString(msg), aslpreprocessorlineno);
}

namespace asl
{
namespace ppinternal
{

bool isDefined(const QString &macroName)
{
    return macroTable.contains(macroName);
}

void parse(const QString &sourcecode, QTextStream *out)
{
    outStream = out;
    pushInput(sourcecode);
    yyparse();
    outStream->flush();
}

} /* namespace ppinternal */
} /* namespace asl */

