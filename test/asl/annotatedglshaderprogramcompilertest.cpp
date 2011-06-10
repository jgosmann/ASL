
#include "../src/asl/annotatedglshaderprogramcompiler.h"

#include "../testenv.h"

#include "annotatedglshadercompilermock.h"

#include <QGLPixelBuffer>
#include <QScopedPointer>

namespace asl
{
/**
 * \note \c uint value parameters will not be tested as they need a recent GLSL
 * version and we want to be able to run the tests on as many systems as
 * possible.
 */
class AnnotatedGLShaderCompilerTest : public TestFixture
{
public:
    AnnotatedGLShaderCompilerTest()
        : pixelBufferForGLContext(1, 1), stdType(QGLShader::Fragment) { }

    void setUp()
    {
        pixelBufferForGLContext.makeCurrent();
        shaderProgramCompiler = new AnnotatedGLShaderProgramCompiler(
                shaderCompilerMock);
    }

    void tearDown()
    {
        CPPUNIT_ASSERT(Mock::VerifyAndClearExpectations(&shaderCompilerMock));
        delete shaderProgramCompiler;
    }

    void addsMainShaderToProgram()
    {
        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader();

        EXPECT_CALL(shaderCompilerMock, compileFile(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, filename));

        assertCleanCompilationAndLinkage(compiled.data());
        CPPUNIT_ASSERT(compiled->shaders().contains(dummyShader));
    }

    void cachesCompiledShader()
    {
        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader();

        EXPECT_CALL(shaderCompilerMock, compileFile(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));

        QScopedPointer<AnnotatedGLShaderProgram> compiled1(
                shaderProgramCompiler->compileFile(stdType, filename));
        QScopedPointer<AnnotatedGLShaderProgram> compiled2(
                shaderProgramCompiler->compileFile(stdType, filename));

        assertCleanCompilationAndLinkage(compiled1.data());
        assertCleanCompilationAndLinkage(compiled2.data());
        CPPUNIT_ASSERT(compiled1->shaders().contains(dummyShader));
        CPPUNIT_ASSERT(compiled2->shaders().contains(dummyShader));
    }



    CPPUNIT_TEST_SUITE(AnnotatedGLShaderCompilerTest);
    CPPUNIT_TEST(addsMainShaderToProgram);
    CPPUNIT_TEST(cachesCompiledShader);
    CPPUNIT_TEST_SUITE_END();
    // TODO loads dependency, duplicates just once
    // TODO top annotations match
    // TODO passes errors/warnings on

private:
    void assertCleanCompilationAndLinkage(AnnotatedGLShaderProgram *program)
    {
        CPPUNIT_ASSERT(shaderProgramCompiler->success());
        CPPUNIT_ASSERT(shaderProgramCompiler->log().isEmpty());
        CPPUNIT_ASSERT(program->log().isEmpty());
        CPPUNIT_ASSERT(program->isLinked());
    }

    AnnotatedGLShader * createDummyShader()
    {
        AnnotatedGLShader *shader =
            new AnnotatedGLShader(stdType, ShaderInfo());
        shader->compileSourceCode("void main() { }");
        return shader;
    }

    QGLPixelBuffer pixelBufferForGLContext;
    const QGLShader::ShaderType stdType;
    AnnotatedGLShaderCompilerMock shaderCompilerMock;
    AnnotatedGLShaderProgramCompiler *shaderProgramCompiler;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::AnnotatedGLShaderCompilerTest);

using namespace asl;

