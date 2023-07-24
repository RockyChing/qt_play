#include <QString>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#include "imgrename.h"


ImgRename::ImgRename()
{

}

void ImgRename::run()
{
    qDebug() << "start...";
    QString base = "c:/Users/rocky/Pictures/fjbz/sunzi";
    QString fdir = base + "/" + "index.html";

    fdir = QDir::toNativeSeparators(fdir);
    QFile ffile(fdir);
    if (!ffile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "err:" << fdir;
        return;
    }

    QTextStream in(&ffile);
    int lineCnt = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        //qDebug() << line;

        int n = line.indexOf("images");
        QString tmp = line.mid(n,17);
        //qDebug() << tmp;
        QString sdir = base + "/" + tmp;
        sdir = QDir::toNativeSeparators(sdir);
        QFile sfile(sdir);
        if (!sfile.exists()) {
            qDebug() << "err:" << sdir;
            break;
        }

        lineCnt += 1;
        QString newImgName("");
        newImgName = QString::asprintf("%03d", lineCnt);
        //qDebug() << newImgName;
        QString ddir = base + "/images/" + newImgName + ".jpg";
        ddir = QDir::toNativeSeparators(ddir);
        //qDebug() << ddir;

        bool res = QFile::rename(sdir, ddir);
        if (!res) {
            qDebug() << "err:" << res;
            break;
        }
    }

    qDebug() << "done!!!!!";
    ffile.close();
}
