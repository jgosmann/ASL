
%option yylineno

whitespace [ \t\r\n]
annotationWhitespace [ \t\r\n*]
notAnnotationWhitespace [^ \t\r\n*]
identifier [_a-zA-Z][_a-zA-Z0-9]*

%x ANNOTATION
%x READ_ANNOTATION_STRING

%{
    #ifdef TEST
        #include "../src/asl/aslparser_internal.h"
    #else
        #include "asl/aslparser_internal.h"
    #endif

    #include "aslparser_yacc.h"

    #include <climits>

    void aslparsererror(const char *msg);

    using namespace asl::parserinternal;

    namespace asl
    {
    namespace parserinternal
    {
        void clean();

        unsigned int calcIndentationDepth(const char *str, unsigned int len);

        unsigned int currentIdentation;
    } /* namespace ppinternal */
    } /* namespace asl */
%}

%%

"/**" {
    currentIdentation = 0;
    BEGIN ANNOTATION;
    return ANNOTATION_START;
}

<ANNOTATION,READ_ANNOTATION_STRING>"*/" {
    BEGIN INITIAL;
    return ANNOTATION_END;
}

<ANNOTATION>[-_a-zA-Z0-9]+: {
    yytext[yyleng - 1] = '\0'; // We don't want to return the colon.
    aslparserlval.string = new QString(yytext);
    BEGIN READ_ANNOTATION_STRING;
    return KEY;
}

<READ_ANNOTATION_STRING>{notAnnotationWhitespace}([^\n*/\/]*{notAnnotationWhitespace})? {
    aslparserlval.string = new QString(yytext);
    return ANNOTATION_STRING;
}

<ANNOTATION>^({annotationWhitespace}*{whitespace})? {
    currentIdentation = calcIndentationDepth(yytext, yyleng);
}

<READ_ANNOTATION_STRING>^({annotationWhitespace}*{whitespace})? {
    const unsigned int identation = calcIndentationDepth(yytext, yyleng);
    if (identation <= currentIdentation) {
        currentIdentation = identation;
        BEGIN ANNOTATION;
    }
}

<ANNOTATION,READ_ANNOTATION_STRING>.|\n {
}

.|\n { }

%%

int yywrap(void)
{
    resetAndClean();
    return 1;
}

namespace asl
{
namespace parserinternal
{

void clean()
{
}

unsigned int calcIndentationDepth(const char *str, unsigned int len)
{
    unsigned int identation = 0;
    for (unsigned int i = 0; i < len; ++i) {
        if ('\t' == str[i]) {
            identation += 8;
        } else {
            ++identation;
        }
    }
    return identation;
}

void resetAndClean() {
    clean();
    yylineno = 1;
    BEGIN INITIAL;
}

void setInput(const QString &sourcecode)
{
    yy_scan_string(sourcecode.toAscii().constData());
}

} /* namespace parserinternal */
} /* namespace asl */

