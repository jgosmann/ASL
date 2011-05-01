
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

    void setInput(const QString &sourcecode);

    extern int aslpreprocessorlineno;

    bool isDefined(const char *macroName);

    void aslpreprocessorerror(char *msg);
    int yylex(void);

    /* Macro replacment stuff. */
    typedef enum { CONST_NODE, IDENTIFIER_NODE, OPERATOR_NODE } nodeType;

    typedef struct {
        long int value;
    } constNode;

    typedef struct {
        const char *identifier;
    } identifierNode;

    typedef struct {
        int op;
        unsigned int operandCount;
        struct node_t *operands[2];
    } operatorNode;

    typedef struct node_t {
        nodeType type;
        union {
            constNode asConst;
            identifierNode asIdentifier;
            operatorNode asOperator;
        };
    } node;

    QHash<QString, node *> macroTable;

    long int evalMacro(const char *macroName);
%}

%union {
    long int integer;
    const char *string;
    struct node_t *macro;
    QStringList *parsed;
}

%token <string> CHARACTERS
%token DEFINE ENDPP UNDEF IF ELIF IFDEF IFNDEF ELSE ENDIF
%token <string> IDENTIFIER
%token <integer> INTEGER

%type <integer> expr
%type <integer> ifstart if ifdef ifndef elif
%type <macro> macrodef
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
    | IDENTIFIER { $$ = evalMacro($1); delete[] $1; }
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
    DEFINE IDENTIFIER ENDPP {
            node *n = new node;
            n->type = CONST_NODE;
            macroTable.insert(QString($2), n);
            delete[] $2;
        }
    | DEFINE IDENTIFIER macrodef ENDPP {
            macroTable.insert(QString($2), $3);
            delete[] $2;
        }

macrodef:
    expr {
            $$ = new node;
            $$->type = CONST_NODE;
            $$->asConst.value = $1;
        }

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

long int evalMacroNode(node *macroNode)
{
    switch (macroNode->type) {
        case CONST_NODE:
            return macroNode->asConst.value;
    }

    throw CompilationException(CompilationException::PREPROCESSING,
        QString::number(aslpreprocessorlineno) + ": "
        + "Macro could not be evaluated. Probably the preprocessor "
        + "implementation is incomplete.");
}

long int evalMacro(const char *macroName)
{
    const QString nameAsQString(macroName);
    if (!macroTable.contains(nameAsQString)) {
        throw new CompilationException(CompilationException::PREPROCESSING,
            QString::number(aslpreprocessorlineno) + ": "
            + "Undefined macro " + nameAsQString + ".");
    }

    return evalMacroNode(macroTable.value(nameAsQString));
}

void aslpreprocessorerror(char *msg)
{
    throw CompilationException(CompilationException::PREPROCESSING,
        QString::number(aslpreprocessorlineno) + ": " + QString(msg));
}

void deleteMacroNode(node *n)
{
    if (n->type == OPERATOR_NODE) {
        for (unsigned int i = 0; i < n->asOperator.operandCount; ++i) {
            deleteMacroNode(n->asOperator.operands[i]);
        }
    }
    delete n;
}

void deleteMacroNodes()
{
    foreach (node *n, macroTable) {
        delete n;
    }
}

void aslPreprocessorReset()
{
    deleteMacroNodes();
    macroTable.clear();
}

QString aslPreprocessorParse(const QString &sourcecode)
{
    aslPreprocessorReset();

    QString out;
    outStream.setString(&out, QIODevice::WriteOnly);

    setInput(sourcecode);
    yyparse();

    outStream.flush();
    return out;
}

