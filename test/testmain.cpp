
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    (void) app; /* Prevent unused variable warning. */

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry &registry =
            CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( registry.makeTest() );
    return !runner.run();
}
