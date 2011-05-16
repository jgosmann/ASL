
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

    void aslparsererror(const char *msg);

    using namespace asl::parserinternal;

    namespace asl
    {
    namespace parserinternal
    {
        void clean();
    } /* namespace ppinternal */
    } /* namespace asl */
%}

%%

"/*" {
    BEGIN ANNOTATION;
    return ANNOTATION_START;
}

<ANNOTATION,READ_ANNOTATION_STRING>"*/" {
    BEGIN INITIAL;
    return ANNOTATION_END;
}

<ANNOTATION>[-_a-zA-Z0-9]+: {
    aslparserlval.string = new QString(yytext);
    BEGIN READ_ANNOTATION_STRING;
    return KEY;
}

<READ_ANNOTATION_STRING>{notAnnotationWhitespace}([^*/\/]*{notAnnotationWhitespace})? {
    aslparserlval.string = new QString(yytext);
    return ANNOTATION_STRING;
}

<ANNOTATION>. {
}

{whitespace}+ { return WHITESPACE; }

. { return CHARACTERS; }

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

