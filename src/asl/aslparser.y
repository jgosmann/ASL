
%{
    #ifdef TEST
        #include "../src/asl/aslparser_internal.h"
    #else
        #include "asl/aslparser_internal.h"
    #endif

    #include <QStringBuilder>

    extern int aslparserlineno;
    void yyerror(const char *msg);
    int yylex();

    using namespace asl::parserinternal;

    namespace asl
    {
    namespace parserinternal
    {
        QString log;
    } /* namespace parserinternal */
    } /* namespace asl */

%}

%union {
    long int integer;
    QString *string;
}

%token WHITESPACE ANNOTATION_START ANNOTATION_STRING ANNOTATION_END ANNOTATION_WHITESPACE CHARACTERS

%%

program:
    remainingProgram
    | header annotationComment remainingProgram;

header:
    header WHITESPACE
    | ;

annotationComment: ANNOTATION_START annotations ANNOTATION_END;

annotations:
    annotations keyValuePair
    | ;

keyValuePair: KEY value;

value: string;

string: WHITESPACE ANNOTATION_STRING ANNOTATION_WHITESPACE;

remainingProgram:
    remainingProgram WHITESPACE
    | remainingProgram CHARACTERS
    | ;

%%

using namespace asl::parserinternal;

void yyerror(const char *msg)
{
    log = log % /*QString::number(sourceStringNo) %*/ QChar(':')
            % QString::number(aslparserlineno)
            % ": error: (aslcompiler) " % QString(msg) % QChar('\n');
}

namespace asl
{
namespace parserinternal
{

void clearLog()
{
    log.clear();
}

void parse(const QString &sourcecode)
{
    setInput(sourcecode);
    yyparse();
}

} /* namespace parserinternal */
} /* namespace asl */

