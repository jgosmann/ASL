#ifndef ANNOTATEDGLSHADERPROGRAMBUILDER_H
#define ANNOTATEDGLSHADERPROGRAMBUILDER_H

#include "annotatedglshaderprogram.h"

namespace asl
{
/**
 * This class allows to easily create an \ref AnnotatedGLShaderProgram and
 * setting the values of the annotations. However, you still have to add and
 * compile the source code of the real shader manually.
 */
class AnnotatedGLShaderProgramBuilder
{
public:
    inline void setName(const QString &name)
    {
        m_name = name;
    }

    inline void setDescription(const QString &description)
    {
        m_description = description;
    }

    inline AnnotatedGLShaderProgram * build()
    {
        return new AnnotatedGLShaderProgram(m_name, m_description);
    }

private:
    QString m_name;
    QString m_description;
};
}

#endif /* ANNOTATEDGLSHADERPROGRAMBUILDER_H */

