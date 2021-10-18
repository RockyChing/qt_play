#ifndef PICMERGEFILEHEADER_H
#define PICMERGEFILEHEADER_H
#include <QString>
#include <QStringList>
#include <QVector>
#include "PicInfo.h"

class PicMerge
{
public:
    PicMerge();

public:
    void append(PicInfo *picInfo, QString picFilePath);

    void setMergedFileName(QString fileName);
    QString& getMergedFileName();

    void fileHeaderFlush();
    int fileMerge();
    int fileMergeCheck();

    int fileSplit(QString& fileName);

    QStringList& getNeedSplitPicFiles();

private:
    int fileSplitOne(QFile& srcFile, QString& picPath, quint32 picIndex,PicInfo *picInfo);

private:
    QVector<PicInfo> mPicInfo;
    QStringList mNeedMergePicFiles;
    QStringList mNeedSplitPicFiles;
    QString mMergedFileName;
};

#endif // PICMERGEFILEHEADER_H
