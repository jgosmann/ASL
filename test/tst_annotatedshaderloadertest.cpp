#include <QtCore/QString>
#include <QtTest/QtTest>

class AnnotatedShaderLoaderTest : public QObject
{
    Q_OBJECT

public:
    AnnotatedShaderLoaderTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

AnnotatedShaderLoaderTest::AnnotatedShaderLoaderTest()
{
}

void AnnotatedShaderLoaderTest::initTestCase()
{
}

void AnnotatedShaderLoaderTest::cleanupTestCase()
{
}

void AnnotatedShaderLoaderTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(AnnotatedShaderLoaderTest);

#include "tst_annotatedshaderloadertest.moc"
