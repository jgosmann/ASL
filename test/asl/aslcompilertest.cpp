
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtOpenGL/QGLPixelBuffer>

#include "../src/asl/aslcompiler.h"
#include "../src/asl/gltypenames.h"

#include "../testenv.h"

#include "dependencyreadermock.h"
#include "logassertions.h"
#include "logentry.h"
#include "shaderparameterinfomatcher.h"

namespace asl
{
/**
 * \note \c uint value parameters will not be tested as they need a recent GLSL
 * version and we want to be able to run the tests on as many systems as
 * possible.
 */
class ASLCompilerTest : public TestFixture
{
public:
    ASLCompilerTest() : pixelBufferForGLContext(1, 1),
            shaderCompiler(dependencyReader) { }

    void setUp()
    {
        pixelBufferForGLContext.makeCurrent();
    }

    void tearDown()
    {
        CPPUNIT_ASSERT(Mock::VerifyAndClearExpectations(&dependencyReader));
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

    void allowsCommentsAndPreprocessorBeforeFirstAslComment()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/* some comment */\n"
                    "// another comment\n"
                    "#define FOO bar\n"
                    "#define MULTILINE 42 \\\n"
                    "    23\n"
                    "/***/\n"
                    + trivialShader));
        assertCleanCompilation(compiled.data());
    }

    void allowCommentsInAnnotatedUniform()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/***/\n"
                    "uniform /* comment */ int /* more\n"
                    "    comment */ foo // c\n"
                    " ; /* c */\n"
                    + trivialShader));
        assertCleanCompilation(compiled.data());
        ShaderParameterInfoMatcher parameter;
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withType(GLTypeInfo::getFor("int")));
    }

    void linePreprocessorDirectiveSetsLine()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "#line 23\n"
                    "/**\n"
                    " * ShaderName: name1\n"
                    " * ShaderName: name2\n"
                    " */\n"
                    + trivialShader));
        CPPUNIT_ASSERT(shaderCompiler.success());
        assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).occuringAt(0, 26));
    }

    void linePreprocessorDirectiveSetsLineAndSourcestringNo()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "#line 23 42\n"
                    "/**\n"
                    " * ShaderName: name1\n"
                    " * ShaderName: name2\n"
                    " */\n"
                    + trivialShader));
        CPPUNIT_ASSERT(shaderCompiler.success());
        assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).occuringAt(42, 26));
    }

    void warnsAboutAslCommontNotPrecedingUniform()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/***/\n"
                    + trivialShader));
        assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).occuringAt(0, 3)
                    .withMessageMatching(QRegExp(
                            ".*not preceding valid uniform.*")));
    }

    void warnsAboutAndDoesNotInterpretGeneralAnnotationsInUniformAslComment()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/** ShaderName: real-name */\n"
                    "/** ShaderName: do-not-use-this-name */\n"
                    "uniform int param;\n"
                    + trivialShader));
        CPPUNIT_ASSERT_EQUAL(QString("real-name"), compiled->name());
        assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).occuringAt(0, 2));
    }

    void warnsAboutAndDoesNotInterpretUniformAnnotationsinStartAslComment()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/** Name: do-not-use */\n"
                    "uniform int param;\n"
                    + trivialShader));
        CPPUNIT_ASSERT_EQUAL(0, compiled->parameters().size());
        assertLogContains(shaderCompiler.log(),
                LogEntry().withType(LOG_WARNING).occuringAt(0, 1));
    }

    void allowsNonAnnotatingTextInAslComments()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/**\n"
                    " * some text\n"
                    " * ShaderName: name\n"
                    " * some more text containing colon : foo\n"
                    " */\n"
                    + trivialShader));
        assertCleanCompilation(compiled.data());
        CPPUNIT_ASSERT_EQUAL(QString("name"), compiled->name());
    }

    void parsesIdentifier()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/***/\n"
                    "uniform int nameOfIdentifier;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withIdentifier("nameOfIdentifier"));
    }

    void defaultsParameterDescriptionToEmptyString()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    aslShaderWithOneAslParameterWithoutAnnotations));
        ShaderParameterInfoMatcher parameter;
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withDescription(""));
    }

    void parsesParameterDescription()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Description: desc */\n"
                    "uniform int param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withDescription("desc"));
    }

    void parsesScalarDefaultIntValue()
    {
        testParsingOfDefaultWithScalar("int", "- 42", static_cast<GLint>(-42));
    }

    void parsesScalarDefaultFloatValue()
    {
        testParsingOfDefaultWithScalar("float", "42.25",
                static_cast<GLfloat>(42.25));
    }

    void parsesScalarDefaultNegativeFloatValue()
    {
        testParsingOfDefaultWithScalar("float", "-42.25",
                static_cast<GLfloat>(-42.25));
    }

    void parsesScalarDefaultFloatWithExponentValue()
    {
        testParsingOfDefaultWithScalar("float", "2.3e-3",
                static_cast<GLfloat>(2.3e-3));
    }

    void parsesTrueBoolDefaultValue()
    {
        testParsingOfDefaultWithScalar("bool", "true", static_cast<GLint>(1));
    }

    void parsesfalseBoolDefaultValue()
    {
        testParsingOfDefaultWithScalar("bool", "false", static_cast<GLint>(0));
    }

    void parsesScalarWithCastConstructor()
    {
        testParsingOfDefaultWithScalar("int", "int(2.3)",
                static_cast<GLint>(2));
    }

    void parsesVectorWithReplicationConstructor()
    {
        GLfloat values[] = { 2.3, 2.3, 2.3 };
        testParsingOfDefault("vec3", "vec3(2.3)", GLVariant("vec3", 3, values));
    }

    void parsesVectorWithInitializerList()
    {
        GLfloat values[] = { 3.0, -1.2, 3.4 };
        testParsingOfDefault("vec3", "vec3(3, -1.2, 3.4)",
                GLVariant("vec3", 3, values));
    }

    void warnsIfDefaultIsInitializedWithInvalidType()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Default: foo(3, 42, 1) */\n"
                    "uniform int param;\n"
                    + trivialShader));
        assertLogContains(shaderCompiler.log(),
            LogEntry().withType(LOG_WARNING).occuringAt(0, 2)
                .withMessageMatching(QRegExp(
                    ".*Could not instantiate.*foo.*")));
    }

    void warnsIfPassedIncompatibleNumberOfArgumentsToTypeConstructor()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Default: vec3(1, 1) */\n"
                    "uniform vec3 param;\n"
                    + trivialShader));
        assertLogContains(shaderCompiler.log(),
            LogEntry().withType(LOG_WARNING).occuringAt(0, 2)
                .withMessageMatching(QRegExp(
                    ".*Could not instantiate.*vec3.*")));
    }

    void castsDefaultValueToCorrectType()
    {
        GLfloat values[] = { 42.0, -2.0, 0.0 };
        testParsingOfDefault("vec3", "ivec3(42, -2, 0)",
                GLVariant("vec3", 3, values));
    }

    void warnsOnSyntaxErrorInDefaultAnnotation()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Default: vec3(1, ) */\n"
                    "uniform vec3 param;\n"
                    + trivialShader));
        assertLogContains(shaderCompiler.log(),
            LogEntry().withType(LOG_WARNING).occuringAt(0, 2)
                .withMessageMatching(QRegExp(
                    ".*syntax error.*")));
    }

    void parsesMultilineDefaultAnnotation()
    {
        GLfloat values[] = { 1.3, -3.5, 2.4 };
        testParsingOfDefault("vec3", "vec3(1.3,\n *     -3.5, 2.4)",
                GLVariant("vec3", 3, values));
    }

    template<const char *glslName> void testRangeDefaultsToMinAndMaxOfType()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/***/\n"
                    "uniform " + QString(glslName) + " param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
            parameter.withMinimum(GLVariant::minOfType(glslName))
                .withMaximum(GLVariant::maxOfType(glslName)));
    }

    void testRangeSetsMinAndMaxValue()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Range: vec3(1, 2, 3), vec3(4, 5, 6) */\n"
                    "uniform vec3 param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        GLfloat minValues[] = { 1, 2, 3 };
        GLfloat maxValues[] = { 4, 5, 6 };
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withMinimum(GLVariant("vec3", 3, minValues))
                    .withMaximum(GLVariant("vec3", 3, maxValues)));
    }

    void testRangeCastsMinAndMaxValue()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Range: ivec3(1, 2, 3), ivec3(4, 5, 6) */\n"
                    "uniform vec3 param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        GLfloat minValues[] = { 1, 2, 3 };
        GLfloat maxValues[] = { 4, 5, 6 };
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withMinimum(GLVariant("vec3", 3, minValues))
                    .withMaximum(GLVariant("vec3", 3, maxValues)));
    }

    void testRangeSetsMinAndMaxWithMinMaxIdentifier()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Range: min, max */\n"
                    "uniform vec3 param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
            parameter.withMinimum(GLVariant::minOfType("vec3"))
                .withMaximum(GLVariant::maxOfType("vec3")));
    }

    void testRangeSetsMinAndMaxWithMinMaxIdentifierAndValueMixed()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Range: min, 3 */\n"
                    "uniform int param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        GLint value = 3;
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
            parameter.withMinimum(GLVariant::minOfType("int"))
                .withMaximum(GLVariant("int", 1, &value)));
    }

    void testSingleWordRangeSpecifiers()
    {
        GLfloat zero = 0;
        GLfloat one = 1;
        GLfloat byteMax = 255;
        GLVariant zeroV("float", 1, &zero);
        GLVariant oneV("float", 1, &one);
        GLVariant byteMaxV("float", 1, &byteMax);
        testSingleWordRangeSpecifier("percent", zeroV, oneV);
        testSingleWordRangeSpecifier("byte", zeroV, byteMaxV);
        testSingleWordRangeSpecifier("unsigned", zeroV,
                GLVariant::maxOfType("float"));
        testSingleWordRangeSpecifier("positive", oneV,
                GLVariant::maxOfType("float"));
    }

    void warnsOnDoubleRangeSpecification()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/**\n"
                    " * Range: 0, 3\n"
                    " * Range: percent\n"
                    " */\n"
                    "uniform int param;\n"
                    + trivialShader));
        assertLogContains(shaderCompiler.log(),
            LogEntry().withType(LOG_WARNING).occuringAt(0, 4)
                .withMessageMatching(QRegExp(".*duplicate.*Range.*")));
    }

    void defaultsControlAnnotationToEmptyList()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/***/\n"
                    "uniform float param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withPreferredUIControls(QStringList()));
    }

    void parsesControlAnnotation()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Control: color\t, default */\n"
                    "uniform vec3 param;\n"
                    + trivialShader));
        QStringList controls;
        controls.append("color");
        controls.append("default");
        ShaderParameterInfoMatcher parameter;
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withPreferredUIControls(controls));
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
    CPPUNIT_TEST(allowsCommentsAndPreprocessorBeforeFirstAslComment);
    CPPUNIT_TEST(allowCommentsInAnnotatedUniform);
    CPPUNIT_TEST(linePreprocessorDirectiveSetsLine);
    CPPUNIT_TEST(linePreprocessorDirectiveSetsLineAndSourcestringNo);
    CPPUNIT_TEST(warnsAboutAslCommontNotPrecedingUniform);
    CPPUNIT_TEST(
            warnsAboutAndDoesNotInterpretGeneralAnnotationsInUniformAslComment);
    CPPUNIT_TEST(
            warnsAboutAndDoesNotInterpretUniformAnnotationsinStartAslComment);
    CPPUNIT_TEST(allowsNonAnnotatingTextInAslComments);
    CPPUNIT_TEST(parsesIdentifier);
    CPPUNIT_TEST(defaultsParameterDescriptionToEmptyString);
    CPPUNIT_TEST(parsesParameterDescription);
    CPPUNIT_TEST(parsesScalarDefaultIntValue);
    CPPUNIT_TEST(parsesScalarDefaultFloatValue);
    CPPUNIT_TEST(parsesScalarDefaultNegativeFloatValue);
    CPPUNIT_TEST(parsesScalarDefaultFloatWithExponentValue);
    CPPUNIT_TEST(parsesTrueBoolDefaultValue);
    CPPUNIT_TEST(parsesfalseBoolDefaultValue);
    CPPUNIT_TEST(parsesScalarWithCastConstructor);
    CPPUNIT_TEST(parsesVectorWithReplicationConstructor);
    CPPUNIT_TEST(parsesVectorWithInitializerList);
    CPPUNIT_TEST(warnsIfDefaultIsInitializedWithInvalidType);
    CPPUNIT_TEST(warnsIfPassedIncompatibleNumberOfArgumentsToTypeConstructor);
    CPPUNIT_TEST(castsDefaultValueToCorrectType);
    CPPUNIT_TEST(warnsOnSyntaxErrorInDefaultAnnotation);
    CPPUNIT_TEST(parsesMultilineDefaultAnnotation);
    CPPUNIT_TEST(testRangeDefaultsToMinAndMaxOfType<gltypenames::FLOAT>);
    CPPUNIT_TEST(testRangeDefaultsToMinAndMaxOfType<gltypenames::INT>);
    CPPUNIT_TEST(testRangeDefaultsToMinAndMaxOfType<gltypenames::BOOL>);
    CPPUNIT_TEST(testRangeSetsMinAndMaxValue);
    CPPUNIT_TEST(testRangeCastsMinAndMaxValue);
    CPPUNIT_TEST(testRangeSetsMinAndMaxWithMinMaxIdentifier);
    CPPUNIT_TEST(testRangeSetsMinAndMaxWithMinMaxIdentifierAndValueMixed);
    CPPUNIT_TEST(testSingleWordRangeSpecifiers);
    CPPUNIT_TEST(warnsOnDoubleRangeSpecification);
    CPPUNIT_TEST(defaultsControlAnnotationToEmptyList);
    CPPUNIT_TEST(parsesControlAnnotation);
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

    template<class T> void testParsingOfDefaultWithScalar(
            const QString &glslTypename, const QString &srcValue, T expected)
    {
        testParsingOfDefault(glslTypename, srcValue,
                GLVariant(glslTypename, 1, &expected));
    }

    void testParsingOfDefault(const QString &glslTypename,
            const QString &srcValue, const GLVariant &expected)
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Default: " + srcValue + " */\n"
                    "uniform " + glslTypename + " param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withDefaultValue(expected));
    }

    void testSingleWordRangeSpecifier(const QString &specifier,
            const GLVariant &min, const GLVariant &max)
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment,
                    "/***/\n"
                    "/** Range: " + specifier + " */\n"
                    "uniform float param;\n"
                    + trivialShader));
        ShaderParameterInfoMatcher parameter;
        assertCleanCompilation(compiled.data());
        assertHasExactlyOneParameterMatching(compiled.data(),
                parameter.withMinimum(min).withMaximum(max));
    }

    static const QString trivialShader;
    static const QString invalidShader;
    static const QString aslShaderWithOneAslParameterWithoutAnnotations;
    static const QString LOG_ERROR;
    static const QString LOG_WARNING;

    QGLPixelBuffer pixelBufferForGLContext;
    DependencyReaderMock dependencyReader;
    asl::ASLCompiler shaderCompiler;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::ASLCompilerTest);

using namespace asl;

const QString ASLCompilerTest::trivialShader("void main() { }");
const QString ASLCompilerTest::invalidShader("invalid main() { }");
const QString ASLCompilerTest::aslShaderWithOneAslParameterWithoutAnnotations(
        "/***/\n/***/\nuniform int param;\n" + trivialShader);
const QString ASLCompilerTest::LOG_ERROR("ERROR");
const QString ASLCompilerTest::LOG_WARNING("WARNING");

