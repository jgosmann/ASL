
#include "relativedependencylocator.h"

#include <QDir>
#include <QFileInfo>

using namespace asl;

QString RelativeDependencyLocator::locate(const QString &name,
        const QString &includingFile)
{
    const QDir basepath(QFileInfo(includingFile).dir());
    const QString dependencyPath(basepath.filePath(name));
    return dependencyPath;
}

