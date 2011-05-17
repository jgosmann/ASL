
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtOpenGL/QGLPixelBuffer>

#include "../src/asl/aslcompiler.h"

#include "common.h"

#include "logassertions.h"
#include "logentry.h"

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
        shaderCompiler.compile(QGLShader::Fragment, invalidShader);
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

    CPPUNIT_TEST_SUITE(ASLCompilerTest);
    CPPUNIT_TEST(logsErrorWhenCompilingInvalidShader);
    CPPUNIT_TEST(resetsStateBeforeCompiling);
    CPPUNIT_TEST(compilesAndLinksTrivialShader);
    CPPUNIT_TEST(shaderNameDefaultsToFilename);
    CPPUNIT_TEST(shaderDescriptionDefaultsToEmptyString);
    CPPUNIT_TEST(parsesShaderName);
    CPPUNIT_TEST(parsesShaderDescription);
    CPPUNIT_TEST(removesLeadingAndTrailingAteriskInAslCommentLine);
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

