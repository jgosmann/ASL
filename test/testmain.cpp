
#include <gmock/gmock.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Q_UNUSED(app);

    ::testing::GTEST_FLAG(throw_on_failure) = true;
    ::testing::InitGoogleMock(&argc, argv);

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry =
            CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    return !runner.run();
}
