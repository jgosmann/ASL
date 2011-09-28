#ifndef GLVARIANT_H
#define GLVARIANT_H

#include <iostream>

#include <qgl.h>

#include "gltypeinfo.h"

namespace asl
{
/**
 * Class able to hold different types which can be passed to a shader program.
 */
class GLVariant
{
public:
    GLVariant();
    GLVariant(const GLVariant &other);
    GLVariant(const GLTypeInfo &type, const GLVariant &other);
    GLVariant(const QString &glslTypename, const GLVariant &other);

    static const GLVariant minOfType(const GLTypeInfo &type);
    static const GLVariant minOfType(const QString &glslTypename);
    static const GLVariant maxOfType(const GLTypeInfo &type);
    static const GLVariant maxOfType(const QString &glslTypename);

    /** \see #set */
    template<class T> GLVariant(const GLTypeInfo &type, GLsizei count,
            const T *value);

    /** \see #set */
    template<class T> GLVariant(const QString &glslTypename, GLsizei count,
            const T *value);

    ~GLVariant();

    /**
     * Sets the value of the GLVariant. This function will try to cast the
     * passed values to the type matching this GLVariant.
     *
     * You have to pass an array with either (rows * columns) of this
     * GLVariant's type or 1. In the former case a matrix will be stored (and
     * returned) in the way that you pass it, i.e. a column major matrix will be
     * stored and returned as column major matrix. In the later case the first
     * entry of the \a value array will be used for each vector component or
     * will initialize the diagonal of a column major matrix with 0 at all other
     * positions.
     *
     * \param count The length of the passed array to set the value.
     * \param value Array to set the value. Must have at least \a count values.
     */
    template<class T> void set(GLsizei count, const T *value);
    void set(const GLVariant &value);

    inline const GLfloat * asFloat() const { return m_data.asFloat; }
    inline const GLint * asInt() const { return m_data.asInt; }
    inline const GLuint * asUInt() const { return m_data.asUInt; }
    inline const GLint * asBool() const { return m_data.asInt; }
    template<class T> const T * as() const;

    template<class CastToT> CastToT ithValueCasted(GLsizei i) const;
    GLsizei count() const;
    inline const GLTypeInfo & type() const { return *m_type; }

    GLVariant & operator=(const GLVariant &rhs);
    bool operator==(const GLVariant &rhs) const;
    inline bool operator!=(const GLVariant &rhs) const
    {
        return !(*this == rhs);
    }

private:
    void allocateMemory();
    void freeMemory();

    template<class StoreT, class InitT> void set(StoreT *storage,
            GLsizei count, const InitT *value);
    template<class StoreT, class InitT> void setVecFromSingleValue(
            StoreT *storage, InitT value);
    template<class StoreT, class InitT> void setDiagonalMat(StoreT *storage,
            InitT value);
    template<class StoreT, class InitT> void setFromArray(StoreT *storage,
            GLsizei count, const InitT *value);
    template<class StoreT, class InitT> StoreT castValue(InitT value) const;

    template<class StoreT> static bool compareData(GLsizei count,
            const StoreT *lhs, const StoreT *rhs);

    const GLTypeInfo *m_type;
    union {
        GLfloat *asFloat;
        GLint *asInt;
        GLuint *asUInt;
    } m_data;
};

std::ostream & operator<<(std::ostream &output, const GLVariant &v);

} /* namespace asl */

#endif /* GLVARIANT_H */

