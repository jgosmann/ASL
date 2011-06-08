#ifndef ANNOTATEDGLSHADERPROGRAMBUILDER_H
#define ANNOTATEDGLSHADERPROGRAMBUILDER_H

#include "annotatedglshaderprogram.h"

#include <QList>
#include <QStringList>

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

    inline void setDependencies(const QStringList &dependencies)
    {
        m_dependencies = dependencies;
    }

    inline void addParameter(const ShaderParameterInfo &parameter)
    {
        m_parameters.append(parameter);
    }

    //inline AnnotatedGLShaderProgram * build()
    //{
        //return new AnnotatedGLShaderProgram(m_name, m_description,
                //m_dependencies, m_parameters);
    //}

private:
    QString m_name;
    QString m_description;
    QList<asl::ShaderParameterInfo> m_parameters;
    QStringList m_dependencies;
};
}

#endif /* ANNOTATEDGLSHADERPROGRAMBUILDER_H */

