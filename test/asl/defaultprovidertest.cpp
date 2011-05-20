
#include "../src/asl/defaultprovider.h"

#include "common.h"

namespace asl
{
class DefaultProviderTest : public TestFixture
{
public:

    void returnsDefaultValueIfNotSet()
    {
        DefaultProvider<int> t(42);
        CPPUNIT_ASSERT_EQUAL(42, t.value());
    }

    void returnsSetValue()
    {
        DefaultProvider<int> t(1);
        t.set(2);
        CPPUNIT_ASSERT_EQUAL(2, t.value());
    }

    void isSetReturnsFalseIfNotSet()
    {
        DefaultProvider<int> t(1);
        CPPUNIT_ASSERT(!t.isSet());
    }

    void isSetReturnsTrueIfSet()
    {
        DefaultProvider<int> t(1);
        t.set(1);
        CPPUNIT_ASSERT(t.isSet());
    }

    void defaultValueReturnsDefaultValue()
    {
        DefaultProvider<int> t(42);
        t.set(23);
        CPPUNIT_ASSERT_EQUAL(42, t.defaultValue());
    }

    void resetRestoresDefaultValues()
    {
        DefaultProvider<int> t(42);
        t.set(23);
        t.reset();
        CPPUNIT_ASSERT_EQUAL(42, t.value());
        CPPUNIT_ASSERT(!t.isSet());
    }

    void assignmentOperatorSetsValue()
    {
        DefaultProvider<int> t(1);
        t = 2;
        CPPUNIT_ASSERT_EQUAL(2, t.value());
    }

    void castToTypeReturnsValue()
    {
        DefaultProvider<int> t(1);
        CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(t));
    }

    CPPUNIT_TEST_SUITE(DefaultProviderTest);
    CPPUNIT_TEST(returnsDefaultValueIfNotSet);
    CPPUNIT_TEST(returnsSetValue);
    CPPUNIT_TEST(isSetReturnsFalseIfNotSet);
    CPPUNIT_TEST(isSetReturnsTrueIfSet);
    CPPUNIT_TEST(defaultValueReturnsDefaultValue);
    CPPUNIT_TEST(resetRestoresDefaultValues);
    CPPUNIT_TEST(assignmentOperatorSetsValue);
    CPPUNIT_TEST(castToTypeReturnsValue);
    CPPUNIT_TEST_SUITE_END();

private:
};
} /* namespace asl */

CPPUNIT_TEST_SUITE_REGISTRATION(asl::DefaultProviderTest);

