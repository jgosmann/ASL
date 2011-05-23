
%{
    #ifdef TEST
        #include "../src/asl/aslparser_internal.h"
        #include "../src/asl/annotatedglshaderprogrambuilder.h"
        #include "../src/asl/shaderparameterinfobuilder.h"
    #else
        #include "asl/aslparser_internal.h"
        #include "asl/annotatedglshaderprogrambuilder.h"
        #include "asl/shaderparameterinfobuilder.h"
    #endif

    #include <QFileInfo>
    #include <QSet>
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
        AnnotatedGLShaderProgramBuilder programBuilder;
        ShaderParameterInfoBuilder parameterInfoBuilder;

        QSet<QString> definedKeys;

        void addToLog(const QString &type, const QString &msg);
        void handleKeyStringValuePair(QString *key, QString *value);
        void warn(const QString &msg);
    } /* namespace parserinternal */
    } /* namespace asl */

%}

%union {
    long int integer;
    QString *string;
}

%type <string> string

%destructor { delete $$; } string

%token <string> KEY IDENTIFIER ANNOTATION_STRING
%token ANNOTATION_START ANNOTATION_END UNIFORM UNEXPECTED_CHAR

%%

program:
    leadingChars
    | annotationComment remainingProgram
    | leadingChars annotationComment remainingProgram {
            warn("ASL program is not starting with ASL comment.");
        }
    | ;

leadingChars:
    leadingChars UNEXPECTED_CHAR
    | ;

remainingProgram:
    remainingProgram parameter
    | ;

parameter: annotationComment UNIFORM datatype IDENTIFIER ';' {
            parameterInfoBuilder.withIdentifier(*$4);
            try {
                programBuilder.addParameter(parameterInfoBuilder.build());
            } catch (const common::NoValueException &e) {
                warn(e.what());
            }
            delete $4;
        }

datatype: IDENTIFIER {
            try {
                parameterInfoBuilder.withType(asl::GLTypeInfo::getFor(*$1));
            } catch (const std::invalid_argument &e) {
                warn(e.what());
            }
            delete $1;
        };

annotationComment: ANNOTATION_START {
            definedKeys.clear();
            parameterInfoBuilder.reset();
         } annotations ANNOTATION_END {}

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


%%

using namespace asl::parserinternal;

void yyerror(const char *msg)
{
    addToLog("ERROR", msg);
}

namespace asl
{
namespace parserinternal
{

void addToLog(const QString &type, const QString &msg)
{
    log = log % type % ": " % "0" % /*QString::number(sourceStringNo) %*/ QChar(':')
            % QString::number(aslparserlineno)
            % ": (aslcompiler) " % QString(msg) % QChar('\n');
}

void clearLog()
{
    log.clear();
}

void handleKeyStringValuePair(QString *key, QString *value)
{
    if (definedKeys.contains(*key)) {
        --aslparserlineno;
        warn("duplicate key: " + *key);
        ++aslparserlineno;
    } else {
        definedKeys.insert(*key);
    }
    if ("ShaderName" == *key) {
        programBuilder.setName(*value);
    } else if ("ShaderDescription" == *key) {
        programBuilder.setDescription(*value);
    } else if ("Name" == *key) {
        parameterInfoBuilder.withName(*value);
    } else {
        warn("unknown key: " + *key);
    }

    delete key;
    delete value;
}

AnnotatedGLShaderProgram * parse(const QString &sourcecode,
        const QString &pathOfSource)
{
    programBuilder = AnnotatedGLShaderProgramBuilder();
    programBuilder.setName(QFileInfo(pathOfSource).fileName());
    setInput(sourcecode);
    yyparse();
    return programBuilder.build();
}

void warn(const QString &msg)
{
    addToLog("WARNING", msg);
}

} /* namespace parserinternal */
} /* namespace asl */

