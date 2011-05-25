
#include "glvariant.h"

using namespace asl;

template GLVariant::GLVariant(const GLTypeInfo &type, GLsizei count,
        GLfloat *value);
template GLVariant::GLVariant(const GLTypeInfo &type, GLsizei count,
        GLint *value);
template GLVariant::GLVariant(const GLTypeInfo &type, GLsizei count,
        GLuint *value);
template<class T> GLVariant::GLVariant(const GLTypeInfo &type, GLsizei count,
        T *value) : m_type(type)
{
    set(count, value);
}

template GLVariant::GLVariant(const QString &glslTypename, GLsizei count,
        GLfloat *value);
template GLVariant::GLVariant(const QString &glslTypename, GLsizei count,
        GLint *value);
template GLVariant::GLVariant(const QString &glslTypename, GLsizei count,
        GLuint *value);
template<class T> GLVariant::GLVariant(const QString &glslTypename,
        GLsizei count, T *value) : m_type(GLTypeInfo::getFor(glslTypename))
{
    set(count, value);
}

template<class T> void GLVariant::set(T **storage, GLsizei count,
        const T *value)
{
    // FIXME: free mem
    *storage = new T[count];
    (*storage)[0] = value[0];
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

