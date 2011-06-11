
#include "../src/asl/annotatedglshaderprogramcompiler.h"
#include "../src/asl/gltypeinfo.h"
#include "../src/asl/shaderparameterinfobuilder.h"

#include "../testenv.h"

#include "annotatedglshadercompilermock.h"
#include "dependencylocatormock.h"
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
class AnnotatedGLShaderCompilerTest : public TestFixture
{
public:
    AnnotatedGLShaderCompilerTest()
        : pixelBufferForGLContext(1, 1), stdType(QGLShader::Fragment) { }

    void setUp()
    {
        pixelBufferForGLContext.makeCurrent();
        shaderProgramCompiler = new AnnotatedGLShaderProgramCompiler(
                shaderCompilerMock, dependencyLocatorMock);
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
        EXPECT_CALL(shaderCompilerMock, compileFile(stdType, filename))
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
        EXPECT_CALL(shaderCompilerMock, compileFile(stdType, filename))
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
        EXPECT_CALL(shaderCompilerMock, compileFile(stdType, filename))
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

    void shaderProgramInfoEqualsMainShaderInfo()
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
        EXPECT_CALL(shaderCompilerMock, compileFile(stdType, filename))
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
        CPPUNIT_ASSERT_EQUAL(info.parameters, compiled->parameters());
    }

    CPPUNIT_TEST_SUITE(AnnotatedGLShaderCompilerTest);
    CPPUNIT_TEST(addsMainShaderToProgram);
    CPPUNIT_TEST(cachesCompiledShader);
    CPPUNIT_TEST(loadsDependencies);
    CPPUNIT_TEST(loadsRecursiveDependencies);
    CPPUNIT_TEST(loadsDuplicatesOnlyOnce);
    CPPUNIT_TEST(shaderProgramInfoEqualsMainShaderInfo);
    CPPUNIT_TEST_SUITE_END();
    
    // TODO passes errors/warnings on
    // TODO asl main

private:
    void assertCleanCompilationAndLinkage(AnnotatedGLShaderProgram *program)
    {
        CPPUNIT_ASSERT(shaderProgramCompiler->success());
        CPPUNIT_ASSERT(shaderProgramCompiler->log().isEmpty());
        CPPUNIT_ASSERT(program->log().isEmpty());
        CPPUNIT_ASSERT(program->isLinked());
    }

    AnnotatedGLShader * createDummyShader(
            const QString &source = QString("void main() { }"),
            const QStringList &dependencies = QStringList())
    {
        ShaderInfo info;
        info.dependencies = dependencies;
        AnnotatedGLShader *shader = new AnnotatedGLShader(stdType, info);
        shader->compileSourceCode(source);
        return shader;
    }

    QGLPixelBuffer pixelBufferForGLContext;
    const QGLShader::ShaderType stdType;
    AnnotatedGLShaderCompilerMock shaderCompilerMock;
    DependencyLocatorMock dependencyLocatorMock;
    AnnotatedGLShaderProgramCompiler *shaderProgramCompiler;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::AnnotatedGLShaderCompilerTest);

using namespace asl;

