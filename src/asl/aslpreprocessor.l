
directivePrefix ^[ \t]*#
identifier [_a-zA-Z][_a-zA-Z0-9]+

%x PP_LINE
%x DEFINE_DECL
%x MACRO_DEF
%x ARGLIST

%{
    #include <QByteArray>
    #include <QStack>
    #include <QTextStream>

    #ifdef TEST
        #include "../src/asl/aslpreprocessor_parserinternal.h"
    #else
        #include "asl/aslpreprocessor_parserinternal.h"
    #endif

    #include "aslpreprocessor_yacc.h"

    using namespace asl::ppinternal;

    namespace asl
    {
    namespace ppinternal
    {
        QStack<QByteArray *> readBuffers;
        QStringList macroExpansionStack;

        /* Reading a IDENTIFIER with this disabled, should always reenable macro
         * expansion.
         *
         * Defaults to false, because in most places the expansions are
         * irrelevant for us.
         */
        bool doMacroExpansions = false;

        void expandMacro(const QString &macroName);

        /* Destroys yytext! */
        bool argumentListFollows();
        QStringList readArgumentList();

    } /* namespace ppinternal */
    } /* namespace asl */
%}

%%

"/*""/"?([^*/]*"*")+"/" {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

"//".*\n {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

<INITIAL>{directivePrefix}define {
    BEGIN DEFINE_DECL;
    doMacroExpansions = false;
    return DEFINE;
}

<INITIAL>{directivePrefix}undef {
    BEGIN PP_LINE;
    doMacroExpansions = false;
    return UNDEF;
}

<INITIAL>{directivePrefix}if {
    BEGIN PP_LINE;
    doMacroExpansions = true;
    return IF;
}

<INITIAL>{directivePrefix}elif {
    BEGIN PP_LINE;
    doMacroExpansions = true;
    return ELIF;
}

<INITIAL>{directivePrefix}ifdef {
    BEGIN PP_LINE;
    doMacroExpansions = false;
    return IFDEF;
}

<INITIAL>{directivePrefix}ifndef {
    BEGIN PP_LINE;
    doMacroExpansions = false;
    return IFNDEF;
}

<INITIAL>{directivePrefix}else {
    BEGIN PP_LINE;
    doMacroExpansions = true;
    return ELSE;
}

<INITIAL>{directivePrefix}endif {
    BEGIN PP_LINE;
    doMacroExpansions = true;
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
<PP_LINE>"defined" { 
    doMacroExpansions = false;
    return DEFINED;
}

<PP_LINE>([0-9]+|0[0-7]+|0x[0-9a-fA-F]+) {
    aslpreprocessorlval.integer = strtoul(yytext, NULL, 0);
    return INTEGER;
}

<PP_LINE>{identifier} {
    aslpreprocessorlval.string = new QString(yytext);

    if (!doMacroExpansions) {
        doMacroExpansions = true;
        return IDENTIFIER;
    } else {
        if (argumentListFollows()) {
            readArgumentList();
        }

        expandMacro(*(aslpreprocessorlval.string));
        delete aslpreprocessorlval.string;
    }
}

<DEFINE_DECL>{identifier} {
    /* yytext has to be copied first, because peeking at the next char will
     * destroy yytext. */
    aslpreprocessorlval.string = new QString(yytext);

    if (argumentListFollows()) {
        BEGIN ARGLIST;
    } else {
        BEGIN MACRO_DEF;
    }

    return IDENTIFIER;
}

<ARGLIST>[(,] { return *yytext; }
<ARGLIST>")" {
    BEGIN MACRO_DEF;
    return *yytext;
}
<ARGLIST>{identifier} {
    aslpreprocessorlval.string = new QString(yytext);
    return IDENTIFIER;
}


<MACRO_DEF>. {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

<PP_LINE,DEFINE_DECL,MACRO_DEF>\n {
    BEGIN INITIAL;
    return ENDPP;
}

.|\n {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

%%

int yywrap(void)
{
    yypop_buffer_state();
    delete readBuffers.pop();
    if (!macroExpansionStack.isEmpty()) {
        macroExpansionStack.removeFirst();
    }
    return !YY_CURRENT_BUFFER;
}

namespace asl
{
namespace ppinternal
{

void clean()
{
    while (YY_CURRENT_BUFFER) {
        yypop_buffer_state();
    }
    while (!readBuffers.isEmpty()) {
        delete readBuffers.pop();
    }
    macroExpansionStack.clear();
    macroTable.clear();
}

void resetAndClean() {
    doMacroExpansions = false;
    clean();
}

void pushInput(const QString &sourcecode)
{
    readBuffers.push(new QByteArray(sourcecode.toAscii()));
    yypush_buffer_state(YY_CURRENT_BUFFER);
    yy_scan_string(readBuffers.top()->constData());
}

void expandMacro(const QString &macroName)
{
    if (!isDefined(macroName)) {
        // TODO: error handling
        return;
    }

    QStringList expansion;
    foreach (MacroPart part, *(macroTable.value(macroName).parts.data())) {
        expansion.append(part.text);
    }
    macroExpansionStack.prepend(macroName);
    pushInput(expansion.join(""));
}

bool argumentListFollows()
{
    char next = yyinput();
    unput(next);
    return '(' == next;
}

QStringList readArgumentList()
{
    unsigned int parenthesisNestingLevel = 0;
    char c;

    do {
        c = yyinput();
        if ('(' == c) {
            ++parenthesisNestingLevel;
        } else if (')' == c) {
            --parenthesisNestingLevel;
        }
    } while (parenthesisNestingLevel > 0);

    return QStringList();
}

} /* namespace ppinternal */
} /* namespace asl */

