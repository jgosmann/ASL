
#include "../testenv.h"

#include "../../src/asl/relativedependencylocator.h"

namespace asl
{
class RelativeDependencyLocatorTest : public TestFixture
{
public:
    void setUp()
    {
        locator = new RelativeDependencyLocator();
    }

    void tearDown()
    {
        delete locator;
    }

    void returnsNameIfNoIncludingFileSpecified()
    {
        QString name("dependencyName");
        CPPUNIT_ASSERT_EQUAL("./" + name, locator->locate(name));
    }

    void returnsPathIfIncludingFileInWorkingDirectory()
    {
        QString name("../foo/dependency.fs");
        QString includingFile("somefile.fs");
        CPPUNIT_ASSERT_EQUAL("./" + name, locator->locate(name, includingFile));
    }

    void returnsBasePathBeforeDependencyPath()
    {
        QString name("foo/dependency.fs");
        QString includingFile("./bar/somefile.fs");
        QString expected("./bar/foo/dependency.fs");
        CPPUNIT_ASSERT_EQUAL(expected, locator->locate(name, includingFile));
    }

    void returnsBasePathBeforeDependencyPathIncludingCdUps()
    {
        QString name("../../foo/dependency.fs");
        QString includingFile("./bar/somefile.fs");
        QString expected("./bar/../../foo/dependency.fs");
        CPPUNIT_ASSERT_EQUAL(expected, locator->locate(name, includingFile));
    }

    CPPUNIT_TEST_SUITE(RelativeDependencyLocatorTest);
    CPPUNIT_TEST(returnsNameIfNoIncludingFileSpecified);
    CPPUNIT_TEST(returnsPathIfIncludingFileInWorkingDirectory);
    CPPUNIT_TEST(returnsBasePathBeforeDependencyPath);
    CPPUNIT_TEST(returnsBasePathBeforeDependencyPathIncludingCdUps);
    CPPUNIT_TEST_SUITE_END();

private:
    RelativeDependencyLocator *locator;
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::RelativeDependencyLocatorTest);

