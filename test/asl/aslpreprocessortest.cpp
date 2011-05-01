#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtOpenGL/QGLPixelBuffer>

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
                "    /* should be included */\n"
                "#endif\n");

        const QString expected = "\n";
        testProcessing(input, expected);
    }

    void includesIfNDefPartIfMacroIsNotDefined()
    {
        const QString header("#ifndef UNDEFINED_MACRO\n");
        const QString partToInclude("/* should be included */\n");
        const QString footer("#endif\n");

        const QString expected = partToInclude + "\n";
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

    void throwsExceptionWhenMisingEndif()
    {
        const QString input("#ifndef UNDEFINED_MACRO\n");
        try {
            preprocessor.process(input);
        } catch (CompilationException &e) {
            CPPUNIT_ASSERT_EQUAL(e.stage(),
                                 CompilationException::PREPROCESSING);
            CPPUNIT_ASSERT(strlen(e.what()) > 0);
            return;
        }
        CPPUNIT_FAIL("CompilationException expected when preprocessing program "
                "with missing #endif.");
    }

    void excludesIfdefPartIfMacroNotDefined()
    {
        const QString input(
                "#ifdef UNDEFINED_MACRO\n"
                "/* should be excluded */\n"
                "#endif\n");
        const QString output("\n");
        testProcessing(input, output);
    }

    void macrosClearedBeforeParsingNextProgram()
    {
        preprocessor.process("#define SHOULD_BE_UNDEFINED\n");
        
        const QString checkInput(
                "#ifdef SHOULD_BE_UNDEFINED\n"
                "/* should be excluded */\n"
                "#endif\n");
        testProcessing(checkInput, "\n");
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
        testProcessing(input, "\n");
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
                "    /* include 1 */\n\n"
                "    /* include 2 */\n\n");
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
        const QString expectedOutput("\n    /* include */\n\n");
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
        const QString expectedOutput("    /* include */\n\n");
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
            "\n    /* include 1 */\n\n"
            "        /* include 2 */\n\n"
            "    /* include 3 */\n"
            "        /* include 4 */\n\n"
            "    /* include 5 */\n\n");
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
        testProcessing(input, "\n");
    }


    CPPUNIT_TEST_SUITE(ASLPreprocessorTest);
    CPPUNIT_TEST(doesNotChangeShaderWithoutPreprocessorDirectives);
    CPPUNIT_TEST(excludesIfNDefPartIfMacroIsDefined);
    CPPUNIT_TEST(includesIfNDefPartIfMacroIsNotDefined);
    CPPUNIT_TEST(ignoresPreprocessorDirectivesInMultilineComment);
    CPPUNIT_TEST(ignoresPreprocessorDirectivesInSinglelineComment);
    CPPUNIT_TEST(ignoresPreprocessorDirectivesIfNotFirstNonWhitespaceInLine);
    CPPUNIT_TEST(throwsExceptionWhenMisingEndif);
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
    CPPUNIT_TEST_SUITE_END();

private:
    void testProcessing(const QString &input, const QString &expectedOutput)
    {
        CPPUNIT_ASSERT_EQUAL(expectedOutput.toStdString(),
                preprocessor.process(input).toStdString());
    }

    asl::ASLPreprocessor preprocessor;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::ASLPreprocessorTest);

using namespace asl;

