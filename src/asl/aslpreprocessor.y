
%{
    #ifdef TEST
        #include "../src/asl/aslpreprocessor_parserinternal.h"
    #else
        #include "asl/aslpreprocessor_parserinternal.h"
    #endif

    #include <QStack>
    #include <QStringBuilder>

    void yyerror(const char *msg);
    int yylex();

    using namespace asl::ppinternal;

    namespace asl
    {
    namespace ppinternal
    {
        unsigned int sourceStringNo;
        unsigned int glslVersion = 0;

        QTextStream *outStream;
        QString log;
        bool success;
        int ifNestingLevel;
        int excludeIfNestingLevel;
        QStack<bool> ifConditionResults;
        QHash<QString, Macro> macroTable;

        void handleIfBlockStart(bool isTrue);
        void handleIfBlockEnd();
    } /* namespace ppinternal */
    } /* namespace asl */

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
%type <string> errormsg

/* Do not reorder the tokens! The order matters! */
%token <string> CHARACTERS
%token DEFINE ENDPP UNDEF IF ELIF IFDEF IFNDEF ELSE ENDIF ERROR LINE
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

%destructor { delete $$; } optargs arglist macrodef part stmt pp ifclause elseclause CHARACTERS IDENTIFIER

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
    | ERROR errormsg ENDPP {
            $$ = new QStringList();
            --aslpreprocessorlineno; // because of ENDPP we are already in the
                                     // next line.
            yyerror($2->toAscii().data());
            ++aslpreprocessorlineno;
            delete $2;
        }
    | LINE INTEGER ENDPP {
            aslpreprocessorlineno = $2 + 1;
            $$ = new QStringList();
        }
    | LINE INTEGER INTEGER ENDPP {
            aslpreprocessorlineno = $2 + 1;
            sourceStringNo = $3;
            $$ = new QStringList();
        }
    | error ENDPP { $$ = new QStringList(); }
    ;

errormsg:
    errormsg CHARACTERS { $$ = $1; $$->append($2); delete $2; }
    | { $$ = new QString(); }
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
    macrodef IDENTIFIER {
            $$ = $1;
            MacroPart part;
            part.isArgument = true;
            part.text = *$2;
            $$->append(part);
            delete $2;
        }
    | macrodef CHARACTERS {
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

ifclause: ifstart { ++ifNestingLevel; ifConditionResults.push($1 != 0); handleIfBlockStart($1 != 0); } part { handleIfBlockEnd(); } elseclause ENDIF { ifConditionResults.pop(); --ifNestingLevel; } ENDPP {
        if ($1 != 0) {
            $$ = $3;
            delete $5;
        } else {
            $$ = $5;
            delete $3;
        }
    };

ifstart:
      if
      | ifdef
      | ifndef
      ;

elseclause:
    ELSE ENDPP { handleIfBlockStart(!ifConditionResults.top()); } part { handleIfBlockEnd(); $$ = $4; }
    | elif { handleIfBlockStart(!ifConditionResults.top() && $1 != 0); ifConditionResults.top() = ($1 != 0); } part { handleIfBlockEnd(); } elseclause {
        if ($1 != 0) {
            $$ = $3;
            delete $5;
        } else {
            $$ = $5;
            delete $3;
        } }
    | { $$ = new QStringList(); }
    ;

if: IF expr ENDPP { $$ = $2; };

ifdef: IFDEF IDENTIFIER ENDPP { $$ = isDefined(*$2); delete $2; };

ifndef: IFNDEF IDENTIFIER ENDPP { $$ = !isDefined(*$2); delete $2; };

elif: ELIF expr ENDPP { $$ = $2; };

%%

using namespace asl::ppinternal;

void yyerror(const char *msg)
{
    success = false;
    log = log % QString::number(sourceStringNo) % QChar(':')
            % QString::number(aslpreprocessorlineno)
            % ": error: (preprocessor) " % QString(msg) % QChar('\n');
}

namespace asl
{
namespace ppinternal
{

void clearLog()
{
    log.clear();
}

bool isDefined(const QString &macroName)
{
    return macroTable.contains(macroName);
}

void handleIfBlockStart(bool isTrue)
{
    if (!isTrue && excludeIfNestingLevel == 0) {
        excludeIfNestingLevel = ifNestingLevel;
    }
}

void handleIfBlockEnd()
{
    if (ifNestingLevel == excludeIfNestingLevel) {
        excludeIfNestingLevel = 0;
    }
}

void parse(const QString &sourcecode, QTextStream *out)
{
    success = true;
    outStream = out;
    pushInput(sourcecode);
    yyparse();
    outStream->flush();
}

} /* namespace ppinternal */
} /* namespace asl */

