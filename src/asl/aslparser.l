
%option yylineno

whitespace [ \t\r\n]
annotationWhitespace [ \t\r\n*]
notAnnotationWhitespace [^ \t\r\n*]
identifier [_a-zA-Z][_a-zA-Z0-9]*
integer ([0-9]+|0[0-7]+|0x[0-9a-fA-F]+)
float [0-9]*"."[0-9]*([eE]-?[0-9]+)?
bool true|false

%x ANNOTATION
%x READ_ANNOTATION_STRING
%x READ_ANNOTATION_GLVARIANT
%x READ_UNIFORM
%x READ_LINE

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
        bool readFirstAslComment;

        unsigned int calcIndentationDepth(const char *str, unsigned int len);
        void setStateFromKey(const QString &key);

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

<ANNOTATION,READ_ANNOTATION_STRING,READ_ANNOTATION_GLVARIANT>"*/" {
    if (readFirstAslComment) {
        BEGIN READ_UNIFORM;
    } else {
        BEGIN INITIAL;
        readFirstAslComment = true;
    }
    return ANNOTATION_END;
}

<ANNOTATION>[-_a-zA-Z0-9]+: {
    yytext[yyleng - 1] = '\0'; // We don't want to return the colon.
    aslparserlval.string = new QString(yytext);
    setStateFromKey(*aslparserlval.string);
    return KEY;
}

<READ_ANNOTATION_STRING>{notAnnotationWhitespace}([^\n*/\/]*{notAnnotationWhitespace})? {
    aslparserlval.string = new QString(yytext);
    return ANNOTATION_STRING;
}

<ANNOTATION>^({annotationWhitespace}*{whitespace})? {
    currentIdentation = calcIndentationDepth(yytext, yyleng);
}

<READ_ANNOTATION_STRING,READ_ANNOTATION_GLVARIANT>^({annotationWhitespace}*{whitespace})? {
    const unsigned int identation = calcIndentationDepth(yytext, yyleng);
    if (identation <= currentIdentation) {
        currentIdentation = identation;
        BEGIN ANNOTATION;
    }
}

<READ_UNIFORM>"uniform" {
    return UNIFORM;
}

<READ_UNIFORM>{identifier} {
    aslparserlval.string = new QString(yytext);
    return IDENTIFIER;
}

<READ_UNIFORM>";" {
    BEGIN INITIAL;
    return yytext[0];
}

<READ_LINE,READ_ANNOTATION_GLVARIANT>{integer} {
    aslparserlval.integer = strtoul(yytext, NULL, 0);
    return INTEGER;
}

<READ_ANNOTATION_GLVARIANT>{float} {
    /* Do not use strtof() function as it interprets the locale and may treat
     * other characters than the dot '.' as decimal dot. */
    aslparserlval.flt = QString(yytext).toFloat();
    return FLT;
}

<READ_ANNOTATION_GLVARIANT>{bool} {
    if ("true" == QString(yytext)) {
        aslparserlval.integer = 1;
    } else {
        aslparserlval.integer = 0;
    }
    return BOOLEAN;
}

<READ_ANNOTATION_GLVARIANT>"-" {
    return NEGATE;
}

<INITIAL,READ_UNIFORM>"/*"([^*]("/"?[^*/]*("*"[^/])?)+)?"*/" { }
<INITIAL,READ_UNIFORM>"//".*\n { }

<INITIAL>^[ \t]*#line {
    BEGIN READ_LINE;
    return LINE;
}
<READ_LINE>. { }
<READ_LINE>\n {
    BEGIN INITIAL;
    return END;
}


<INITIAL>^[ \t]*#(.|\\\n)* {
    if (QString(yytext).trimmed().startsWith("#line")) {
        REJECT;
    }
}

<INITIAL>[^ \t\r\n] {
    if (!readFirstAslComment) {
        return UNEXPECTED_CHAR;
    }
}

<INITIAL,READ_UNIFORM,ANNOTATION,READ_ANNOTATION_STRING,READ_ANNOTATION_GLVARIANT>.|\n { }

%%

int yywrap(void)
{
    return 1;
}

namespace asl
{
namespace parserinternal
{

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

void setStateFromKey(const QString &key) {
    if ("Default" == key) {
        BEGIN READ_ANNOTATION_GLVARIANT;
    } else {
        BEGIN READ_ANNOTATION_STRING;
    }
}

void reset() {
    yylineno = 1;
    sourceStringNo = 0;
    readFirstAslComment = false;
    BEGIN INITIAL;
}

void setInput(const QString &sourcecode)
{
    reset();
    yy_scan_string(sourcecode.toAscii().constData());
}

} /* namespace parserinternal */
} /* namespace asl */

