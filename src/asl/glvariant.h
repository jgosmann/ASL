#ifndef GLVARIANT_H
#define GLVARIANT_H

#include <GL/gl.h>

#include "gltypeinfo.h"
#include "glvalue.h"

namespace asl
{
/**
 * Class able to hold different types which can passed to a shader program.
 *
 * \note Matrices will be stored and returned in the way you pass them. This
 * means when passing a column major matrix it will be stored and returned
 * as column major.
 */
class GLVariant
{
public:
    GLVariant(const GLVariant &other);

    template<class T> GLVariant(const GLTypeInfo &type, GLsizei count,
            const T *value);

    template<class T> GLVariant(const QString &glslTypename, GLsizei count,
            const T *value);

    ~GLVariant();

    /**
     * Sets the value of the GLVariant. This function will try to cast the
     * passed values to the type matching this GLVariant.
     */
    template<class T> void set(GLsizei count, const T *value);

    inline const GLfloat * asFloat() const { return m_values.asFloat; }
    inline const GLint * asInt() const { return m_values.asInt; }
    inline const GLuint * asUInt() const { return m_values.asUInt; }
    inline const GLint * asBool() const { return m_values.asInt; }
    const GLsizei count() const;

    GLVariant & operator=(const GLVariant &rhs);

private:
    void allocateMemory();

    template<class StoreT, class InitT> void set(StoreT **storage,
            GLsizei count, const InitT *value);

    const GLTypeInfo &m_type;
    union {
        GLfloat *asFloat;
        GLint *asInt;
        GLuint *asUInt;
    } m_values;
};
} /* namespace asl */

#endif /* GLVARIANT_H */

