#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QList>

class FileUtil {
public:
    static bool copyDir(const QString &source,
                        const QString &destination, bool override);

    static bool copyRecursively(const QString &srcFilePath,
                                const QString &tgtFilePath);

    static bool fileRecursive(const QString &filePath, QList<QString> &fileList);
};
#endif // FILEUTIL_H
