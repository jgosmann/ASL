#ifndef GLVARIANT_H
#define GLVARIANT_H

#include <GL/gl.h>

#include "gltypeinfo.h"
#include "glvalue.h"

namespace asl
{
/**
 * \note Matrices will be stored as column major.
 */
class GLVariant
{
public:
    GLVariant(const GLVariant &other);

    template<class T> GLVariant(const GLTypeInfo &type, GLsizei count,
            T *value);

    template<class T> GLVariant(const QString &glslTypename, GLsizei count,
            T *value);

    inline void set(GLsizei count, const GLfloat *value)
    {
        set(&m_values.asFloat, count, value);
    }

    inline void set(GLsizei count, const GLint *value)
    {
        set(&m_values.asInt, count, value);
    }

    inline void set(GLsizei count, const GLuint *value)
    {
        set(&m_values.asUInt, count, value);
    }

    inline const GLfloat * asFloat() const { return m_values.asFloat; }
    inline const GLint * asInt() const { return m_values.asInt; }
    inline const GLuint * asUInt() const { return m_values.asUInt; }
    const GLsizei count() const;

    GLVariant & operator=(const GLVariant &rhs);

private:
    template<class T> void set(T **storage, GLsizei count, const T *value);

    const GLTypeInfo &m_type;
    union {
        GLfloat *asFloat;
        GLint *asInt;
        GLuint *asUInt;
    } m_values;
};
} /* namespace asl */

#endif /* GLVARIANT_H */

