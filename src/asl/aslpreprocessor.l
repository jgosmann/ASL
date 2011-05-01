
directivePrefix ^[ \t]*#

%x C_COMMENT
%x PP_LINE

%{
    #include <QString>
    #include <QStringList>
    #include "aslpreprocessor_yacc.h"

    YY_BUFFER_STATE bufferState;

    char *copyStrAndNullTerminate(const char *str, unsigned int len);
%}

%%

"/*" {
    BEGIN C_COMMENT;
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    return CHARACTERS;
}
<C_COMMENT>"*/" {
    BEGIN INITIAL;
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    return CHARACTERS;
}
<C_COMMENT>.|\n {
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    return CHARACTERS;
}

"//".* {
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    return CHARACTERS;
}

<INITIAL>{directivePrefix}define {
    BEGIN PP_LINE;
    return DEFINE;
}

<INITIAL>{directivePrefix}undef {
    BEGIN PP_LINE;
    return UNDEF;
}

<INITIAL>{directivePrefix}if {
    BEGIN PP_LINE;
    return IF;
}

<INITIAL>{directivePrefix}elif {
    BEGIN PP_LINE;
    return ELIF;
}

<INITIAL>{directivePrefix}ifdef {
    BEGIN PP_LINE;
    return IFDEF;
}

<INITIAL>{directivePrefix}ifndef {
    BEGIN PP_LINE;
    return IFNDEF;
}

<INITIAL>{directivePrefix}else {
    BEGIN PP_LINE;
    return ELSE;
}

<INITIAL>{directivePrefix}endif {
    BEGIN PP_LINE;
    return ENDIF;
}

<PP_LINE>[-+*/%()<>!&^|~] { return *yytext; }
<PP_LINE>"==" { return EQ; }
<PP_LINE>"!=" { return NE; }
<PP_LINE>"||" { return OR; }
<PP_LINE>"&&" { return AND; }
<PP_LINE>"<=" { return LE; }
<PP_LINE>">=" { return GE; }
<PP_LINE>"<<" { return SL; }
<PP_LINE>">>" { return SR; }
<PP_LINE>"defined" { return DEFINED; }

<PP_LINE>[_a-zA-Z][_a-zA-Z0-9]+ {
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    return IDENTIFIER;
}

<PP_LINE>([0-9]+|0[0-7]+|0x[0-9a-fA-F]+) {
    aslpreprocessorlval.integer = strtoul(yytext, NULL, 0);
    return INTEGER;
}

<PP_LINE>\n {
    BEGIN INITIAL;
    return ENDPP;
}

.|\n {
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    return CHARACTERS;
}

%%

int yywrap(void)
{
    yy_delete_buffer(bufferState);
    return 1;
}

void setInput(const QString &sourcecode)
{
    bufferState = yy_scan_string(sourcecode.toAscii());
}

char *copyStrAndNullTerminate(const char *str, unsigned int len)
{
    char *copy = new char[len + 1];
    strncpy(copy, str, len + 1);
    return copy;
}

