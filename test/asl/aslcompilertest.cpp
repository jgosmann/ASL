
#include <QtCore/QScopedPointer>
#include <QtCore/QString>
#include <QtOpenGL/QGLPixelBuffer>

#include "../src/asl/aslcompiler.h"
#include "../src/asl/compilationexception.h"

#include "common.h"

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

    void throwsExceptionWhenCompilingInvalidShader()
    {
        const QString invalidShader("invalid main() { }");
        try {
            shaderCompiler.compile(QGLShader::Fragment, invalidShader);
        } catch (CompilationException &e) {
            CPPUNIT_ASSERT_EQUAL(e.stage(),
                                 CompilationException::COMPILATION);
            CPPUNIT_ASSERT(strlen(e.what()) > 0);
            return;
        }
        CPPUNIT_FAIL("CompilationException expected when compiling shader "
            "with invalid syntax.");
    }

    void compilesAndLinksTrivialShader()
    {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
            shaderCompiler.compile(QGLShader::Fragment, trivialShader));
        CPPUNIT_ASSERT(!compiled.isNull());
        CPPUNIT_ASSERT_MESSAGE(compiled->log().toStdString(),
                               compiled->log().isEmpty());
        CPPUNIT_ASSERT(compiled->isLinked());
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

    CPPUNIT_TEST_SUITE(ASLCompilerTest);
    CPPUNIT_TEST(throwsExceptionWhenCompilingInvalidShader);
    CPPUNIT_TEST(compilesAndLinksTrivialShader);
    CPPUNIT_TEST(shaderNameDefaultsToFilename);
    CPPUNIT_TEST(shaderDescriptionDefaultsToEmptyString);
    CPPUNIT_TEST(parsesShaderName);
    CPPUNIT_TEST_SUITE_END();

private:
    static const QString trivialShader;

    QGLPixelBuffer pixelBufferForGLContext;
    asl::ASLCompiler shaderCompiler;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::ASLCompilerTest);

using namespace asl;

const QString ASLCompilerTest::trivialShader("void main() { }");

