
#include "../src/asl/aslprogramcompiler.h"
#include "../src/asl/gltypeinfo.h"
#include "../src/asl/shaderparameterinfobuilder.h"

#include "../testenv.h"

#include "annotatedglshadercompilermock.h"
#include "dependencylocatormock.h"
#include "logassertions.h"
#include "logentry.h"
#include "shaderparameterinfomatcher.h"

#include <QGLPixelBuffer>
#include <QScopedPointer>

ACTION_P(ReturnArgPrefixed, pre) { return pre + arg0; }

std::ostream & operator<<(std::ostream &output,
        const asl::ShaderParameterInfo &shaderParamInfo)
{
    return (output << "ShaderParameterInfo(" << shaderParamInfo.identifier
            << ")");
}

namespace asl
{
/**
 * \note \c uint value parameters will not be tested as they need a recent GLSL
 * version and we want to be able to run the tests on as many systems as
 * possible.
 */
class ASLProgramCompilerTest : public TestFixture
{
public:
    ASLProgramCompilerTest()
        : pixelBufferForGLContext(1, 1), stdType(QGLShader::Fragment)
    {
        ON_CALL(dependencyLocatorMock, locate(_, _))
            .WillByDefault(ReturnArg<0>());
        ON_CALL(shaderCompilerMock, log()).WillByDefault(Return(QString()));
        ON_CALL(shaderCompilerMock, success()).WillByDefault(Return(true));
    }

    void setUp()
    {
        pixelBufferForGLContext.makeCurrent();
        shaderProgramCompiler = new ASLProgramCompiler(
                dependencyLocatorMock, shaderCompilerMock);
    }

    void tearDown()
    {
        CPPUNIT_ASSERT(Mock::VerifyAndClearExpectations(&shaderCompilerMock));
        CPPUNIT_ASSERT(Mock::VerifyAndClearExpectations(
                    &dependencyLocatorMock));
        delete shaderProgramCompiler;
    }

    void addsMainShaderToProgram()
    {
        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader();

        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(stdType, filename))
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

        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(stdType, filename))
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

    void loadsDependencies()
    {
        QStringList dependencies;
        dependencies.append("dependency1");
        dependencies.append("dependency2");

        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader(
                "void main() { }", dependencies);
        AnnotatedGLShader *dependency1 = createDummyShader("void a() { }");
        AnnotatedGLShader *dependency2 = createDummyShader("void b() { }");

        EXPECT_CALL(dependencyLocatorMock, locate(_, filename))
            .WillRepeatedly(ReturnArgPrefixed(QString("path/to/")));
        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));
        EXPECT_CALL(shaderCompilerMock,
                compileFile(stdType, QString("path/to/dependency1")))
            .Times(1).WillOnce(Return(dependency1));
        EXPECT_CALL(shaderCompilerMock,
                compileFile(stdType, QString("path/to/dependency2")))
            .Times(1).WillOnce(Return(dependency2));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, filename));

        assertCleanCompilationAndLinkage(compiled.data());
        CPPUNIT_ASSERT(compiled->shaders().contains(dummyShader));
        CPPUNIT_ASSERT(compiled->shaders().contains(dependency1));
        CPPUNIT_ASSERT(compiled->shaders().contains(dependency2));
    }

    void loadsRecursiveDependencies()
    {
        QStringList depsMain, depsSubmain;
        depsMain.append("depMain");
        depsSubmain.append("depSubmain");

        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader(
                "void main() { }", depsMain);
        AnnotatedGLShader *depMain = createDummyShader("void a() { }",
                depsSubmain);
        AnnotatedGLShader *depSubmain = createDummyShader("void b() { }");

        EXPECT_CALL(dependencyLocatorMock, locate(_, _))
            .WillRepeatedly(ReturnArgPrefixed(QString("path/to/")));
        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));
        EXPECT_CALL(shaderCompilerMock,
                compileFile(stdType, QString("path/to/depMain")))
            .Times(1).WillOnce(Return(depMain));
        EXPECT_CALL(shaderCompilerMock,
                compileFile(stdType, QString("path/to/depSubmain")))
            .Times(1).WillOnce(Return(depSubmain));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, filename));

        assertCleanCompilationAndLinkage(compiled.data());
        CPPUNIT_ASSERT(compiled->shaders().contains(dummyShader));
        CPPUNIT_ASSERT(compiled->shaders().contains(depMain));
        CPPUNIT_ASSERT(compiled->shaders().contains(depSubmain));
    }

    void addsDependencyParametersToProgram()
    {
        ShaderParameterInfo parameterInfo;

        QStringList dependencies;
        dependencies.append("dependency");

        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader(
                "void main() { }", dependencies);
        QList<asl::ShaderParameterInfo> parameters;
        parameters.append(parameterInfo);
        AnnotatedGLShader *dependency = createDummyShader(
                "void a() { }", QStringList(), QStringList(), parameters);

        EXPECT_CALL(dependencyLocatorMock, locate(_, filename))
            .WillRepeatedly(ReturnArgPrefixed(QString("path/to/")));
        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));
        EXPECT_CALL(shaderCompilerMock,
                compileFile(stdType, QString("path/to/dependency")))
            .Times(1).WillOnce(Return(dependency));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, filename));

        assertCleanCompilationAndLinkage(compiled.data());
        CPPUNIT_ASSERT(compiled->parameters().contains(parameterInfo));
    }

    void logsCompiledDependencies()
    {
        QStringList depsMain, depsSubmain;
        depsMain.append("depMain");
        depsSubmain.append("depSubmain");

        QString filename("filename");
        AnnotatedGLShader *dummyShader = createDummyShader(
                "void main() { }", depsMain);
        AnnotatedGLShader *depMain = createDummyShader("void a() { }",
                depsSubmain);
        AnnotatedGLShader *depSubmain = createDummyShader("void b() { }");

        EXPECT_CALL(dependencyLocatorMock, locate(_, _))
            .WillRepeatedly(ReturnArgPrefixed(QString("path/to/")));
        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));
        EXPECT_CALL(shaderCompilerMock,
                compileFile(stdType, QString("path/to/depMain")))
            .Times(1).WillOnce(Return(depMain));
        EXPECT_CALL(shaderCompilerMock,
                compileFile(stdType, QString("path/to/depSubmain")))
            .Times(1).WillOnce(Return(depSubmain));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, filename));

        assertCleanCompilationAndLinkage(compiled.data());
        assertLogged(LogEntry().withType(LOG_INFO).occuringIn(0)
                .withMessageMatching(QRegExp(".*depMain.*")));
        assertLogged(LogEntry().withType(LOG_INFO).occuringIn(0)
                .withMessageMatching(QRegExp(".*depSubmain.*")));
    }

    void loadsDuplicatesOnlyOnce()
    {
        QString filename("filename");

        QStringList depsMain, deps1, deps2;
        depsMain.append(filename);
        depsMain.append("deps1");
        depsMain.append("deps1");

        deps1.append(filename);
        deps1.append("deps1");
        deps1.append("deps2");

        deps2.append("deps2");

        AnnotatedGLShader *dummyShader = createDummyShader(
                "void main() { }", depsMain);
        AnnotatedGLShader *dep1 = createDummyShader("void a() { }", deps1);
        AnnotatedGLShader *dep2 = createDummyShader("void b() { }", deps2);

        EXPECT_CALL(dependencyLocatorMock, locate(_, _))
            .WillRepeatedly(ReturnArg<0>());
        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(stdType, filename))
            .Times(1).WillOnce(Return(dummyShader));
        EXPECT_CALL(shaderCompilerMock, compileFile(stdType, QString("deps1")))
            .Times(1).WillOnce(Return(dep1));
        EXPECT_CALL(shaderCompilerMock, compileFile(stdType, QString("deps2")))
            .Times(1).WillOnce(Return(dep2));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, filename));

        assertCleanCompilationAndLinkage(compiled.data());
        CPPUNIT_ASSERT_EQUAL(3, compiled->shaders().size());
    }

    void shaderProgramInfoEqualsMainShaderInfoExceptParameters()
    {
        QString filename("filename");
        ShaderInfo info;
        info.name = "MainShader";
        info.description = "Description";
        info.dependencies.append("dependency");
        ShaderParameterInfoBuilder paramBuilder;
        paramBuilder.withIdentifier("param1");
        paramBuilder.withType(GLTypeInfo::getFor("int"));
        info.parameters.append(paramBuilder.build());

        AnnotatedGLShader *mainShader = new AnnotatedGLShader(stdType, info);
        mainShader->compileSourceCode("void main() { }");

        AnnotatedGLShader *dependency = createDummyShader("void a() { }");

        EXPECT_CALL(dependencyLocatorMock, locate(_, _))
            .WillRepeatedly(ReturnArg<0>());
        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(stdType, filename))
            .WillRepeatedly(Return(mainShader));
        EXPECT_CALL(shaderCompilerMock,
            compileFile(stdType, QString("dependency")))
            .WillRepeatedly(Return(dependency));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, filename));

        assertCleanCompilationAndLinkage(compiled.data());
        CPPUNIT_ASSERT_EQUAL(info.name, compiled->name());
        CPPUNIT_ASSERT_EQUAL(info.description, compiled->description());
        CPPUNIT_ASSERT_EQUAL(info.dependencies, compiled->dependencies());
    }

    void mainShaderWarningsAppearInCompilerLog()
    {
        AnnotatedGLShader *shader = createDummyShader();

        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(_, _))
            .WillRepeatedly(Return(shader));
        EXPECT_CALL(shaderCompilerMock, log()).WillRepeatedly(Return(QString(
                LOG_WARNING + ": 13:37: foobaz")));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, ""));

        CPPUNIT_ASSERT(shaderProgramCompiler->success());
        assertLogged(LogEntry().withType(LOG_WARNING).occuringAt(13, 37)
                .withMessageMatching(QRegExp("foobaz")));
    }

    void dependencyWarningsAppearInCompilerLog()
    {
        QStringList dependencies;
        dependencies.append("dependency");
        AnnotatedGLShader *shader = createDummyShader("void main() { }",
                dependencies);
        AnnotatedGLShader *dependency = createDummyShader("void a() { }");

        InSequence seq;
        EXPECT_CALL(shaderCompilerMock,
            compileFileAsMain(_, Ne(QString("dependency"))))
            .Times(1).WillRepeatedly(Return(shader));
        EXPECT_CALL(shaderCompilerMock, log())
            .WillRepeatedly(Return(QString()));
        EXPECT_CALL(shaderCompilerMock, compileFile(_, QString("dependency")))
            .Times(1).WillRepeatedly(Return(dependency));
        EXPECT_CALL(shaderCompilerMock, log()).WillRepeatedly(Return(QString(
                LOG_WARNING + ": 13:37: foobaz")));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, ""));

        CPPUNIT_ASSERT(shaderProgramCompiler->success());
        assertLogged(LogEntry().withType(LOG_WARNING).occuringAt(13, 37)
                .withMessageMatching(QRegExp("foobaz")));
    }

    void mainShaderErrorsAppearInCompilerLog()
    {
        AnnotatedGLShader *shader = createDummyShader();

        EXPECT_CALL(shaderCompilerMock, compileFileAsMain(_, _))
            .WillRepeatedly(Return(shader));
        EXPECT_CALL(shaderCompilerMock, success())
            .WillRepeatedly(Return(false));
        EXPECT_CALL(shaderCompilerMock, log()).WillRepeatedly(Return(QString(
                LOG_ERROR + ": 13:37: foobaz")));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, ""));

        CPPUNIT_ASSERT(!shaderProgramCompiler->success());
        assertLogged(LogEntry().withType(LOG_ERROR).occuringAt(13, 37)
                .withMessageMatching(QRegExp("foobaz")));
    }

    void dependencyErrorsAppearInCompilerLog()
    {
        QStringList dependencies;
        dependencies.append("dependency");
        AnnotatedGLShader *shader = createDummyShader("void main() { }",
                dependencies);
        AnnotatedGLShader *dependency = createDummyShader("void a() { }");

        Expectation mainCall = EXPECT_CALL(shaderCompilerMock,
            compileFileAsMain(_, Ne(QString("dependency"))))
            .Times(1).WillOnce(Return(shader));
        EXPECT_CALL(shaderCompilerMock, log())
            .WillRepeatedly(Return(QString()));
        EXPECT_CALL(shaderCompilerMock, success()).After(mainCall)
            .WillRepeatedly(Return(true));
        Expectation failingCall = EXPECT_CALL(shaderCompilerMock,
            compileFile(_, QString("dependency"))).Times(1).After(mainCall)
            .WillOnce(Return(dependency));
        EXPECT_CALL(shaderCompilerMock, log()).After(failingCall)
            .WillRepeatedly(Return(QString(LOG_ERROR + ": 13:37: foobaz")));
        EXPECT_CALL(shaderCompilerMock, success()).After(failingCall)
            .WillRepeatedly(Return(false));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, ""));

        CPPUNIT_ASSERT(!shaderProgramCompiler->success());
        assertLogged(LogEntry().withType(LOG_ERROR).occuringAt(13, 37)
                .withMessageMatching(QRegExp("foobaz")));
    }

    void linkerErrorsAppearInCompilerLog()
    {
        QStringList dependencies;
        dependencies.append("dependency");
        AnnotatedGLShader *shader = createDummyShader("void main() { }",
                dependencies);
        AnnotatedGLShader *dependency = createDummyShader("void main() { }");

        EXPECT_CALL(shaderCompilerMock,
            compileFileAsMain(_, Ne(QString("dependency"))))
            .Times(1).WillOnce(Return(shader));
        EXPECT_CALL(shaderCompilerMock,
            compileFile(_, QString("dependency")))
            .Times(1).WillOnce(Return(dependency));

        QScopedPointer<AnnotatedGLShaderProgram> compiled(
                shaderProgramCompiler->compileFile(stdType, ""));

        CPPUNIT_ASSERT(!shaderProgramCompiler->success());
        assertLogContainsEntryOfType(shaderProgramCompiler->log(), LOG_ERROR);
    }

    void resetsErrorStateAndLogBeforeCompilation()
    {
        Expectation first = EXPECT_CALL(shaderCompilerMock,
                compileFileAsMain(_, QString("file1")))
            .WillRepeatedly(Return(createDummyShader()));
        EXPECT_CALL(shaderCompilerMock, log()).After(first)
            .WillRepeatedly(Return(QString("ERROR: 0:0: fubar\n")));
        EXPECT_CALL(shaderCompilerMock, success()).After(first)
            .WillRepeatedly(Return(false));

        Expectation second = EXPECT_CALL(shaderCompilerMock,
                compileFileAsMain(_, QString("file2")))
            .Times(AtLeast(1)).After(first)
            .WillRepeatedly(Return(createDummyShader()));
        EXPECT_CALL(shaderCompilerMock, log()).After(second)
            .WillRepeatedly(Return(QString("")));
        EXPECT_CALL(shaderCompilerMock, success()).After(second)
            .WillRepeatedly(Return(true));

        QScopedPointer<AnnotatedGLShaderProgram> compiled1(
                shaderProgramCompiler->compileFile(stdType, "file1"));
        CPPUNIT_ASSERT(!shaderProgramCompiler->log().isEmpty());
        CPPUNIT_ASSERT(!shaderProgramCompiler->success());

        QScopedPointer<AnnotatedGLShaderProgram> compiled2(
                shaderProgramCompiler->compileFile(stdType, "file2"));
        CPPUNIT_ASSERT(shaderProgramCompiler->log().isEmpty());
        CPPUNIT_ASSERT(shaderProgramCompiler->success());
    }

    void returnsExportedFunctions()
    {
        QStringList exportedFunctions;
        exportedFunctions.append("f1");
        exportedFunctions.append("f2");

        EXPECT_CALL(shaderCompilerMock, compileFile(_, QString("dep")))
            .WillRepeatedly(Return(createDummyShader("void main() { }",
                            QStringList(), exportedFunctions)));

        CPPUNIT_ASSERT_EQUAL(exportedFunctions,
            shaderProgramCompiler->getExportedFunctionsForDependency(
                QGLShader::Fragment, "dep", ""));
    }

    CPPUNIT_TEST_SUITE(ASLProgramCompilerTest);
    CPPUNIT_TEST(addsMainShaderToProgram);
    CPPUNIT_TEST(cachesCompiledShader);
    CPPUNIT_TEST(loadsDependencies);
    CPPUNIT_TEST(loadsRecursiveDependencies);
    CPPUNIT_TEST(addsDependencyParametersToProgram);
    CPPUNIT_TEST(logsCompiledDependencies);
    CPPUNIT_TEST(loadsDuplicatesOnlyOnce);
    CPPUNIT_TEST(shaderProgramInfoEqualsMainShaderInfoExceptParameters);
    CPPUNIT_TEST(mainShaderWarningsAppearInCompilerLog);
    CPPUNIT_TEST(dependencyWarningsAppearInCompilerLog);
    CPPUNIT_TEST(mainShaderErrorsAppearInCompilerLog);
    CPPUNIT_TEST(dependencyErrorsAppearInCompilerLog);
    CPPUNIT_TEST(linkerErrorsAppearInCompilerLog);
    CPPUNIT_TEST(resetsErrorStateAndLogBeforeCompilation);
    CPPUNIT_TEST(returnsExportedFunctions);
    CPPUNIT_TEST_SUITE_END();

private:
    void assertCleanCompilationAndLinkage(AnnotatedGLShaderProgram *program)
    {
        CPPUNIT_ASSERT(shaderProgramCompiler->success());
        assertLogContainsOnlyInfo();
        CPPUNIT_ASSERT(program->log().isEmpty());
        CPPUNIT_ASSERT(program->isLinked());
    }

    void assertLogContainsOnlyInfo() {
        const QStringList lines = shaderProgramCompiler->log()
            .split(QRegExp("[\r\n]"));
        foreach (QString line, lines) {
            CPPUNIT_ASSERT(line.startsWith("INFO:") || line.isEmpty());
        }
    }

    void assertLogged(const asl::LogEntry &entry)
    {
        asl::assertLogContains(shaderProgramCompiler->log(), entry);
    }

    AnnotatedGLShader * createDummyShader(
            const QString &source = QString("void main() { }"),
            const QStringList &dependencies = QStringList(),
            const QStringList &exportedFunctions = QStringList(),
            const QList<asl::ShaderParameterInfo> &parameters =
                QList<asl::ShaderParameterInfo>())
    {
        ShaderInfo info;
        info.dependencies = dependencies;
        info.exportedFunctions = exportedFunctions;
        info.parameters = parameters;
        AnnotatedGLShader *shader = new AnnotatedGLShader(stdType, info);
        shader->compileSourceCode(source);
        return shader;
    }

    static const QString LOG_ERROR;
    static const QString LOG_WARNING;
    static const QString LOG_INFO;

    QGLPixelBuffer pixelBufferForGLContext;
    const QGLShader::ShaderType stdType;
    NiceMock<AnnotatedGLShaderCompilerMock> shaderCompilerMock;
    NiceMock<DependencyLocatorMock> dependencyLocatorMock;
    ASLProgramCompiler *shaderProgramCompiler;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::ASLProgramCompilerTest);

using namespace asl;

const QString ASLProgramCompilerTest::LOG_ERROR("ERROR");
const QString ASLProgramCompilerTest::LOG_WARNING("WARNING");
const QString ASLProgramCompilerTest::LOG_INFO("INFO");

