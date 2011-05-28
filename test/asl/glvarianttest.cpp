
#include <stdexcept>

#include <GL/gl.h>

#include "../src/asl/gltypenames.h"
#include "../src/asl/glvariant.h"

#include "../testenv.h"

#define IN ,
#define OUT ,
#define DIM ,

using namespace std;

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

    template<const char *glslName> void testNonScalar()
    {
        const GLTypeInfo &type = GLTypeInfo::getFor(glslName);
        const GLsizei count = type.rowDimensionality()
            * type.columnDimensionality();
        GLVariant variant(type, count, testValues);
        assertEqualArrayOfLengthN(count, testValues, variant.asFloat());
    }

    void testSingleValueIsReplicatedToFillVector()
    {
        GLVariant variant("vec3", 1, floatScalarValue);
        for (unsigned short int i = 0; i < 3; ++i) {
            CPPUNIT_ASSERT_EQUAL(*floatScalarValue, variant.asFloat()[i]);
        }
    }

    void testSingleValueInitsDiagnolOfColumnMajorMatrix()
    {
        /* Non quadratic matrix to verfiy column major ordering. */
        GLVariant variant("mat3x4", 1, floatScalarValue);
        for (unsigned short int i = 0; i < 3; ++i) {
            for (unsigned short int j = 0; j < 4; ++j) {
                const GLfloat actual = variant.asFloat()[4 * i + j];
                if (i == j) {
                    CPPUNIT_ASSERT_EQUAL(*floatScalarValue, actual);
                } else {
                    CPPUNIT_ASSERT_EQUAL(static_cast<GLfloat>(0.0), actual);
                }
            }
        }
    }

    void testThrowsInvalidArgumentExceptionIfNotEnoughValuesProvided() {
        try {
            GLVariant variant("vec4", 2, testValues);
        } catch (const invalid_argument &e) {
            return;
        }
        CPPUNIT_FAIL("Expected invalid_argument exception.");
    }

    void testSilentlyIgnoresExtraComponents() {
        GLVariant variant("vec2", 4, testValues);
        assertEqualArrayOfLengthN(2, testValues, variant.asFloat());
    }

    void testCopyConstructor()
    {
        // TODO
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
    CPPUNIT_TEST(testNonScalar<gltypenames::VEC2>);
    CPPUNIT_TEST(testNonScalar<gltypenames::VEC3>);
    CPPUNIT_TEST(testNonScalar<gltypenames::VEC4>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT2>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT3>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT4>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT2x2>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT2x3>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT2x4>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT3x2>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT3x3>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT3x4>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT4x2>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT4x3>);
    CPPUNIT_TEST(testNonScalar<gltypenames::MAT4x4>);
    CPPUNIT_TEST(testSingleValueIsReplicatedToFillVector);
    CPPUNIT_TEST(testSingleValueInitsDiagnolOfColumnMajorMatrix);
    CPPUNIT_TEST(testThrowsInvalidArgumentExceptionIfNotEnoughValuesProvided);
    CPPUNIT_TEST(testSilentlyIgnoresExtraComponents);
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

    static const GLfloat testValues[];
};

const GLfloat GLVariantTest::floatScalarValue[] = { -2.3 };
const GLint GLVariantTest::intScalarValue[] = { -3 };
const GLuint GLVariantTest::uintScalarValue[] = { 4 };

const GLfloat GLVariantTest::testValues[] = {
    /* more or less random values */
    -2.3,  4.2,  5.6, -1.2,
     4.5,  0.0, -0.0, -1.0,
     1.0, -34.,  56.,  0.1,
     3.0,  1.2, -5.5, -0.2 };

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

