#include "aslpreprocessor.h"

#include "aslpreprocessor_parserinternal.h"

#include <exception>
#include <GL/glu.h>
#include <QByteArray>
#include <QGLPixelBuffer>
#include <QRegExp>

using namespace asl;

ASLPreprocessor::ASLPreprocessor()
{
    setGlslVersion(systemGlslVersion());
}

const QString & ASLPreprocessor::log()
{
    return ppinternal::log;
}

void ASLPreprocessor::process(const QString &sourcecode, QTextStream *out,
        unsigned int sourceStringNo)
{
    ppinternal::clearLog();
    ppinternal::sourceStringNo = sourceStringNo;
    ppinternal::parse(sourcecode, out);
}

QString ASLPreprocessor::process(const QString &sourcecode,
        unsigned int sourceStringNo)
{
    QString out;
    QTextStream outStream(&out, QIODevice::WriteOnly);
    ppinternal::glslVersion = m_glslVersion;
    process(sourcecode, &outStream, sourceStringNo);
    return out;
}

void ASLPreprocessor::resetAndClean()
{
    ppinternal::resetAndClean();
}

unsigned int ASLPreprocessor::glslVersion() const
{
    return m_glslVersion;
}

void ASLPreprocessor::setGlslVersion(unsigned int version)
{
    m_glslVersion = version;
}

unsigned int ASLPreprocessor::systemGlslVersion()
{
    QGLPixelBuffer pixelBufToProvideGLContext(1, 1);
    if (!QGLContext::currentContext()) {
        pixelBufToProvideGLContext.makeCurrent();
    }

    const GLubyte *versionStr = glGetString(GL_SHADING_LANGUAGE_VERSION);
    if (!versionStr) {
        return 0;
    }

    return parseGlslVersionString(versionStr);
}

unsigned int ASLPreprocessor::parseGlslVersionString(const GLubyte *versionStr)
{
    unsigned int version = 0;

    const QRegExp versionMatcher("(\\d+.\\d+(.\\d+)?)(\\D.*)?");
    const QString matchable(reinterpret_cast<const char *>(versionStr));
    if (versionMatcher.exactMatch(matchable)) {
        version = versionMatcher.cap(1).remove('.').toUInt();
        if (versionMatcher.cap(2).isEmpty()) {
            version *= 10;
        }
    }

    return version;
}

