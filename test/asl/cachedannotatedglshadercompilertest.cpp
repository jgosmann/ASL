
#include "../testenv.h"

#include "annotatedglshadercompilermock.h"

#include "../../src/asl/cachedannotatedglshadercompiler.h"

namespace asl
{
class CachedAnnotatedGLShaderCompilerTest : public TestFixture
{
public:
    CachedAnnotatedGLShaderCompilerTest() : stdType(QGLShader::Fragment)
    {
        ON_CALL(compilerMock, success()).WillByDefault(Return(true));
        ON_CALL(compilerMock, log()).WillByDefault(Return(QString("")));
    }

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

    void doesNotCacheFailedCompilations()
    {
        QString filename("filename");

        Expectation failingCall = EXPECT_CALL(compilerMock,
            compileFile(stdType, filename))
            .Times(1).WillOnce(Return(createDummyShader()));
        EXPECT_CALL(compilerMock, success()).After(failingCall)
            .WillRepeatedly(Return(false));
        Expectation successfulCall = EXPECT_CALL(compilerMock,
            compileFile(stdType, filename)).Times(1).After(failingCall)
            .WillOnce(Return(createDummyShader()));
        EXPECT_CALL(compilerMock, success()).After(successfulCall)
            .WillRepeatedly(Return(true));

        QSharedPointer<AnnotatedGLShader> compiled1 =
                cachedCompiler->compileFile(stdType, filename);
        QSharedPointer<AnnotatedGLShader> compiled2 =
                cachedCompiler->compileFile(stdType, filename);
    }

    void compileFileAsMainCompilesShader()
    {
        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader();

        EXPECT_CALL(compilerMock, compileFileAsMain(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));

        QSharedPointer<AnnotatedGLShader> compiled =
                cachedCompiler->compileFileAsMain(stdType, filename);

        CPPUNIT_ASSERT_EQUAL(dummyShader, compiled.data());
    }

    void compileFileAsMainCachesCompiledShader()
    {
        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader();

        EXPECT_CALL(compilerMock, compileFileAsMain(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));

        QSharedPointer<AnnotatedGLShader> compiled1 =
                cachedCompiler->compileFileAsMain(stdType, filename);
        QSharedPointer<AnnotatedGLShader> compiled2 =
                cachedCompiler->compileFileAsMain(stdType, filename);

        CPPUNIT_ASSERT_EQUAL(dummyShader, compiled1.data());
        CPPUNIT_ASSERT_EQUAL(dummyShader, compiled2.data());
    }

    void doesNotCacheFailedAsMainCompilations()
    {
        QString filename("filename");

        Expectation failingCall = EXPECT_CALL(compilerMock,
            compileFileAsMain(stdType, filename))
            .Times(1).WillOnce(Return(createDummyShader()));
        EXPECT_CALL(compilerMock, success()).After(failingCall)
            .WillRepeatedly(Return(false));
        Expectation successfulCall = EXPECT_CALL(compilerMock,
            compileFileAsMain(stdType, filename)).Times(1).After(failingCall)
            .WillOnce(Return(createDummyShader()));
        EXPECT_CALL(compilerMock, success()).After(successfulCall)
            .WillRepeatedly(Return(true));

        QSharedPointer<AnnotatedGLShader> compiled1 =
                cachedCompiler->compileFileAsMain(stdType, filename);
        QSharedPointer<AnnotatedGLShader> compiled2 =
                cachedCompiler->compileFileAsMain(stdType, filename);
    }

    void doesNotMixMainAndNonMainShadersInCache()
    {
        QString filename("filename");

        EXPECT_CALL(compilerMock, compileFile(stdType, filename))
            .Times(1).WillRepeatedly(Return(createDummyShader()));
        EXPECT_CALL(compilerMock, compileFileAsMain(stdType, filename))
            .Times(1).WillRepeatedly(Return(createDummyShader()));

        QSharedPointer<AnnotatedGLShader> compiled1 =
                cachedCompiler->compileFile(stdType, filename);
        QSharedPointer<AnnotatedGLShader> compiled2 =
                cachedCompiler->compileFileAsMain(stdType, filename);

        CPPUNIT_ASSERT(compiled1.data() != compiled2.data());
    }

    void returningCachedShaderResetsSuccessAndLog()
    {
        EXPECT_CALL(compilerMock, compileFile(stdType, QString("succesful")))
            .Times(1).WillOnce(Return(createDummyShader()));
        const QString failingShader("failing");
        Expectation failingShaderCompiled =
            EXPECT_CALL(compilerMock, compileFile(stdType, failingShader))
                .WillOnce(Return(createDummyShader()));

        EXPECT_CALL(compilerMock, success()).WillRepeatedly(Return(true));
        EXPECT_CALL(compilerMock, success()).Times(AtLeast(1)).After(failingShaderCompiled)
            .WillRepeatedly(Return(false));

        QSharedPointer<AnnotatedGLShader> compiled =
                cachedCompiler->compileFile(stdType, "succesful");
        cachedCompiler->compileFile(stdType, failingShader);
        CPPUNIT_ASSERT(!cachedCompiler->success());
        QSharedPointer<AnnotatedGLShader> compiled2 =
                cachedCompiler->compileFile(stdType, "succesful");
        CPPUNIT_ASSERT(cachedCompiler->success());
    }

    CPPUNIT_TEST_SUITE(CachedAnnotatedGLShaderCompilerTest);
    CPPUNIT_TEST(compileFileCompilesShader);
    CPPUNIT_TEST(compileFileCachesCompiledShader);
    CPPUNIT_TEST(doesNotCacheFailedCompilations);
    CPPUNIT_TEST(compileFileAsMainCompilesShader);
    CPPUNIT_TEST(compileFileAsMainCachesCompiledShader);
    CPPUNIT_TEST(doesNotCacheFailedAsMainCompilations);
    CPPUNIT_TEST(doesNotMixMainAndNonMainShadersInCache);
    CPPUNIT_TEST(returningCachedShaderResetsSuccessAndLog);
    CPPUNIT_TEST_SUITE_END();

private:
    AnnotatedGLShader * createDummyShader()
    {
        return new AnnotatedGLShader(stdType, ShaderInfo());
    }

    const QGLShader::ShaderType stdType;
    NiceMock<AnnotatedGLShaderCompilerMock> compilerMock;
    CachedAnnotatedGLShaderCompiler *cachedCompiler;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::CachedAnnotatedGLShaderCompilerTest);

