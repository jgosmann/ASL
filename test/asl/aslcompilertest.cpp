
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtOpenGL/QGLPixelBuffer>

#include "../src/asl/aslcompiler.h"

#include "../testenv.h"

#include "logassertions.h"
#include "logentry.h"
#include "shaderparameterinfomatcher.h"

namespace asl
{
class ASLCompilerTest : public TestFixture
{
public:
    ASLCompilerTest() : pixelBufferForGLContext(1, 1) { }

    void setUp()
    {
        pixelBufferForGLContext.makeCurrent();
    }

    void logsErrorWhenCompilingInvalidShader()
    {
        shaderCompiler.compile(QGLShader::Fragment, invalidShader);
        assertFailedAndLogged(LogEntry().withType(LOG_ERROR).occuringIn(0));
    }

    void resetsStateBeforeCompiling()
    {
        QScopedPointer<AnnotatedGLShaderProgram> createsLogEntries(
                shaderCompiler.compile(QGLShader::Fragment,
                   "/** unknownKey: foo */\n" + invalidShader));
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment, trivialShader));
        assertCleanCompilation(compiled.data());
    }

    void compilesAndLinksTrivialShader()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
            shaderCompiler.compile(QGLShader::Fragment, trivialShader));
        assertCleanCompilation(compiled.data());
    }

    void shaderNameDefaultsToFilename()
    {
        const QString filename("filename.fs");
        const QString path("path/to/" + filename);
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
            shaderCompiler.compile(QGLShader::Fragment, trivialShader,
                path));
        CPPUNIT_ASSERT_EQUAL(filename, compiled->name());
    }

    void shaderDescriptionDefaultsToEmptyString()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment, trivialShader));
        CPPUNIT_ASSERT_EQUAL(QString(""), compiled->description());
    }

    void parsesShaderName()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/** ShaderName: test-shader */\n"
                    + trivialShader));
        CPPUNIT_ASSERT_EQUAL(QString("test-shader"), compiled->name());
    }

    void parsesShaderDescription()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/** ShaderDescription: Some description. */\n"
                    + trivialShader));
        CPPUNIT_ASSERT_EQUAL(QString("Some description."),
                compiled->description());
    }

    void removesLeadingAndTrailingAteriskInAslCommentLine()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/**\n"
                    " * ShaderName: test-shader *\n"
                    " */\n"
                    + trivialShader));
        CPPUNIT_ASSERT_EQUAL(QString("test-shader"), compiled->name());
    }

    void parsesMultilineStrings()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/**\n"
                    " * ShaderDescription: Line 1.\n"
                    " *     Line 2. *\n"
                    " * \t Line 3.\n"
                    "    ShaderName: Line 4.\n"
                    " */\n"
                    + trivialShader));
        CPPUNIT_ASSERT_EQUAL(
                QString("Line 1. Line 2. Line 3. ShaderName: Line 4."),
                compiled->description());
    }

    void parsesMultilineStringsWithNoInitialIndentation()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/**\n"
                    "ShaderDescription: Line 1.\n"
                    " *     Line 2. *\n"
                    " * \t Line 3.\n"
                    " */\n"
                    + trivialShader));
        CPPUNIT_ASSERT_EQUAL(QString("Line 1. Line 2. Line 3."),
                compiled->description());
    }

    void parsesTwoKeysWithSameIdentation()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/**\n"
                    " * ShaderName: name\n"
                    " * ShaderDescription: desc\n"
                    " */\n"
                    + trivialShader));
        assertCleanCompilation(compiled.data());
        CPPUNIT_ASSERT_EQUAL(QString("name"), compiled->name());
        CPPUNIT_ASSERT_EQUAL(QString("desc"), compiled->description());
    }

    void warnsAboutUnknownKeys()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/** unknownKey: value */\n"
                    + trivialShader));
        CPPUNIT_ASSERT(shaderCompiler.success());
        asl::assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).occuringAt(0, 1)
                    .withMessageMatching(QRegExp(".*unknown.*unknownKey.*")));
    }

    void warnsAboutDuplicateKeys()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/**\n"
                    " * ShaderName: name1\n"
                    " * ShaderName: name2\n"
                    " */\n"
                    + trivialShader));
        CPPUNIT_ASSERT(shaderCompiler.success());
        asl::assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).occuringAt(0, 3)
                    .withMessageMatching(QRegExp(".*duplicate.*ShaderName.*")));
    }

    void parsesParameterName()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/**\n"
                    " * Name: test-param\n"
                    " */\n"
                    "uniform int param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withName("test-param"));
    }

    void defaultsParameterNameToIdentifier()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/***/\n"
                    "uniform int param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withName("param"));

    }

    void parsesParameterType()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/***/\n"
                    "uniform int param;\n"
                    +trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withType(GLTypeInfo::getFor("int")));
    }

    void doesNotLeakAnnotationsFromPreviousCompilation()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled1(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/**\n"
                    " * ShaderName: nameOfWrongShader\n"
                    " * ShaderDescription: Description of wrong shader.\n"
                    " */\n"
                    "/***/\n"
                    "uniform int param;\n"
                    + trivialShader));
        QScopedPointer<AnnotatedGLShaderProgram> compiled2(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n" + trivialShader));
        CPPUNIT_ASSERT_EQUAL(QString(""), compiled2->description());
        CPPUNIT_ASSERT_EQUAL(0, compiled2->parameters().size());
    }

    void logsWarningIfAnnotatedUnsupportedType()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/***/\n"
                    "uniform sampler1D unsupported;\n"
                    + trivialShader));
        assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).occuringAt(0, 3));
    }

    void logsWarningIfAslProgramStartsNotWithAslComment()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "uniform int foo;\n"
                    "/***/\n"
                    + trivialShader));
        assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).withMessageMatching(
                    QRegExp(".*not starting with ASL comment.*")));
    }

    CPPUNIT_TEST_SUITE(ASLCompilerTest);
    CPPUNIT_TEST(logsErrorWhenCompilingInvalidShader);
    CPPUNIT_TEST(resetsStateBeforeCompiling);
    CPPUNIT_TEST(compilesAndLinksTrivialShader);
    CPPUNIT_TEST(shaderNameDefaultsToFilename);
    CPPUNIT_TEST(shaderDescriptionDefaultsToEmptyString);
    CPPUNIT_TEST(parsesShaderName);
    CPPUNIT_TEST(parsesShaderDescription);
    CPPUNIT_TEST(removesLeadingAndTrailingAteriskInAslCommentLine);
    CPPUNIT_TEST(parsesMultilineStrings);
    CPPUNIT_TEST(parsesMultilineStringsWithNoInitialIndentation);
    CPPUNIT_TEST(parsesTwoKeysWithSameIdentation);
    CPPUNIT_TEST(warnsAboutUnknownKeys);
    CPPUNIT_TEST(warnsAboutDuplicateKeys);
    CPPUNIT_TEST(parsesParameterName);
    CPPUNIT_TEST(defaultsParameterNameToIdentifier);
    CPPUNIT_TEST(parsesParameterType);
    CPPUNIT_TEST(doesNotLeakAnnotationsFromPreviousCompilation);
    CPPUNIT_TEST(logsWarningIfAnnotatedUnsupportedType);
    CPPUNIT_TEST(logsWarningIfAslProgramStartsNotWithAslComment);
    CPPUNIT_TEST_SUITE_END();

private:
    void assertCleanCompilation(const AnnotatedGLShaderProgram *program)
    {
        CPPUNIT_ASSERT(program);
        CPPUNIT_ASSERT_MESSAGE(qPrintable(shaderCompiler.log()),
                shaderCompiler.log().isEmpty());
        CPPUNIT_ASSERT_MESSAGE(qPrintable(program->log()),
                program->log().isEmpty());
        CPPUNIT_ASSERT(shaderCompiler.success());
        CPPUNIT_ASSERT(program->isLinked());
    }

    void assertFailedAndLogged(const asl::LogEntry &entry)
    {
        CPPUNIT_ASSERT(!shaderCompiler.success());
        asl::assertLogContains(shaderCompiler.log(), entry);
    }

    void assertHasExactlyOneParameterMatching(
            const AnnotatedGLShaderProgram *program,
            const ShaderParameterInfoMatcher &matcher)
    {
        CPPUNIT_ASSERT_EQUAL(1, program->parameters().size());
        CPPUNIT_ASSERT(matcher.matches(program->parameters()[0]));
    }

    static const QString trivialShader;
    static const QString invalidShader;
    static const QString LOG_ERROR;
    static const QString LOG_WARNING;

    QGLPixelBuffer pixelBufferForGLContext;
    asl::ASLCompiler shaderCompiler;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::ASLCompilerTest);

using namespace asl;

const QString ASLCompilerTest::trivialShader("void main() { }");
const QString ASLCompilerTest::invalidShader("invalid main() { }");
const QString ASLCompilerTest::LOG_ERROR("ERROR");
const QString ASLCompilerTest::LOG_WARNING("WARNING");

