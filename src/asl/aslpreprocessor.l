
directivePrefix ^[ \t]*#

%x C_COMMENT
%x PP_LINE

%{
    #include "aslpreprocessor_yacc.h"
    #include <QString>

    YY_BUFFER_STATE bufferState;
%}

%%

"/*" {
    BEGIN C_COMMENT;
    aslpreprocessorlval.string = yytext;
    return CHARACTERS;
}
<C_COMMENT>"*/" {
    BEGIN INITIAL;
    aslpreprocessorlval.string = yytext;
    return CHARACTERS;
}
<C_COMMENT>.|\n {
    aslpreprocessorlval.string = yytext;
    return CHARACTERS;
}

"//".* {
    aslpreprocessorlval.string = yytext;
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
    return ELSE;
}

<INITIAL>{directivePrefix}endif {
    return ENDIF;
}

<PP_LINE>[_a-zA-Z][_a-zA-Z0-9]* {
    aslpreprocessorlval.string = yytext;
    return IDENTIFIER;
}

<PP_LINE>[-+]?[0-9]+ {
    aslpreprocessorlval.integer = strtol(yytext, NULL, 0);
    return INTEGER;
}

<PP_LINE>\n {
    BEGIN INITIAL;
}

.|\n {
    aslpreprocessorlval.string = yytext;
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

