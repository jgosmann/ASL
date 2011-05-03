#ifndef ASLPREPROCESSOR_H
#define ASLPREPROCESSOR_H

#include <QString>
#include <QTextStream>

namespace asl
{

/** GLSL preprocessor implementation tailored to the needs auf the \ref
 * AnnotatedShaderCompiler. That means not all preprocessor directives will be
 * translated. Nevertheless, at least the conditionals (\c #if, \c #ifdef, ...)
 * and macros in these will work.
 *
 * \attention Be aware that you must not execute multiple calls to the methods
 * of this class at the same time (e.g. from different threads). Even with
 * different instances of this class you must not do this!
 */
class ASLPreprocessor { public: ASLPreprocessor();

    void process(const QString &sourcecode, QTextStream *out);
    QString process(const QString &sourcecode);

    void resetAndClean();
};
}

#endif // ASLPREPROCESSOR_H
