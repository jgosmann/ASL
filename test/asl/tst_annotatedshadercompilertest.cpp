#include <QtCore/QString>
#include <QtOpenGL/QGLPixelBuffer>
#include <QtTest/QtTest>

#include "../src/asl/annotatedshadercompiler.h"

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

QTEST_MAIN(AnnotatedShaderLoaderTest);

#include "tst_annotatedshaderloadertest.moc"
