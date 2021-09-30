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

private:
    int fileSplitOne(QFile& srcFile, QString& picPath, quint32 picIndex,PicInfo *picInfo);

private:
    QVector<PicInfo> mPicInfo;
    QStringList mPicFiles;
    QString mMergedFileName;
};

#endif // PICMERGEFILEHEADER_H
