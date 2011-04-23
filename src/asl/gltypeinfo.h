#ifndef GLTYPEINFO_H
#define GLTYPEINFO_H

namespace asl
{
    class GLTypeInfo
    {
    public:
        enum Structure
        {
            SCALAR,
            VECTOR,
            MATRIX,
            SAMPLER
        };

        enum Type
        {
            FLOAT,
            INT,
            UINT,
            BOOL
        };

        inline virtual ~GLTypeInfo() { }

        virtual Structure structure() const = 0;
        virtual Type type() const = 0;
        virtual unsigned short int rowDimensionality() const = 0;
        virtual unsigned short int columnDimensionality() const = 0;

        inline bool isScalar() const {
            return structure() == SCALAR;
        }

        inline bool isVector() const {
            return structure() == VECTOR;
        }

        inline bool isMatrix() const {
            return structure() == MATRIX;
        }

        inline bool isSampler() const {
            return structure() == SAMPLER;
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
    };
}

#endif // GLTYPEINFO_H
