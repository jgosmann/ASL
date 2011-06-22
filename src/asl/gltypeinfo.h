#ifndef GLTYPEINFO_H
#define GLTYPEINFO_H

#include <stdexcept>
#include <utility>

#include <QHash>
#include <QString>

namespace asl
{
/**
 * Describes GLSL types.
 *
 * \attention Samplers are not supported, yet.
 */
class GLTypeInfo
{
public:
    static const GLTypeInfo & getFor(const QString &glslName)
            throw(std::invalid_argument);

    enum Structure
    {
        SCALAR,
        VECTOR,
        MATRIX
    };

    enum Type
    {
        FLOAT,
        INT,
        UINT,
        BOOL
    };

    const QString & glslName() const { return m_glslName; }
    Structure structure() const { return m_structure; }
    Type type() const { return m_type; }
    unsigned short int rowDimensionality() const { return m_rowDimensionality; }
    unsigned short int columnDimensionality() const
    {
        return m_columnDimensionality;
    }

    inline bool isScalar() const {
        return structure() == SCALAR;
    }

    inline bool isVector() const {
        return structure() == VECTOR;
    }

    inline bool isMatrix() const {
        return structure() == MATRIX;
    }

    inline bool isFloat() const {
        return type() == FLOAT;
    }

    inline bool isInt() const {
        return type() == INT;
    }

    inline bool isUInt() const {
        return type() == UINT;
    }

    inline bool isBool() const {
        return type() == BOOL;
    }

    bool operator==(const GLTypeInfo &rhs) const;
    inline bool operator!=(const GLTypeInfo &rhs) const
    {
        return !(*this == rhs);
    }

private:
    GLTypeInfo(const QString &glslName, Structure structure, Type type,
            unsigned short int columnDimensionality,
            unsigned short int rowDimensionality);

    const QString m_glslName;
    const Structure m_structure;
    const Type m_type;
    const unsigned short int m_columnDimensionality;
    const unsigned short int m_rowDimensionality;

    class KnownTypesTable
    {
        public:
            KnownTypesTable();
            ~KnownTypesTable();

            inline bool isTypeKnown(const QString &glslName) const
            {
                return m_knownTypes.contains(glslName);
            }

            inline const GLTypeInfo *getType(const QString &glslName) const
            {
                return m_knownTypes.value(glslName);
            }

        private:
            void registerType(const GLTypeInfo *type);

            QHash<QString, const GLTypeInfo *> m_knownTypes;
    };

    static const KnownTypesTable & knownTypes();
};
}

#endif // GLTYPEINFO_H
