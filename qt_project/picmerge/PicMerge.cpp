/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/9/28
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include "PicMerge.h"


PicMerge::PicMerge()
{

}

void PicMerge::append(PicInfo *picInfo, QString picFilePath)
{
    PicInfo item;
    item.format = picInfo->format;
    item.size = picInfo->size;
    mPicInfo.append(item);
    mPicFiles.append(picFilePath);
}

void PicMerge::setMergedFileName(QString fileName)
{
    mMergedFileName.clear();
    mMergedFileName.append(fileName);
}

QString& PicMerge::getMergedFileName()
{
    return mMergedFileName;
}

void PicMerge::fileHeaderFlush()
{
    QFile file(mMergedFileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    quint32 nCount  = mPicInfo.size();
    file.write((const char *) &nCount, sizeof(quint32));

    for (quint32 i = 0; i < nCount; i ++) {
        PicInfo info = mPicInfo.at(i);
        file.write((const char *) &info, sizeof(info));
    }

    file.close();
}

int PicMerge::fileMerge()
{
    QFile fileMerge(mMergedFileName);

    if (!fileMerge.open(QIODevice::WriteOnly | QIODevice::Append))
        return -1;

    for (int i = 0; i < mPicFiles.size(); i ++) {
        QString fileName = mPicFiles.at(i);
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
            return -2;

        qint64 fileSize = file.size();
        quint8 *data = new quint8[fileSize];
        if (NULL == data)
            return -3;

        if (fileSize != file.read((char *)data, fileSize)) {
            delete [] data;
            return -4;
        }
        file.close();

        if (fileSize != fileMerge.write((char *)data, fileSize)) {
            delete [] data;
            return -5;
        }
    }

    fileMerge.close();
    return 0;
}

int PicMerge::fileMergeCheck()
{
    QFile fileMerge(mMergedFileName);

    if (!fileMerge.open(QIODevice::ReadOnly))
        return -1;

    quint32 nCount;
    if (sizeof(nCount) != fileMerge.read((char *) &nCount, sizeof(nCount))) {
        fileMerge.close();
        return -2;
    }

    PicInfo info;
    quint32 picSize = 0;
    for (quint32 i = 0; i < nCount; i ++) {
        if (sizeof(info) != fileMerge.read((char *) &info, sizeof(info))) {
            fileMerge.close();
            return -3;
        }

        if (!(info.format >= PIC_FMT_JPG && info.format <= PIC_FMT_GIF)) {
            fileMerge.close();
            return -4;
        }
        picSize += info.size;
    }

    quint32 hdrSize = sizeof(nCount) + nCount * sizeof(PicInfo);
    if ((hdrSize + picSize) != fileMerge.size()) {
        fileMerge.close();
        return -5;
    }

    fileMerge.close();
    return 0;
}

int PicMerge::fileSplitOne(QFile& srcFile, QString& picPath, quint32 picIndex,PicInfo *picInfo)
{
    QString suffix("");
    if (PIC_FMT_JPG == picInfo->format)
        suffix.append(".jpg");
    else if (PIC_FMT_PNG == picInfo->format)
        suffix.append(".png");
    else if (PIC_FMT_BMP == picInfo->format)
        suffix.append(".bmp");
    else if (PIC_FMT_GIF == picInfo->format)
        suffix.append(".gif");
    else
        return -11;

    QString name = QString("pic%1").arg(picIndex);
    QString fileName(picPath + "/" + name + suffix);
    qDebug() << "Split fileName" << fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return -12;

    quint8 *data = new quint8[picInfo->size];
    if (NULL == data) {
        file.close();
        return -13;
    }

    if (picInfo->size != srcFile.read((char *) data, picInfo->size)) {
        file.close();
        delete [] data;
        return -14;
    }

    if (picInfo->size != file.write((char *) data, picInfo->size)) {
        file.close();
        delete [] data;
        return -15;
    }

    file.close();
    delete [] data;
    return 0;
}

int PicMerge::fileSplit(QString& fileName)
{
    QFile fileMerge(fileName);
    if (!fileMerge.open(QIODevice::ReadOnly))
        return -1;

    quint32 nCount;
    if (sizeof(nCount) != fileMerge.read((char *) &nCount, sizeof(nCount))) {
        fileMerge.close();
        return -2;
    }

    QVector<PicInfo> vecPicInfo;
    for (quint32 i = 0; i < nCount; i ++) {
        PicInfo info;
        if (sizeof(PicInfo) != fileMerge.read((char *) &info, sizeof(PicInfo))) {
            fileMerge.close();
            return -3;
        }
        vecPicInfo.append(info);
    }

    if (nCount != vecPicInfo.size()) {
        fileMerge.close();
        return -4;
    }

    QFileInfo fileInfo(fileName);
    QString filePath = fileInfo.absolutePath();

    for (quint32 i = 0; i < nCount; i ++) {
        PicInfo info = vecPicInfo.at(i);
        int result = fileSplitOne(fileMerge, filePath, i, &info);
        if (result) {
            fileMerge.close();
            return result;
        }
    }

    fileMerge.close();
    return 0;
}
