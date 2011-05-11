
%{
    #ifdef TEST
        #include "../src/asl/aslparser_internal.h"
        #include "../src/asl/annotatedglshaderprogrambuilder.h"
    #else
        #include "asl/aslparser_internal.h"
        #include "asl/annotatedglshaderprogrambuilder.h"
    #endif

    #include <QFileInfo>
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
        AnnotatedGLShaderProgramBuilder builder;
    } /* namespace parserinternal */
    } /* namespace asl */

%}

%union {
    long int integer;
    QString *string;
}


%token <string> ANNOTATION_STRING
%token WHITESPACE ANNOTATION_START ANNOTATION_END CHARACTERS KEY

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

keyValuePair: KEY ANNOTATION_STRING { builder.setName(*$2); delete $2; };

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

AnnotatedGLShaderProgram * parse(const QString &sourcecode,
        const QString &pathOfSource)
{
    builder = AnnotatedGLShaderProgramBuilder();
    builder.setName(QFileInfo(pathOfSource).fileName());
    setInput(sourcecode);
    yyparse();
    return builder.build();
}

} /* namespace parserinternal */
} /* namespace asl */

