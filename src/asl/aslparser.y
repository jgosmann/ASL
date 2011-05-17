
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

        void handleKeyStringValuePair(QString *key, QString *value);
    } /* namespace parserinternal */
    } /* namespace asl */

%}

%union {
    long int integer;
    QString *string;
}

%type <string> string

%token <string> KEY ANNOTATION_STRING
%token WHITESPACE ANNOTATION_START ANNOTATION_END CHARACTERS

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

keyValuePair: KEY string { handleKeyStringValuePair($1, $2); };

string:
    string ANNOTATION_STRING {
            $$ = $1;
            $$->append(" ");
            $$->append($2);
            delete $2;
        }
    | ANNOTATION_STRING { $$ = $1; }
    ;

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

void handleKeyStringValuePair(QString *key, QString *value)
{
    if ("ShaderName:" == *key) {
        builder.setName(*value);
    } else if ("ShaderDescription:" == *key) {
        builder.setDescription(*value);
    }

    delete key;
    delete value;
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

