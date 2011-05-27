
#include "glvariant.h"

using namespace asl;

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
            delete[] m_values.asFloat;
            break;
        case GLTypeInfo::BOOL: /* fall through */
        case GLTypeInfo::INT:
            delete[] m_values.asInt;
            break;
        case GLTypeInfo::UINT:
            delete[] m_values.asUInt;
            break;
        default:
            // FIXME: handle forgotten destructor
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
            set(&m_values.asFloat, count, value);
            break;
        case GLTypeInfo::BOOL: /* fall through */
        case GLTypeInfo::INT:
            set(&m_values.asInt, count, value);
            break;
        case GLTypeInfo::UINT:
            set(&m_values.asUInt, count, value);
            break;
        default:
            // FIXME throw exception;
            break;
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
            m_values.asFloat = new GLfloat[count()];
            break;
        case GLTypeInfo::BOOL: /* fall through */
        case GLTypeInfo::INT:
            m_values.asInt = new GLint[count()];
            break;
        case GLTypeInfo::UINT:
            m_values.asUInt = new GLuint[count()];
            break;
        default:
            // FIXME: throw exception;
            break;
    }
}

template<class StoreT, class InitT> void GLVariant::set(StoreT **storage,
        GLsizei count, const InitT *value)
{
    if (m_type.type() == GLTypeInfo::BOOL) {
        (*storage)[0] = (0 != value[0]);
    } else {
        (*storage)[0] = static_cast<StoreT>(value[0]);
    }
}

// TODO: delete or reuse this code
//bool GLVariant::operator==(const GLValue &compareTo) const
//{
    //const unsigned short int &rows = m_type->rowDimensionality();
    //const unsigned short int &cols = m_type->columnDimensionality();
    //for (unsigned int i = 0; i < cols; ++i) {
        //for (unsigned int j = 0; j < rows; ++j) {
            //const unsigned short int idx = i * cols + rows;
            //if (m_type->isFloat()) {
                //if (m_value.asFloats[idx] != compareTo.asFloats[idx]) {
                    //return false;
                //}
            //} else {
                //if (m_value.asInts[idx] != compareTo.asInts[idx]) {
                    //return false;
                //}
            //}
        //}
    //}
    //return true;
//}

//bool operator==(const GLValue &lhs, const GLVariant &rhs)
//{
    //return rhs == lhs;
//}

