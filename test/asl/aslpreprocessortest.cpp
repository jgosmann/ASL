#include <QRegExp>
#include <QScopedPointer>
#include <QString>

#include "../src/asl/aslpreprocessor.h"
#include "../src/asl/compilationexception.h"

#include "common.h"

namespace asl
{
class ASLPreprocessorTest : public TestFixture
{
public:
    void doesNotChangeShaderWithoutPreprocessorDirectives()
    {
        const QString input(
                "void main(void) {\n"
                "    gl_FragColor.rgb = vec3(1, 0, 0);\n"
                "}\n");
        testProcessing(input, input);
    }

    void excludesIfNDefPartIfMacroIsDefined()
    {
        const QString input(
                "#define INCLUDE\n"
                "#ifndef INCLUDE\n"
                "    /* should not be included */\n"
                "#endif\n"
                "/* should be included */\n");

        const QString expected = "/* should be included */\n";
        testProcessing(input, expected);
    }

    void includesIfNDefPartIfMacroIsNotDefined()
    {
        const QString header("#ifndef UNDEFINED_MACRO\n");
        const QString partToInclude("/* should be included */\n");
        const QString footer("#endif\n/* should be included */\n");

        const QString expected = partToInclude + "/* should be included */\n";
        const QString input = header + partToInclude + footer;
        testProcessing(input, expected);
    }

    void ignoresPreprocessorDirectivesInMultilineComment()
    {
        const QString input(
                "/*\n"
                "#ifndef UNDEFINED_MACRO\n"
                "should be included\n"
                "#endif\n"
                "*/\n");
        testProcessing(input, input);
    }

    void ignoresPreprocessorDirectivesInSinglelineComment()
    {
        const QString input(
                "// #ifndef UNDEFINED_MACRO\n"
                "// should be included\n"
                "// #endif\n");
        testProcessing(input, input);
    }

    void ignoresPreprocessorDirectivesIfNotFirstNonWhitespaceInLine()
    {
        const QString input(
                "someChars #ifndef UNDEFINED_MACRO\n"
                "/* should be included */\n"
                "someChars #endif\n");
        testProcessing(input, input);
    }

    void logsErrorIfMissingEndif()
    {
        const QString input("#ifndef UNDEFINED_MACRO\n");
        preprocessor.process(input);
        assertLoggedError(2, QRegExp(".*syntax.*"));
    }

    void excludesIfdefPartIfMacroNotDefined()
    {
        const QString input(
                "#ifdef UNDEFINED_MACRO\n"
                "/* should be excluded */\n"
                "#endif\n"
                "/* should be included */\n");
        const QString output("/* should be included */\n");
        testProcessing(input, output);
    }

    void macrosClearedBeforeParsingNextProgram()
    {
        preprocessor.process("#define SHOULD_BE_UNDEFINED\n");
        
        const QString checkInput(
                "#ifdef SHOULD_BE_UNDEFINED\n"
                "/* should be excluded */\n"
                "#endif\n");
        testProcessing(checkInput, "");
    }

    void keepsUnknownDirectives()
    {
        const QString input("#version 140\n");
        testProcessing(input, input);
    }

    void handlesNestingInExcludedParts()
    {
        const QString input(
                "#ifdef UNDEFINED_MACRO\n"
                "    /* exclude 1 */\n"
                "    #ifndef UNDEFINED_MACRO\n"
                "        /* exclude 2 */\n"
                "    #endif\n"
                "    /* exclude 3 */\n"
                "#endif\n");
        testProcessing(input, "");
    }

    void handlesNestingInIncludedParts()
    {
        const QString input(
                "#ifndef UNDEFINED_MACRO\n"
                "    /* include 1 */\n"
                "    #ifdef UNDEFINED_MACRO\n"
                "        /* exclude */\n"
                "    #endif\n"
                "    /* include 2 */\n"
                "#endif\n");
        const QString expectedOutput(
                "    /* include 1 */\n"
                "    /* include 2 */\n");
        testProcessing(input, expectedOutput);
    }

    void handlesElseClauseInclusion()
    {
        const QString input(
                "#ifdef UNDEFINED_MACRO\n"
                "    /* exclude */\n"
                "#else\n"
                "    /* include */\n"
                "#endif\n");
        const QString expectedOutput("    /* include */\n");
        testProcessing(input, expectedOutput);
    }

    void handlesElseClauseExclusion()
    {
        const QString input(
                "#ifndef UNDEFINED_MACRO\n"
                "    /* include */\n"
                "#else\n"
                "    /* exclude */\n"
                "#endif\n");
        const QString expectedOutput("    /* include */\n");
        testProcessing(input, expectedOutput);
    }

    void handlesNestedElseClauses()
    {
        const QString input(
            "#ifdef UNDEFINED_MACRO\n"
            "    /* exclude 1 */\n"
            "    #ifdef UNDEFINED_MACRO\n"
            "        /* exclude 2 */\n"
            "    #else\n"
            "        /* exclude 3 */\n"
            "    #endif\n"
            "    /* exclude 4 */\n"
            "#else\n"
            "    /* include 1 */\n"
            "    #ifdef UNDEFINED_MACRO\n"
            "        /* exclude 4 */\n"
            "    #else\n"
            "        /* include 2 */\n"
            "    #endif\n"
            "    /* include 3 */\n"
            "    #ifndef UNDEFINED_MACRO\n"
            "        /* include 4 */\n"
            "    #else\n"
            "        /* exclude 5 */\n"
            "    #endif\n"
            "    /* include 5 */\n"
            "#endif\n");
        const QString expectedOutput(
            "    /* include 1 */\n"
            "        /* include 2 */\n"
            "    /* include 3 */\n"
            "        /* include 4 */\n"
            "    /* include 5 */\n");
        testProcessing(input, expectedOutput);
    }

    void undefiningUndefinedMacroHasNoEffect()
    {
        const QString input("#undef UNDEFINED_MACRO\n");
        testProcessing(input, "");
    }

    void undefiningDefinedMacroUndefinesMacro()
    {
        const QString input(
                "#define SHOULD_BE_UNDEFINED\n"
                "#undef SHOULD_BE_UNDEFINED\n"
                "#ifdef SHOULD_BE_UNDEFINED\n"
                "    /* should be excluded */\n"
                "#endif\n");
        testProcessing(input, "");
    }

    void includesIfPartIfConditionMet()
    {
        const QString input(
                "#if 1\n"
                "/* include */\n"
                "#endif\n");
        const QString expectedOutput("/* include */\n");
        testProcessing(input, expectedOutput);
    }

    void includesElsePartIfConditionIsNotMet()
    {
        const QString input(
                "#if 0\n"
                "/* exclude */\n"
                "#else\n"
                "/* include */\n"
                "#endif\n");
        const QString expectedOutput("/* include */\n");
        testProcessing(input, expectedOutput);
    }

    void ignoresElifPartsAndElsePartIfIfConditionIsMet()
    {
        const QString input(
                "#if 1\n"
                "/* include */\n"
                "#elif 1\n"
                "/* exclude 1 */\n"
                "#elif 1\n"
                "/* exclude 2 */\n"
                "#else\n"
                "/* exclude 3 */\n"
                "#endif\n");
        const QString expectedOutput("/* include */\n");
        testProcessing(input, expectedOutput);
    }

    void ignoresElifPartsAndElsePartsAfterFirstMetCondition()
    {
        const QString input(
                "#if 0\n"
                "/* exclude 1 */\n"
                "#elif 0\n"
                "/* exclude 2 */\n"
                "#elif 1\n"
                "/* include */\n"
                "#elif 1\n"
                "/* exclude 3 */\n"
                "#elif 1\n"
                "/* exclude 4 */\n"
                "#else\n"
                "/* exclude 5 */\n"
                "#endif\n");
        const QString expectedOutput("/* include */\n");
        testProcessing(input, expectedOutput);
    }

    void includesElsePartIfNoIfOrElifConditionIsMet()
    {
        const QString input(
                "#if 0\n"
                "/* exclude 1 */\n"
                "#elif 0\n"
                "/* exclude 2 */\n"
                "#elif 0\n"
                "/* exclude 3 */\n"
                "#else\n"
                "/* include */\n"
                "#endif\n");
        const QString expectedOutput("/* include */\n");
        testProcessing(input, expectedOutput);
    }

    void handlesNestedElifs()
    {
        const QString input(
                "#if 0\n"
                "    /* exclude 1 */\n"
                "    #if 0\n"
                "        /* exclude 2 */\n"
                "    #elif 1\n"
                "        /* exlcude 3 */\n"
                "    #endif\n"
                "#elif 1\n"
                "    /* include 1 */\n"
                "    #if 0\n"
                "        /* exclude 4 */\n"
                "    #elif 1\n"
                "        /* include 2 */\n"
                "    #elif 1\n"
                "        /* exclude 5 */\n"
                "    #endif\n"
                "    /* include 3 */\n"
                "#endif\n");
        const QString expectedOutput(
                "    /* include 1 */\n"
                "        /* include 2 */\n"
                "    /* include 3 */\n");
        testProcessing(input, expectedOutput);
    }

    void testEqualityOperator()
    {
        CPPUNIT_ASSERT(!expressionResultsInZero("1 == 1"));
        CPPUNIT_ASSERT(expressionResultsInZero("1 == 0"));
    }

    void testUnequalityOperator()
    {
        CPPUNIT_ASSERT(expressionResultsInZero("1 != 1"));
        CPPUNIT_ASSERT(!expressionResultsInZero("1 != 0"));
    }

    void testLogicalAndOperator()
    {
        CPPUNIT_ASSERT(!expressionResultsInZero("1 && 2"));
        CPPUNIT_ASSERT(expressionResultsInZero("0 && 1"));
    }

    void testLogicalOrOperator()
    {
        CPPUNIT_ASSERT(!expressionResultsInZero("2 || 1"));
        CPPUNIT_ASSERT(expressionResultsInZero("0 || 0"));
    }

    void testLowerOperator()
    {
        CPPUNIT_ASSERT(!expressionResultsInZero("2 < 3"));
        CPPUNIT_ASSERT(expressionResultsInZero("3 < 3"));
        CPPUNIT_ASSERT(expressionResultsInZero("4 < 3"));
    }

    void testGreaterOperator()
    {
        CPPUNIT_ASSERT(expressionResultsInZero("2 > 3"));
        CPPUNIT_ASSERT(expressionResultsInZero("3 > 3"));
        CPPUNIT_ASSERT(!expressionResultsInZero("4 > 3"));
    }

    void testLowerOrEqualOperator()
    {
        CPPUNIT_ASSERT(!expressionResultsInZero("2 <= 3"));
        CPPUNIT_ASSERT(!expressionResultsInZero("3 <= 3"));
        CPPUNIT_ASSERT(expressionResultsInZero("4 <= 3"));
    }

    void testGreaterOrEqualOperator()
    {
        CPPUNIT_ASSERT(expressionResultsInZero("2 >= 3"));
        CPPUNIT_ASSERT(!expressionResultsInZero("3 >= 3"));
        CPPUNIT_ASSERT(!expressionResultsInZero("4 >= 3"));
    }

    void testLogicalNegationOperator()
    {
        CPPUNIT_ASSERT(expressionResultsInZero("!1"));
        CPPUNIT_ASSERT(!expressionResultsInZero("!0"));
    }

    void testBitwiseBinaryNonShiftOperators()
    {
        CPPUNIT_ASSERT(expressionResultsIn("0xff0 & 0xf00", 0xf00));
        CPPUNIT_ASSERT(expressionResultsIn("0xff0 ^ 0xf00", 0x0f0));
        CPPUNIT_ASSERT(expressionResultsIn("0xff0 | 0xf00", 0xff0));
    }
    
    void testBitwiseBinaryShiftOperators()
    {
        CPPUNIT_ASSERT(expressionResultsIn("0xf0f << 4", 0xf0f0));
        CPPUNIT_ASSERT(expressionResultsIn("0xf0f >> 4", 0x00f0));
    }

    void testAdditiveOperators()
    {
        CPPUNIT_ASSERT(expressionResultsIn("2 + 3", 5));
        CPPUNIT_ASSERT(expressionResultsIn("2 - 3", -1));
    }

    void testMultiplicativeOperators()
    {
        CPPUNIT_ASSERT(expressionResultsIn("2 * 3", 6));
        CPPUNIT_ASSERT(expressionResultsIn("4 / 2", 2));
        CPPUNIT_ASSERT(expressionResultsIn("3 / 2", 1));
        CPPUNIT_ASSERT(expressionResultsIn("7 % 2", 1));
    }

    void testUnaryIntegerOperators()
    {
        CPPUNIT_ASSERT(expressionResultsIn("+ 2", 2));
        CPPUNIT_ASSERT(expressionResultsIn("- 2", -2));
    }

    void testBitwiseNegation()
    {
        CPPUNIT_ASSERT(expressionResultsIn("~ 0xf0", 0xffffffffffffff0f));
    }

    void testDefinedOperator()
    {
        const QString input(
                "#define DEFINED_MACRO\n"
                "#if defined DEFINED_MACRO\n"
                "    /* include */\n"
                "#endif\n"
                "#if defined UNDEFINED_MACRO\n"
                "    /* exclude */\n"
                "#endif\n");
        const QString expectedOutput("    /* include */\n");
        testProcessing(input, expectedOutput);
    }

    void testParenthesisTakePrecedence()
    {
        CPPUNIT_ASSERT(expressionResultsIn("(2+3)*3", 15));
    }

    void understandsDecimalNumbers()
    {
        CPPUNIT_ASSERT(expressionResultsIn("123456789", 123456789));
    }

    void understandsOctalNumbers()
    {
        CPPUNIT_ASSERT(expressionResultsIn("017", 017));
    }

    void understandsHexadecimalNumbers()
    {
        CPPUNIT_ASSERT(expressionResultsIn("0x1f", 0x1f));
    }

    void macroWithoutArgumentsGetsExpanded()
    {
        const QString input(
                "#define REPLACE 42\n"
                "#if REPLACE == 42\n"
                "    /* include */\n"
                "#endif\n");
        const QString expectedOutput("    /* include */\n");
        testProcessing(input, expectedOutput);
    }

    void macroWithArgumentsGetsExpanded()
    {
        const QString input(
                "#define REPLACE(arg1, arg2) 42\n"
                "#if REPLACE(2, 3) == 42\n"
                "    /* include */\n"
                "#endif\n");
        const QString expectedOutput("    /* include */\n");
        testProcessing(input, expectedOutput);
    }

    void macroArgumentsGetExpanded()
    {
        const QString input(
                "#define REPLACE(arg1, arg2) arg1 * arg2\n"
                "#if REPLACE(6, 7) == 42\n"
                "    /* include */\n"
                "#endif\n");
        const QString expectedOutput("    /* include */\n");
        testProcessing(input, expectedOutput);
    }

    void macroWithoutArugmentsGetsExpandedWhenFollowedByParenthesis() {
        const QString input(
                "#define REPLACE 1 +\n"
                "#if REPLACE(1 + 1) == 3\n"
                "    /* include */\n"
                "#endif\n");
        const QString expectedOutput("    /* include */\n");
        testProcessing(input, expectedOutput);
    }

    void logsErrorIfMacroMissingEmptyArgumentList()
    {
        const QString input(
                "#define MACRO() 0\n"
                "#if MACRO + 1\n"
                "#endif\n");
        preprocessor.process(input);
        assertLoggedError(2, QRegExp(".*argument.*"));
    }

    void logsErrorIfTooFewArgumentsArePassedToMacro()
    {
        const QString input(
                "#define MACRO(a, b) 0\n"
                "#if MACRO(1) + 1\n"
                "#endif\n");
        preprocessor.process(input);
        assertLoggedError(2, QRegExp(".*too\\s+few\\s+arguments.*"));
    }

    void logsErrorIfTooManyArgumentsArePassedToMacro()
    {
        const QString input(
                "#define MACRO(a) 0\n"
                "#if MACRO(1, 1) + 1\n"
                "#endif\n");
        preprocessor.process(input);
        assertLoggedError(2, QRegExp(".*too\\s+many\\s+arguments.*"));
    }

    void logsErrorIfExpandingUndefinedMacro()
    {
        const QString input(
                "#if MACRO\n"
                "#endif\n");
        preprocessor.process(input);
        assertLoggedError(1, QRegExp(".*undefined.*"));
    }

    void logsErrorIfPreprocessorDirectiveWithinAnother()
    {
        const QString input(
                "#define #if TEST\n"
                "#if #define\n");
        preprocessor.process(input);
        assertLoggedError(1, QRegExp(".*syntax.*"));
        assertLoggedError(2, QRegExp(".*syntax.*"));
    }

    void commentEndsDefine()
    {
        const QString input(
                "#define MACRO 1 /* some comment\n"
                "    spanning more than one line */\n"
                "#if MACRO\n"
                "    /* include */\n"
                "#endif\n");
        testProcessing(input, "/* some comment\n    spanning more than one "
                "line */\n    /* include */\n");
    }

    void escapedNewlineContinuesDefine()
    {
        const QString input(
                "#define MACRO 0 + \\\n"
                "    1\n"
                "#if MACRO\n"
                "    /* include */\n"
                "#endif\n");
        testProcessing(input, "    /* include */\n");
    }

    void unfulfilledIfClauseExcludesDefine()
    {
        const QString input(
                "#if 0\n"
                "#define SHOULD_BE_UNDEFINED\n"
                "#endif\n"
                "#ifdef SHOULD_BE_UNDEFINED\n"
                "/* should be excluded */\n"
                "#endif\n");
        testProcessing(input, "");
    }

    void unfulfilledIfClauseExcludesSyntaxErrors()
    {
        const QString input(
                "#if 0\n"
                "#define A(,,)\n"
                "#endif\n");
        testProcessing(input, "");
    }

    void expandsMacrosInMacros()
    {
        const QString input(
                "#define A B\n"
                "#define B 1\n"
                "#if A\n"
                "    /* include */\n"
                "#endif\n");
        testProcessing(input, "    /* include */\n");
    }

    CPPUNIT_TEST_SUITE(ASLPreprocessorTest);
    CPPUNIT_TEST(doesNotChangeShaderWithoutPreprocessorDirectives);
    CPPUNIT_TEST(excludesIfNDefPartIfMacroIsDefined);
    CPPUNIT_TEST(includesIfNDefPartIfMacroIsNotDefined);
    CPPUNIT_TEST(ignoresPreprocessorDirectivesInMultilineComment);
    CPPUNIT_TEST(ignoresPreprocessorDirectivesInSinglelineComment);
    CPPUNIT_TEST(ignoresPreprocessorDirectivesIfNotFirstNonWhitespaceInLine);
    CPPUNIT_TEST(logsErrorIfMissingEndif);
    CPPUNIT_TEST(excludesIfdefPartIfMacroNotDefined);
    CPPUNIT_TEST(macrosClearedBeforeParsingNextProgram);
    CPPUNIT_TEST(keepsUnknownDirectives);
    CPPUNIT_TEST(handlesNestingInExcludedParts);
    CPPUNIT_TEST(handlesNestingInIncludedParts);
    CPPUNIT_TEST(handlesElseClauseInclusion);
    CPPUNIT_TEST(handlesElseClauseExclusion);
    CPPUNIT_TEST(handlesNestedElseClauses);
    CPPUNIT_TEST(undefiningUndefinedMacroHasNoEffect);
    CPPUNIT_TEST(undefiningDefinedMacroUndefinesMacro);
    CPPUNIT_TEST(includesIfPartIfConditionMet);
    CPPUNIT_TEST(includesElsePartIfConditionIsNotMet);
    CPPUNIT_TEST(ignoresElifPartsAndElsePartIfIfConditionIsMet);
    CPPUNIT_TEST(ignoresElifPartsAndElsePartsAfterFirstMetCondition);
    CPPUNIT_TEST(includesElsePartIfNoIfOrElifConditionIsMet);
    CPPUNIT_TEST(handlesNestedElifs);
    CPPUNIT_TEST(testEqualityOperator);
    CPPUNIT_TEST(testUnequalityOperator);
    CPPUNIT_TEST(testLogicalAndOperator);
    CPPUNIT_TEST(testLogicalOrOperator);
    CPPUNIT_TEST(testLowerOperator);
    CPPUNIT_TEST(testGreaterOperator);
    CPPUNIT_TEST(testLowerOrEqualOperator);
    CPPUNIT_TEST(testGreaterOrEqualOperator);
    CPPUNIT_TEST(testLogicalNegationOperator);
    CPPUNIT_TEST(testBitwiseBinaryNonShiftOperators);
    CPPUNIT_TEST(testBitwiseBinaryShiftOperators);
    CPPUNIT_TEST(testAdditiveOperators);
    CPPUNIT_TEST(testMultiplicativeOperators);
    CPPUNIT_TEST(testUnaryIntegerOperators);
    CPPUNIT_TEST(testBitwiseNegation);
    CPPUNIT_TEST(testDefinedOperator);
    CPPUNIT_TEST(testParenthesisTakePrecedence);
    CPPUNIT_TEST(understandsDecimalNumbers);
    CPPUNIT_TEST(understandsOctalNumbers);
    CPPUNIT_TEST(understandsHexadecimalNumbers);
    CPPUNIT_TEST(macroWithoutArgumentsGetsExpanded);
    CPPUNIT_TEST(macroWithArgumentsGetsExpanded);
    CPPUNIT_TEST(macroArgumentsGetExpanded);
    CPPUNIT_TEST(macroWithoutArugmentsGetsExpandedWhenFollowedByParenthesis);
    CPPUNIT_TEST(logsErrorIfMacroMissingEmptyArgumentList);
    CPPUNIT_TEST(logsErrorIfTooFewArgumentsArePassedToMacro);
    CPPUNIT_TEST(logsErrorIfTooManyArgumentsArePassedToMacro);
    CPPUNIT_TEST(logsErrorIfExpandingUndefinedMacro);
    CPPUNIT_TEST(logsErrorIfPreprocessorDirectiveWithinAnother);
    CPPUNIT_TEST(commentEndsDefine);
    CPPUNIT_TEST(escapedNewlineContinuesDefine);
    CPPUNIT_TEST(unfulfilledIfClauseExcludesDefine);
    CPPUNIT_TEST(unfulfilledIfClauseExcludesSyntaxErrors);
    //CPPUNIT_TEST(expandsMacrosInMacros);
    CPPUNIT_TEST_SUITE_END();

private:
    void testProcessing(const QString &input, const QString &expectedOutput)
    {
        CPPUNIT_ASSERT_EQUAL(expectedOutput.toStdString(),
                preprocessor.process(input).toStdString());
        assertLogIsEmpty();
    }

    bool expressionResultsInZero(const QString &expression)
    {
        return expressionResultsIn(expression, 0);
    }

    bool expressionResultsIn(const QString &expression, long int value)
    {
        const QString input(
                "#if (" + expression + ") == " + QString::number(value) + "\n"
                "    /* is zero */\n"
                "#endif\n");
        const QString output = preprocessor.process(input);
        assertLogIsEmpty();
        return output == "    /* is zero */\n";
    }

    void assertLoggedError(unsigned int line,
            const QRegExp &mustContain = QRegExp(".*"))
    {
        const QRegExp matcher("(\\d+):\\s+\\(preprocessor\\)\\s+([^\r\n]*)");
        const QString log = preprocessor.log();

        int matchIdx = -1;
        while (0 <= (matchIdx = matcher.indexIn(log, matchIdx + 1))) {
            bool lineCorrect = (matcher.cap(1).toUInt() == line);
            bool mustContainCorrect =
                (0 <= mustContain.indexIn(matcher.cap(2)));
            if (lineCorrect && mustContainCorrect) {
                return;
            }
        }

        CPPUNIT_FAIL(("Expected error containing \"" + mustContain.pattern()
                + "\" in line " + QString::number(line) + ". Actual log:\n"
                + log).toAscii().constData());
    }

    void assertLogIsEmpty()
    {
        CPPUNIT_ASSERT(preprocessor.log().length() <= 0);
    }


    asl::ASLPreprocessor preprocessor;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::ASLPreprocessorTest);

using namespace asl;

