
%option yylineno

whitespace [ \t\r\n]
annotationWhitespace [ \t\r\n*]
identifier [_a-zA-Z][_a-zA-Z0-9]*

%x ANNOTATION

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

<ANNOTATION>{annotationWhitespace}* {
    return ANNOTATION_WHITESPACE;
}

<ANNOTATION>"*/" {
    BEGIN INITIAL;
    return ANNOTATION_END;
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
}

void setInput(const QString &sourcecode)
{
    yy_scan_string(sourcecode.toAscii().constData());
}

} /* namespace parserinternal */
} /* namespace asl */

