#ifndef ANNOTATEDSHADERLOADER_H
#define ANNOTATEDSHADERLOADER_H

#include <QGLShader>
#include <QGLShaderProgram>
#include <QObject>
#include <QSharedPointer>

#include "annotatedglshaderprogram.h"
#include "dependencyreader.h"

namespace asl
{
class ASLCompiler : public QObject
{
    Q_OBJECT
public:
    explicit ASLCompiler(QSharedPointer<DependencyReader> dependencyReader,
            QObject *parent = 0);

    /**
     * Compiles and links an annotated shader program and returns a pointer to
     * it.
     *
     * \note Use ASLCompiler#success() to test whether the last compilation was
     * successful. Please note that ASLCompiler#log() may return a string
     * containing warnings (but not errors) even if the compilation was
     * successful.
     *
     * \note Use always `/' as an universal directory separator for the
     * \a pathOfSource argument.
     *
     * \attention You have to free the memory allocated for the shader when you
     * do not need it anymore. Use \c delete for that.
     */
    asl::AnnotatedGLShaderProgram * compile(QGLShader::ShaderType type, const
            QString &source, const QString &pathOfSource = "");

    QString log() const { return m_log; }

    /**
     * \note Even if \c true is returned ASLCompiler#log() may return a string
     * containing warnings (but not errors).
     */
    const bool success() const { return m_success; }

signals:

public slots:

private:
    QString m_log;
    bool m_success;

    QSharedPointer<DependencyReader> m_dependencyReader;

};
}

#endif // ANNOTATEDSHADERLOADER_H
