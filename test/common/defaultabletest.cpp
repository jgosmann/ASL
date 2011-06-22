
#include "../src/common/defaultable.h"

#include "../testenv.h"

namespace common
{
class DefaultableTest : public TestFixture
{
public:

    void returnsDefaultValueIfNotSet()
    {
        Defaultable<int> t(42);
        CPPUNIT_ASSERT_EQUAL(42, t.value());
    }

    void returnsSetValue()
    {
        Defaultable<int> t(1);
        t.set(2);
        CPPUNIT_ASSERT_EQUAL(2, t.value());
    }

    void isSetReturnsFalseIfNotSet()
    {
        Defaultable<int> t(1);
        CPPUNIT_ASSERT(!t.isSet());
    }

    void isSetReturnsTrueIfSet()
    {
        Defaultable<int> t(1);
        t.set(1);
        CPPUNIT_ASSERT(t.isSet());
    }

    void defaultValueReturnsDefaultValue()
    {
        Defaultable<int> t(42);
        t.set(23);
        CPPUNIT_ASSERT_EQUAL(42, t.defaultValue());
    }

    void resetRestoresDefaultValues()
    {
        Defaultable<int> t(42);
        t.set(23);
        t.reset();
        CPPUNIT_ASSERT_EQUAL(42, t.value());
        CPPUNIT_ASSERT(!t.isSet());
    }

    void assignmentOperatorSetsValue()
    {
        Defaultable<int> t(1);
        t = 2;
        CPPUNIT_ASSERT_EQUAL(2, t.value());
    }

    void castToTypeReturnsValue()
    {
        Defaultable<int> t(1);
        CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(t));
    }

    CPPUNIT_TEST_SUITE(DefaultableTest);
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

CPPUNIT_TEST_SUITE_REGISTRATION(common::DefaultableTest);

