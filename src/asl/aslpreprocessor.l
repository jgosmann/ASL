
directivePrefix ^[ \t]*#
identifier [_a-zA-Z][_a-zA-Z0-9]+

%x C_COMMENT
%x PP_LINE
%x DEFINE_DECL
%x MACRO_DEF
%x ARGLIST

%{
    #include <QByteArray>
    #include <QHash>
    #include <QStack>
    #include <QString>
    #include <QStringList>
    #include "aslpreprocessor_yacc.h"

    QStack<QByteArray *> buffers;
    QStringList macroExpansionStack;

    typedef struct macroPart_t {
        bool isParameter;
        QString text;
    } macroPart;

    typedef struct macro_t {
        QHash<QString, int> parameterPositions;
        QList<macroPart> parts;
    } macro;

    extern QHash<QString, macro> macroTable;

    bool doNotMacroExpandNextIdentifer = true;

    char *copyStrAndNullTerminate(const char *str, unsigned int len);
    void expandMacro(const QString &macroName);
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
    BEGIN DEFINE_DECL;
    return DEFINE;
}

<INITIAL>{directivePrefix}undef {
    BEGIN PP_LINE;
    doNotMacroExpandNextIdentifer = true;
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
    doNotMacroExpandNextIdentifer = true;
    return IFDEF;
}

<INITIAL>{directivePrefix}ifndef {
    BEGIN PP_LINE;
    doNotMacroExpandNextIdentifer = true;
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
<PP_LINE>"defined" { doNotMacroExpandNextIdentifer = true; return DEFINED; }

<PP_LINE>{identifier} {
        aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    if (doNotMacroExpandNextIdentifer) {
        doNotMacroExpandNextIdentifer = false;
        return IDENTIFIER;
    } else {
        char next = yyinput();
        unput(next);
        if ('(' == next) {
            unsigned int n = 0;
            char c;
            
            do {
                c = yyinput();
                if ('(' == c) {
                    ++n;
                } else if (')' == c) {
                    --n;
                }
            } while (n > 0);
        }

        expandMacro(QString(aslpreprocessorlval.string));
        delete aslpreprocessorlval.string;
    }
}

<DEFINE_DECL>{identifier} {
    /* yytext has to be copied first, because peeking at the next char will
     * destroy yytext. */
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);

    char next = yyinput();
    unput(next);
    if ('(' != next) {
        BEGIN MACRO_DEF;
    } else {
        BEGIN ARGLIST;
    }

    return IDENTIFIER;
}

<ARGLIST>[(,] { return *yytext; }
<ARGLIST>")" {
    BEGIN MACRO_DEF;
    return *yytext;
}
<ARGLIST>{identifier} {
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    return IDENTIFIER;
}

<PP_LINE>([0-9]+|0[0-7]+|0x[0-9a-fA-F]+) {
    aslpreprocessorlval.integer = strtoul(yytext, NULL, 0);
    return INTEGER;
}

<MACRO_DEF>. {
    aslpreprocessorlval.string = copyStrAndNullTerminate(yytext, yyleng);
    return CHARACTERS;
}

<PP_LINE,DEFINE_DECL,MACRO_DEF>\n {
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
    yypop_buffer_state();
    delete buffers.pop();
    if (!macroExpansionStack.isEmpty()) {
        macroExpansionStack.removeFirst();
    }
    return !YY_CURRENT_BUFFER;
}

void pushInput(const QString &sourcecode)
{
    buffers.push(new QByteArray(sourcecode.toAscii()));
    yypush_buffer_state(YY_CURRENT_BUFFER);
    yy_scan_string(buffers.top()->constData());
}

void reset()
{
    doNotMacroExpandNextIdentifer = false;
    while (YY_CURRENT_BUFFER) {
        yypop_buffer_state();
    }
    while (!buffers.isEmpty()) {
        delete buffers.pop();
    }
    macroExpansionStack.clear();
}

char *copyStrAndNullTerminate(const char *str, unsigned int len)
{
    char *copy = new char[len + 1];
    strncpy(copy, str, len + 1);
    return copy;
}

void expandMacro(const QString &macroName)
{
    if (!macroTable.contains(macroName)) {
        // TODO: error handling
        return;
    }

printf(">%s<", macroName.toAscii().constData());
    QStringList expansion;
    foreach (macroPart part, macroTable.value(macroName).parts) {
        expansion.append(part.text);
    }
    macroExpansionStack.prepend(macroName);
printf("]%s[", expansion.join("").toAscii().constData());
    pushInput(expansion.join(""));
}

