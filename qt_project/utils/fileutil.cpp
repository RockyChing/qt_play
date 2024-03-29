#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>
#include "fileutil.h"


bool FileUtil::copyDir(const QString &source,
                        const QString &dst, bool override) {
    QDir srcDir(source);
    if (!srcDir.exists()) {
        return false;
    }

    bool res;
    QDir dstDir(dst);
    if (!dstDir.exists()) {
        res = dstDir.mkpath(dst);
        if (!res) {
            return false;
        }
    }

    QString srcPath = QDir::toNativeSeparators(source);
    if (!srcPath.endsWith(QDir::separator()))
        srcPath += QDir::separator();
    QString dstPath = QDir::toNativeSeparators(dst);
    if (!dstPath.endsWith(QDir::separator()))
        dstPath += QDir::separator();

    bool error = false;
    QStringList fileNames = srcDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    for (QStringList::size_type i = 0; i < fileNames.size(); i++) {
        QString fileName = fileNames.at(i);
        QString srcFilePath = srcPath + fileName;
        QString dstFilePath = dstPath + fileName;
        //qDebug() << "srcFilePath:" << srcFilePath;
        //qDebug() << "dstFilePath:" << dstFilePath;

        QFileInfo fileInfo(srcFilePath);
        if (fileInfo.isFile() || fileInfo.isSymLink()) {
            if (override) {
                QFile::setPermissions(dstFilePath, QFile::WriteOwner);
            }

            QFile::copy(srcFilePath, dstFilePath);
        } else if (fileInfo.isDir()) {
            if (!copyDir(srcFilePath, dstFilePath, override)) {
                error = true;
            }
        }
    }
    return !error;
}

bool FileUtil::copyRecursively(const QString &srcFilePath,
                                const QString &tgtFilePath) {
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files
                | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath  = srcFilePath + QDir::separator() + fileName;
            const QString newTgtFilePath  = tgtFilePath + QDir::separator() + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath)) {
                return false;
            }
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath)) {
            return false;
        }
    }
    return true;
}

bool FileUtil::fileRecursive(const QString &filePath, QList<QString> &fileList)
{
    QDir dir(filePath);
    if (!dir.exists())
        return false;

    /* find all fiels dirs, except . and .. */
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    if (list.size() < 1)
        return false;

    for (int i = 0; i < list.size(); i ++) {
        QFileInfo info = list.at(i);
        bool isDir = info.isDir();
        if (isDir) {
            fileRecursive(info.absoluteFilePath(), fileList);
        } else {
            //qDebug() << info.absoluteFilePath();
            fileList.append(info.absoluteFilePath());
        }
    }

    return true;
}
