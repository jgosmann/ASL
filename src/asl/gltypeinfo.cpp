
#include "gltypeinfo.h"

#include <QStringBuilder>

using namespace asl;
using namespace std;

const GLTypeInfo::KnownTypesTable GLTypeInfo::knownTypes;

const GLTypeInfo * GLTypeInfo::getFor(const QString &glslName)
        throw(invalid_argument)
{
    if (!knownTypes.isTypeKnown(glslName)) {
        throw invalid_argument(
                "Not a valid GLSL type supported by GLTypeInfo.");
    }
    return knownTypes.getType(glslName);
}

GLTypeInfo::GLTypeInfo(const QString &glslName, GLTypeInfo::Structure structure,
            GLTypeInfo::Type type, unsigned short int columnDimensionality,
            unsigned short int rowDimensionality)
        : m_glslName(glslName), m_structure(structure), m_type(type),
        m_columnDimensionality(columnDimensionality),
        m_rowDimensionality(rowDimensionality)
{
}

bool GLTypeInfo::operator==(const GLTypeInfo &rhs) const
{
    return m_structure == rhs.m_structure
        && m_type == rhs.m_type
        && m_columnDimensionality == rhs.m_columnDimensionality
        && m_rowDimensionality == rhs.m_rowDimensionality;
}

GLTypeInfo::KnownTypesTable::KnownTypesTable()
{
    /* Scalars, Open GL Shading Language, 3rd Edition, p. 68 */
    registerType(new GLTypeInfo(
        "float", GLTypeInfo::SCALAR, GLTypeInfo::FLOAT, 1, 1));
    registerType(new GLTypeInfo(
        "int", GLTypeInfo::SCALAR, GLTypeInfo::INT, 1, 1));
    registerType(new GLTypeInfo(
        "uint", GLTypeInfo::SCALAR, GLTypeInfo::UINT, 1, 1));
    registerType(new GLTypeInfo(
        "bool", GLTypeInfo::SCALAR, GLTypeInfo::BOOL, 1, 1));

    /* Vectors, Open GL Shading Language, 3rd Edition, p. 69 */
    for (unsigned short int i = 2; i <= 4; ++i) {
        QString basename = "vec" % QString::number(i);
        registerType(new GLTypeInfo(
            basename, GLTypeInfo::VECTOR, GLTypeInfo::FLOAT, 1, i));
        registerType(new GLTypeInfo(
            "i" % basename, GLTypeInfo::VECTOR, GLTypeInfo::INT, 1, i));
        registerType(new GLTypeInfo(
            "u" % basename, GLTypeInfo::VECTOR, GLTypeInfo::UINT, 1, i));
        registerType(new GLTypeInfo(
            "b" % basename, GLTypeInfo::VECTOR, GLTypeInfo::BOOL, 1, i));
    }

    /* Matrices, Open GL Shading Language, 3rd Edition, pp. 70-71 */
    for (unsigned short int i = 2; i <= 4; ++i) {
        for (unsigned short int j = 2; j <= 4; ++j) {
            registerType(new GLTypeInfo(
                "mat" % QString::number(i) % "x" % QString::number(j),
                GLTypeInfo::MATRIX, GLTypeInfo::FLOAT, i, j));
            if (i == j) {
                registerType(new GLTypeInfo(
                    "mat" % QString::number(i), GLTypeInfo::MATRIX,
                    GLTypeInfo::FLOAT, i, j));
            }
        }
    }
}

GLTypeInfo::KnownTypesTable::~KnownTypesTable()
{
    foreach (const GLTypeInfo *type, m_knownTypes) {
        delete type;
    }
}

void GLTypeInfo::KnownTypesTable::registerType(const GLTypeInfo *type)
{
    m_knownTypes.insert(type->glslName(), type);
}

