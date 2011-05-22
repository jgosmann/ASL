
#include "../src/common/nullable.h"

#include "../testenv.h"

namespace common
{
class NullableTest : public TestFixture
{
public:

    void throwsNoValueExceptionIfNotSet()
    {
        Nullable<int> t;
        try {
            t.value();
        } catch (const NoValueException &e) {
            return;
        }
        CPPUNIT_FAIL("No value exception expected.");
    }

    void returnsSetValue()
    {
        Nullable<int> t;
        t.set(2);
        CPPUNIT_ASSERT_EQUAL(2, t.value());
    }

    void hasValueReturnsFalseIfNotSet()
    {
        Nullable<int> t;
        CPPUNIT_ASSERT(!t.hasValue());
    }

    void hasValueReturnsTrueIfSet()
    {
        Nullable<int> t;
        t.set(1);
        CPPUNIT_ASSERT(t.hasValue());
    }

    void unsetResetsHasValue()
    {
        Nullable<int> t;
        t.set(23);
        t.unset();
        CPPUNIT_ASSERT(!t.hasValue());
    }

    void assignmentOperatorSetsValue()
    {
        Nullable<int> t;
        t = 2;
        CPPUNIT_ASSERT_EQUAL(2, t.value());
    }

    void castToTypeReturnsValue()
    {
        Nullable<int> t;
        t.set(1);
        CPPUNIT_ASSERT_EQUAL(1, static_cast<int>(t));
    }

    void castToTypeThrowsNoValueExceptionIfNotSet()
    {
        Nullable<int> t;
        try {
            static_cast<int>(t);
        } catch (const NoValueException &e) {
            return;
        }
        CPPUNIT_FAIL("Expected NoValueException.");
    }

    CPPUNIT_TEST_SUITE(NullableTest);
    CPPUNIT_TEST(throwsNoValueExceptionIfNotSet);
    CPPUNIT_TEST(returnsSetValue);
    CPPUNIT_TEST(hasValueReturnsFalseIfNotSet);
    CPPUNIT_TEST(hasValueReturnsTrueIfSet);
    CPPUNIT_TEST(unsetResetsHasValue);
    CPPUNIT_TEST(assignmentOperatorSetsValue);
    CPPUNIT_TEST(castToTypeReturnsValue);
    CPPUNIT_TEST(castToTypeThrowsNoValueExceptionIfNotSet);
    CPPUNIT_TEST_SUITE_END();

private:
};
} /* namespace asl */

CPPUNIT_TEST_SUITE_REGISTRATION(common::NullableTest);

