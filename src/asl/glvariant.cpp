
#include "glvariant.h"

#include <stdexcept>

using namespace asl;
using namespace std;

GLVariant::GLVariant(const GLVariant &other) : m_type(other.m_type)
{
    allocateMemory();
    set(other);
}

GLVariant::GLVariant(const GLTypeInfo &type, const GLVariant &other)
        : m_type(type)
{
    allocateMemory();
    set(other);
}

GLVariant::GLVariant(const QString &glslTypename, const GLVariant &other)
        : m_type(GLTypeInfo::getFor(glslTypename))
{
    allocateMemory();
    set(other);
}

template GLVariant::GLVariant(const GLTypeInfo &type, GLsizei count,
        const GLfloat *value);
template GLVariant::GLVariant(const GLTypeInfo &type, GLsizei count,
        const GLint *value);
template GLVariant::GLVariant(const GLTypeInfo &type, GLsizei count,
        const GLuint *value);
template<class T> GLVariant::GLVariant(const GLTypeInfo &type, GLsizei count,
        const T *value) : m_type(type)
{
    allocateMemory();
    set(count, value);
}

template GLVariant::GLVariant(const QString &glslTypename, GLsizei count,
        const GLfloat *value);
template GLVariant::GLVariant(const QString &glslTypename, GLsizei count,
        const GLint *value);
template GLVariant::GLVariant(const QString &glslTypename, GLsizei count,
        const GLuint *value);
template<class T> GLVariant::GLVariant(const QString &glslTypename,
        GLsizei count, const T *value)
    : m_type(GLTypeInfo::getFor(glslTypename))
{
    allocateMemory();
    set(count, value);
}

GLVariant::~GLVariant() {
    switch (m_type.type()) {
        case GLTypeInfo::FLOAT:
            delete[] m_data.asFloat;
            break;
        case GLTypeInfo::BOOL: /* fall through */
        case GLTypeInfo::INT:
            delete[] m_data.asInt;
            break;
        case GLTypeInfo::UINT:
            delete[] m_data.asUInt;
            break;
        default:
            throw logic_error("Missing memory cleanup.");
            break;
    }
}

template void GLVariant::set(GLsizei count, const GLfloat *value);
template void GLVariant::set(GLsizei count, const GLint *value);
template void GLVariant::set(GLsizei count, const GLuint *value);
template<class T> void GLVariant::set(GLsizei count, const T *value)
{
    switch (m_type.type()) {
        case GLTypeInfo::FLOAT:
            set(m_data.asFloat, count, value);
            break;
        case GLTypeInfo::BOOL: /* fall through */
        case GLTypeInfo::INT:
            set(m_data.asInt, count, value);
            break;
        case GLTypeInfo::UINT:
            set(m_data.asUInt, count, value);
            break;
        default:
            throw invalid_argument("Type not supported.");
            break;
    }
}

void GLVariant::set(const GLVariant &value)
{
    switch (value.type().type()) {
        case GLTypeInfo::FLOAT:
            set(value.count(), value.asFloat());
            break;
        case GLTypeInfo::BOOL: /* fall through */
        case GLTypeInfo::INT:
            set(value.count(), value.asInt());
            break;
        case GLTypeInfo::UINT:
            set(value.count(), value.asUInt());
            break;
        default:
            throw invalid_argument("Copy from GLVariant's type not supported.");
    }
}

const GLsizei GLVariant::count() const
{
    return m_type.rowDimensionality() * m_type.columnDimensionality();
}

void GLVariant::allocateMemory()
{
    switch (m_type.type()) {
        case GLTypeInfo::FLOAT:
            m_data.asFloat = new GLfloat[count()];
            break;
        case GLTypeInfo::BOOL: /* fall through */
        case GLTypeInfo::INT:
            m_data.asInt = new GLint[count()];
            break;
        case GLTypeInfo::UINT:
            m_data.asUInt = new GLuint[count()];
            break;
        default:
            throw invalid_argument("Type not supported.");
            break;
    }
}

template<class StoreT, class InitT> void GLVariant::set(StoreT *storage,
        GLsizei count, const InitT *value)
{
    if (m_type.structure() == GLTypeInfo::VECTOR && count == 1) {
        setVecFromSingleValue(storage, value[0]);
    } else if (m_type.structure() == GLTypeInfo::MATRIX && count == 1) {
        setDiagonalMat(storage, value[0]);
    } else {
        setFromArray(storage, count, value);
    }
}

template<class StoreT, class InitT> void GLVariant::setVecFromSingleValue(
        StoreT *storage, InitT value)
{
    const unsigned short int count = m_type.rowDimensionality()
            * m_type.columnDimensionality();
    StoreT castedValue = castValue<StoreT, InitT>(value);
    for (unsigned short int i = 0; i < count; ++i) {
        storage[i] = castedValue;
    }
}

template<class StoreT, class InitT> void GLVariant::setDiagonalMat(
        StoreT *storage, InitT value)
{
    StoreT castedValue = castValue<StoreT, InitT>(value);
    for (unsigned short int i = 0; i < m_type.columnDimensionality(); ++i) {
        for (unsigned short int j = 0; j < m_type.rowDimensionality(); ++j) {
            const unsigned short int idx = i * m_type.rowDimensionality() + j;
            if (i == j) {
                storage[idx] = castedValue;
            } else {
                storage[idx] = 0;
            }
        }
    }
}

template<class StoreT, class InitT> void GLVariant::setFromArray(
        StoreT *storage, GLsizei count, const InitT *value)
{
    const GLsizei neededCount = m_type.rowDimensionality()
            * m_type.columnDimensionality();
    if (count < neededCount) {
        throw invalid_argument("Count of array elements does not match the "
                "GLVariant's dimensionality.");
    }

    for (GLsizei i = 0; i < neededCount; ++i) {
        storage[i] = castValue<StoreT, InitT>(value[i]);
    }
}

template<class StoreT, class InitT> StoreT GLVariant::castValue(InitT value)
{
    if (m_type.type() == GLTypeInfo::BOOL) {
        return (0 != value);
    } else {
        return static_cast<StoreT>(value);
    }
}

bool GLVariant::operator==(const GLVariant &compareTo) const
{
    if (m_type != compareTo.type()) {
        return false;
    }

    const GLsizei count = m_type.rowDimensionality()
            * m_type.columnDimensionality();
    switch (m_type.type())
    {
        case GLTypeInfo::FLOAT:
            return compareData(count, asFloat(), compareTo.asFloat());
            break;
        case GLTypeInfo::BOOL: /* fall through */
        case GLTypeInfo::INT:
            return compareData(count, asInt(), compareTo.asInt());
            break;
        case GLTypeInfo::UINT:
            return compareData(count, asUInt(), compareTo.asUInt());
            break;
        default:
            throw logic_error("Missing comparision for type.");
            break;
    }
}

template<class StoreT> bool GLVariant::compareData(GLsizei count,
        const StoreT *lhs, const StoreT *rhs)
{
    for (GLsizei i = 0; i < count; ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }
    return true;
}

ostream & asl::operator<<(ostream &output, const GLVariant &v)
{
    output << qPrintable(v.type().glslName()) << "(";
    for (GLsizei i = 0; i < v.count(); ++i) {
        if (i > 0) {
            output << ", ";
        }

        switch (v.type().type()) {
            case GLTypeInfo::FLOAT:
                output << v.asFloat()[i];
                break;
            case GLTypeInfo::BOOL: /* fall through */
            case GLTypeInfo::INT:
                output << v.asInt()[i];
                break;
            case GLTypeInfo::UINT:
                output << v.asUInt()[i];
                break;
            default:
                output << "<no output function defined>";
                break;
        }
    }
    output << ")";

    return output;
}

