
#include <GL/gl.h>

#include "../src/asl/gltypenames.h"
#include "../src/asl/glvariant.h"

#include "../testenv.h"

#define IN ,
#define OUT ,

namespace asl
{
class GLVariantTest : public TestFixture
{
public:
    template<const char *glslName, class InitType> void testScalar()
    {
        const InitType *values = getScalarValue<InitType>();
        GLVariant variant(glslName, 1, values);
        CPPUNIT_ASSERT_EQUAL(values[0], getResults<InitType>(variant)[0]);
    }

    template<const char *glslName, class InType, class OutType>
    void testCast()
    {
        const InType *values = getScalarValue<InType>();
        GLVariant variant(glslName, 1, values);
        CPPUNIT_ASSERT_EQUAL(static_cast<OutType>(values[0]),
                getResults<OutType>(variant)[0]);
    }

    template<class InType, GLuint inValue, bool outValue>
    void testBooleanScalar()
    {
        const InType values[] = { inValue };
        GLVariant variant(gltypenames::BOOL, 1, values);
        CPPUNIT_ASSERT_EQUAL(static_cast<GLint>(outValue), variant.asBool()[0]);
    }

    CPPUNIT_TEST_SUITE(GLVariantTest);
    CPPUNIT_TEST(testScalar<gltypenames::FLOAT IN GLfloat>);
    CPPUNIT_TEST(testScalar<gltypenames::INT IN GLint>);
    CPPUNIT_TEST(testScalar<gltypenames::UINT IN GLuint>);
    CPPUNIT_TEST(testCast<gltypenames::FLOAT IN GLint OUT GLfloat>);
    CPPUNIT_TEST(testCast<gltypenames::FLOAT IN GLuint OUT GLfloat>);
    CPPUNIT_TEST(testCast<gltypenames::INT IN GLuint OUT GLint>);
    CPPUNIT_TEST(testBooleanScalar<GLint IN 1 OUT true>);
    CPPUNIT_TEST(testBooleanScalar<GLint IN 246 OUT true>);
    CPPUNIT_TEST(testBooleanScalar<GLuint IN 2 OUT true>);
    CPPUNIT_TEST(testBooleanScalar<GLfloat IN 24 OUT true>);
    CPPUNIT_TEST(testBooleanScalar<GLint IN 0 OUT false>);
    CPPUNIT_TEST(testBooleanScalar<GLuint IN 0 OUT false>);
    CPPUNIT_TEST(testBooleanScalar<GLfloat IN 0 OUT false>);
    CPPUNIT_TEST_SUITE_END();

private:
    template<class T> static const T * getScalarValue();
    template<class T> static const T * getResults(const GLVariant &variant);

    template<class T> void assertEqualArrayOfLengthN(unsigned int n,
            const T *expected, const T *actual)
    {
        for (unsigned int i = 0; i < n; ++i) {
            CPPUNIT_ASSERT_EQUAL(expected[i], actual[i]);
        }
    }

    static const GLfloat floatScalarValue[];
    static const GLint intScalarValue[];
    static const GLuint uintScalarValue[];
};

const GLfloat GLVariantTest::floatScalarValue[] = { -2.3 };
const GLint GLVariantTest::intScalarValue[] = { -3 };
const GLuint GLVariantTest::uintScalarValue[] = { 4 };

template<> const GLfloat * GLVariantTest::getScalarValue() {
    return floatScalarValue;
}

template<> const GLuint * GLVariantTest::getScalarValue() {
    return uintScalarValue;
}

template<> const GLint * GLVariantTest::getScalarValue() {
    return intScalarValue;
}

template<> const GLfloat * GLVariantTest::getResults(const GLVariant &variant)
{
    return variant.asFloat();
}

template<> const GLint * GLVariantTest::getResults(const GLVariant &variant)
{
    return variant.asInt();
}

template<> const GLuint * GLVariantTest::getResults(const GLVariant &variant)
{
    return variant.asUInt();
}

}

CPPUNIT_TEST_SUITE_REGISTRATION(asl::GLVariantTest);

