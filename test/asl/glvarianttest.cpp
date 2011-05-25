
#include <GL/gl.h>

#include "../src/asl/glvariant.h"

#include "../testenv.h"

namespace asl
{
class GLVariantTest : public TestFixture
{
public:
    void testScalarFloat()
    {
        GLfloat values[] = { 2.3 };
        GLVariant variant("float", 1, values);
        assertEqualArrayOfLengthN(1, values, variant.asFloat());
    }

    void testScalarInt()
    {
        GLint values[] = { 42 };
        GLVariant variant("int", 1, values);
        assertEqualArrayOfLengthN(1, values, variant.asInt());
    }

    void testScalarUInt()
    {
        GLuint values[] = { 42 };
        GLVariant variant("uint", 1, values);
        assertEqualArrayOfLengthN(1, values, variant.asUInt());
    }

    CPPUNIT_TEST_SUITE(GLVariantTest);
    CPPUNIT_TEST(testScalarFloat);
    CPPUNIT_TEST(testScalarInt);
    CPPUNIT_TEST(testScalarUInt);
    CPPUNIT_TEST_SUITE_END();

private:
    template<class T> void assertEqualArrayOfLengthN(unsigned int n,
            const T *expected, const T *actual)
    {
        for (unsigned int i = 0; i < n; ++i) {
            CPPUNIT_ASSERT_EQUAL(expected[i], actual[i]);
        }
    }
};
}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::GLVariantTest);

