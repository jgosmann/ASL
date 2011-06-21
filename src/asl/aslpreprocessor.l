
%option yylineno

directivePrefix ^[ \t]*#
identifier [_a-zA-Z][_a-zA-Z0-9]*

%x PP_LINE
%x DEFINE_DECL
%x MACRO_DEF
%x ARGLIST
%x ERROR_DIRECTIVE

%{
    #include <cassert>
    #include <QByteArray>
    #include <QStack>
    #include <QTextStream>

    #include "asl/aslpreprocessor_parserinternal.h"

    #include "aslpreprocessor_yacc.h"

    void aslpreprocessorerror(const char *msg);

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

        void popInput();

        /**
         * Does macro expansion by pushing a new buffer on the stack.
         *
         * \arg \a arguments has to be \c NULL if the expanded macro has no
         *     arguments. If it is a macro with an empty argument list
         *     (<tt>\#define MACRO()</tt>) you have to pass the pointer to an
         *     empty list.
         */
        void handleMacro(const QString &macroName,
                const QStringList *arguments);
        QString expandPredefinedMacro(const QString &macroName,
                const QStringList *arguments);
        bool checkIfValidMacroAndLogErrorIfNot(const QString &macroName,
                const QStringList *arguments);
        QString expandUserMacro(const QString &macroName,
                const QStringList *arguments);
        void pushExpandedMacro(const QString &macroName,
                const QString &expansion);

        /** \attention Destroys \c yytext! */
        bool argumentListFollows();
        QStringList readArgumentList();

        QString readErrorDirective();

    } /* namespace ppinternal */
    } /* namespace asl */
%}

%%

<INITIAL>"/*"("/"?[^*/]*("*"[^/])?)+"*/" {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

<INITIAL>"//".*\n {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

<INITIAL>{directivePrefix}define {
    if (excludeIfNestingLevel > 0) {
        aslpreprocessorlval.string = new QString(yytext);
        return CHARACTERS;
    } else {
        BEGIN DEFINE_DECL;
        doMacroExpansions = false;
        return DEFINE;
    }
}

<INITIAL>{directivePrefix}undef {
    if (excludeIfNestingLevel > 0) {
        aslpreprocessorlval.string = new QString(yytext);
        return CHARACTERS;
    } else {
        BEGIN PP_LINE;
        doMacroExpansions = false;
        return UNDEF;
    }
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

<INITIAL>{directivePrefix}error[^\n] {
    BEGIN ERROR_DIRECTIVE;
    doMacroExpansions = false;
    return ERROR;
}

<INITIAL>{directivePrefix}line {
    BEGIN PP_LINE;
    doMacroExpansions = false;
    return LINE;
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
    QString identifier = QString(yytext);

    if (!doMacroExpansions) {
        aslpreprocessorlval.string = new QString(identifier);
        doMacroExpansions = true;
        return IDENTIFIER;
    } else {
        bool macroHasArgList = isDefined(identifier)
                && macroTable.value(identifier).arguments != NULL;
        if (macroHasArgList && argumentListFollows()) {
            QStringList arguments = readArgumentList();
            handleMacro(identifier, &arguments);
        } else {
            handleMacro(identifier, NULL);
        }
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
<ARGLIST,MACRO_DEF>{identifier} {
    aslpreprocessorlval.string = new QString(yytext);
    return IDENTIFIER;
}

<PP_LINE,DEFINE_DECL,MACRO_DEF>"/*"|"//" {
    unput(yytext[1]);
    unput(yytext[0]);
    BEGIN INITIAL;
    return ENDPP;
}

<MACRO_DEF>\\\n { }

<MACRO_DEF>. {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

<PP_LINE,DEFINE_DECL,MACRO_DEF,ERROR_DIRECTIVE>"/*""/"?([^*/]*"*")+"/" { }
<ERROR_DIRECTIVE>"//".*\n {
    BEGIN INITIAL;
    return ENDPP;
}

<ERROR_DIRECTIVE>. {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

<PP_LINE,DEFINE_DECL,MACRO_DEF,ERROR_DIRECTIVE>\n {
    BEGIN INITIAL;
    return ENDPP;
}

<PP_LINE,DEFINE_DECL,MACRO_DEF>[^ \t\r\n] {
    aslpreprocessorerror("syntax error");
}

^[^#/\n]*\n {
    /* This pattern is to improve perfomance. It matches all whole lines which
     * definitly to not include a token which is not CHARACTERS. If a line
     * contains a '#' or '/' it may contain a comment or preprocessor directive
     * token. Therefore these are disallowed in the line.
     */
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}


.|\n {
    aslpreprocessorlval.string = new QString(yytext);
    return CHARACTERS;
}

<PP_LINE,DEFINE_DECL,ARGLIST>. { }

%%

int yywrap(void)
{
    popInput();
    if (!YY_CURRENT_BUFFER) {
        resetAndClean();
        return 1;
    } else {
        return 0;
    }
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
    clean();
    yylineno = 1;
    doMacroExpansions = false;
    ifNestingLevel = 0;
    excludeIfNestingLevel = 0;
}

void popInput() {
    yypop_buffer_state();
    delete readBuffers.pop();
    if (!macroExpansionStack.isEmpty()) {
        macroExpansionStack.removeFirst();
    }
}

void pushInput(const QString &sourcecode)
{
    readBuffers.push(new QByteArray(sourcecode.toAscii()));

    if (readBuffers.size() == 1) {
        if (sourcecode.rightRef(1) != "\n") {
            readBuffers.top()->append('\n');
        }
    }

    yypush_buffer_state(YY_CURRENT_BUFFER);
    yy_scan_string(readBuffers.top()->constData());
}

void handleMacro(const QString &macroName, const QStringList *arguments)
{
    QString expansion = expandPredefinedMacro(macroName, arguments);
    bool expandedMacro = !expansion.isEmpty();

    if (!expandedMacro) {
        if (!checkIfValidMacroAndLogErrorIfNot(macroName, arguments)) {
            return;
        }
        expansion = expandUserMacro(macroName, arguments);
    }

    pushExpandedMacro(macroName, expansion);
}

QString expandPredefinedMacro(const QString &macroName,
        const QStringList *arguments)
{
    /* Suppress unused argument warning. Maybe there will be predefined macros
     * with arguments in later versions.
     */
    (void) arguments;

    if ("__LINE__" == macroName) {
        return QString::number(aslpreprocessorlineno);
    } else if("__FILE__" == macroName) {
        return QString::number(sourceStringNo);
    } else if("__VERSION__" == macroName) {
        return QString::number(glslVersion);
    }
    return "";
}

bool checkIfValidMacroAndLogErrorIfNot(const QString &macroName,
        const QStringList *arguments)
{
    if (!isDefined(macroName)) {
        aslpreprocessorerror(
            ("undefined macro: " + macroName).toAscii().data());
        return false;
    }

    const Macro &macro = macroTable.value(macroName);

    if (macro.arguments) {
        if (!arguments || arguments->size() < macro.arguments->size()) {
            aslpreprocessorerror(
                ("Macro " + macroName + " has too few arguments.")
                .toAscii().data());
            return false;
        } else if (arguments->size() > macro.arguments->size()) {
            aslpreprocessorerror(
                ("Macro " + macroName + " has too many arguments.")
                .toAscii().data());
            return false;
        }
    }

    return true;
}

QString expandUserMacro(const QString &macroName, const QStringList *arguments)
{
    assert(checkIfValidMacroAndLogErrorIfNot(macroName, arguments));

    const Macro &macro = macroTable.value(macroName);

    QStringList expansion;
    foreach (MacroPart part, *(macro.parts.data())) {
        bool isMacroArgument = macro.arguments
            && part.isArgument && macro.arguments->contains(part.text);
        if (arguments && isMacroArgument) {
            const unsigned int argIdx = macro.arguments->indexOf(part.text);
            const QString argExpansion = arguments->at(argIdx);
            expansion.append(argExpansion);
        } else {
            expansion.append(part.text);
        }
    }

    return expansion.join("");
}

void pushExpandedMacro(const QString &macroName, const QString &expansion)
{
    macroExpansionStack.prepend(macroName);
    pushInput(expansion);
}

bool argumentListFollows()
{
    char next = yyinput();
    unput(next);
    return '(' == next;
}

QStringList readArgumentList()
{
    QStringList arguments = QStringList(QString());

    unsigned int parenthesisifNestingLevelLevel = 0;
    char c;

    do {
        c = yyinput();
        if ('(' == c) {
            ++parenthesisifNestingLevelLevel;
        } else if (')' == c) {
            --parenthesisifNestingLevelLevel;
        } else if (',' == c && parenthesisifNestingLevelLevel == 1) {
            arguments.append(QString());
        } else {
            arguments.back() += c;
        }
    } while (parenthesisifNestingLevelLevel > 0);

    return arguments;
}

} /* namespace ppinternal */
} /* namespace asl */

