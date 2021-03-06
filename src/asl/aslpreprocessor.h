#ifndef ASLPREPROCESSOR_H
#define ASLPREPROCESSOR_H

#include <qgl.h>
#include <QString>
#include <QTextStream>

namespace asl
{

/**
 * GLSL preprocessor implementation tailored to the needs auf the \ref
 * AnnotatedGLShaderCompiler. That means not all preprocessor directives will be
 * translated. Nevertheless, at least the conditionals (\c \#if, \c \#ifdef,
 * ...) and macros in these will work.
 *
 * \attention Be aware that you must not execute multiple calls to the methods
 * of this class at the same time (e.g. from different threads). Even with
 * different instances of this class you must not do this!
 */
class ASLPreprocessor {
public:
    ASLPreprocessor();

    const QString & log() const;
    bool success() const;

    void process(const QString &sourcecode, QTextStream *out,
            unsigned int sourceStringNo = 0);
    QString process(const QString &sourcecode, unsigned int sourceStringNo = 0);

    void resetAndClean();

    unsigned int glslVersion() const;
    void setGlslVersion(unsigned int version);

    static unsigned int systemGlslVersion();
    static unsigned int parseGlslVersionString(const GLubyte *versionStr);

private:
    unsigned int m_glslVersion;
};
}

#endif // ASLPREPROCESSOR_H
