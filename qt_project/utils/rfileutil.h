#ifndef RFILEUTIL_H
#define RFILEUTIL_H
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>

class RFileUtil {
public:
    static bool copyDir(const QString &source,
                        const QString &destination, bool override);

    static bool copyRecursively(const QString &srcFilePath,
                                const QString &tgtFilePath);
};
#endif // RFILEUTIL_H
