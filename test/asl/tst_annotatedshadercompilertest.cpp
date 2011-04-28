#include <QtCore/QScopedPointer>
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
    void shaderNameDefaultsToFilename();
    void throwsExceptionWhenCompilingInvalidShader();

private:
    static const QString trivialShader;

    QGLPixelBuffer pixelBufferForGLContext;
    asl::AnnotatedShaderCompiler shaderCompiler;
};
}

using namespace asl;

const QString AnnotatedShaderLoaderTest::trivialShader("void main() { }");

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
    QScopedPointer<AnnotatedGLShaderProgram> compiled(
            shaderCompiler.compile(QGLShader::Fragment, trivialShader));
    QVERIFY(!compiled.isNull());
    QVERIFY2(compiled->log().isEmpty(), compiled->log().toAscii());
    QVERIFY(compiled->isLinked());
}

void AnnotatedShaderLoaderTest::shaderNameDefaultsToFilename()
{
    const QString filename("filename.fs");
    const QString path("path/to/" + filename);
    QScopedPointer<AnnotatedGLShaderProgram> compiled(
            shaderCompiler.compile(QGLShader::Fragment, trivialShader,
                path));
    QCOMPARE(compiled->name(), filename);
}

void AnnotatedShaderLoaderTest::throwsExceptionWhenCompilingInvalidShader()
{
    const QString invalidShader("invalid main() { }");
    try {
        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderCompiler.compile(QGLShader::Fragment, invalidShader));
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
