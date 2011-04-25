#include <QtCore/QString>
#include <QtOpenGL/QGLPixelBuffer>
#include <QtTest/QtTest>

#include "../src/asl/annotatedshadercompiler.h"
#include "../src/asl/compilationexception.h"

namespace asl
{
class AnnotatedShaderLoaderTest : public QObject
{
    Q_OBJECT

public:
    AnnotatedShaderLoaderTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void compilesAndLinksTrivialShader();
    void throwsExceptionWhenCompilingInvalidShader();

private:
    QGLPixelBuffer pixelBufferForGLContext;
    asl::AnnotatedShaderCompiler shaderCompiler;
};
}

using namespace asl;

AnnotatedShaderLoaderTest::AnnotatedShaderLoaderTest()
    : pixelBufferForGLContext(1, 1)
{
}

void AnnotatedShaderLoaderTest::initTestCase()
{
    pixelBufferForGLContext.makeCurrent();
}

void AnnotatedShaderLoaderTest::cleanupTestCase()
{
}

void AnnotatedShaderLoaderTest::compilesAndLinksTrivialShader()
{
    const QString trivialShader("void main() { }");
    AnnotatedGLShaderProgram *compiled =
            shaderCompiler.compile(QGLShader::Fragment, trivialShader);
    QVERIFY(compiled != NULL);
    QVERIFY2(compiled->log().isEmpty(), compiled->log().toAscii());
    QVERIFY(compiled->isLinked());
    if (compiled)
        delete compiled;
}

void AnnotatedShaderLoaderTest::throwsExceptionWhenCompilingInvalidShader()
{
    const QString invalidShader("invalid main() { }");
    try {
        AnnotatedGLShaderProgram *compiled =
                shaderCompiler.compile(QGLShader::Fragment, invalidShader);
        delete compiled;
    } catch (CompilationException &e) {
        QVERIFY(e.stage() == CompilationException::COMPILATION);
        QVERIFY(strlen(e.what()) > 0);
        return;
    }
    QFAIL("CompilationException expected when compiling shader with invalid "
            "syntax");
}

QTEST_MAIN(AnnotatedShaderLoaderTest);

#include "tst_annotatedshadercompilertest.moc"
