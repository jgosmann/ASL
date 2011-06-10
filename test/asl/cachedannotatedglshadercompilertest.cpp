
#include "../testenv.h"

#include "annotatedglshadercompilermock.h"

#include "../../src/asl/cachedannotatedglshadercompiler.h"

namespace asl
{
class CachedAnnotatedGLShaderCompilerTest : public TestFixture
{
public:
    CachedAnnotatedGLShaderCompilerTest() : stdType(QGLShader::Fragment) { }

    void setUp()
    {
        cachedCompiler = new CachedAnnotatedGLShaderCompiler(compilerMock);
    }

    void tearDown()
    {
        CPPUNIT_ASSERT(Mock::VerifyAndClearExpectations(&compilerMock));
        delete cachedCompiler;
    }

    void compileFileCompilesShader()
    {
        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader();

        EXPECT_CALL(compilerMock, compileFile(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));

        QSharedPointer<AnnotatedGLShader> compiled =
                cachedCompiler->compileFile(stdType, filename);

        CPPUNIT_ASSERT_EQUAL(dummyShader, compiled.data());
    }

    void compileFileCachesCompiledShader()
    {
        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader();

        EXPECT_CALL(compilerMock, compileFile(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));

        QSharedPointer<AnnotatedGLShader> compiled1 =
                cachedCompiler->compileFile(stdType, filename);
        QSharedPointer<AnnotatedGLShader> compiled2 =
                cachedCompiler->compileFile(stdType, filename);

        CPPUNIT_ASSERT_EQUAL(dummyShader, compiled1.data());
        CPPUNIT_ASSERT_EQUAL(dummyShader, compiled2.data());
    }

    CPPUNIT_TEST_SUITE(CachedAnnotatedGLShaderCompilerTest);
    CPPUNIT_TEST(compileFileCompilesShader);
    CPPUNIT_TEST(compileFileCachesCompiledShader);
    CPPUNIT_TEST_SUITE_END();

private:
    AnnotatedGLShader * createDummyShader()
    {
        return new AnnotatedGLShader(stdType, ShaderInfo());
    }

    const QGLShader::ShaderType stdType;
    AnnotatedGLShaderCompilerMock compilerMock;
    CachedAnnotatedGLShaderCompiler *cachedCompiler;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::CachedAnnotatedGLShaderCompilerTest);

