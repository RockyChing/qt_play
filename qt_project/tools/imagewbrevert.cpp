#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QFileInfo>
#include <QFileInfoList>
#include <QPalette>
#include <QRegExp>
#include <QSettings>
#include <QDebug>
#include <QIntValidator>
#include <QStringList>
#include <QMimeDatabase>
#include <QMimeType>

#include "config/appsettings.h"
#include "utils/msgboxutil.h"

#include "imagewbrevert.h"
#include "ui_imagewbrevert.h"

ImageWBRevert::ImageWBRevert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageWBRevert)
{
    ui->setupUi(this);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);

    initUI();
    initData();
}

ImageWBRevert::~ImageWBRevert()
{
    delete ui;
}

void ImageWBRevert::initUI()
{
    connect(ui->btnOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(ui->btnProcess, SIGNAL(clicked()), this, SLOT(onBtnProcessClicked()));
}

void ImageWBRevert::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mDirName = s.value(AppSettings::IMG_REVT_PATH).toString();
    if (!mDirName.isEmpty()) {
        ui->lineEditFilePath->setText(mDirName);
    } else {
        ui->lineEditFilePath->setText(tr("未指定目录"));
    }
}

void ImageWBRevert::onBtnOpenClicked()
{
    QString filePath = ui->lineEditFilePath->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mDirName = QFileDialog::getExistingDirectory(this, tr("Open file"), filePath);
    //mDirName = QDir::toNativeSeparators(mDirName);
    qDebug() << "mDirName: " << mDirName;
    if (mDirName.isEmpty()) {
        ui->lineEditFilePath->setText(tr("未指定目录"));
    } else {
        ui->lineEditFilePath->setText(mDirName);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::IMG_REVT_PATH, mDirName);
    }
}

void ImageWBRevert::onBtnProcessClicked()
{
    // 1.check parameter
    if (mDirName.isEmpty()) {
        MsgBoxUtil::warning(this, tr("未指定目录！"));
        return;
    }
    qDebug() << "mDirName: " << mDirName;

    QDir dir(mDirName);
    if (!dir.exists()) {
        MsgBoxUtil::warning(this, tr("指定目录不存在！"));
        return;
    }

    // 2.traverse files
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot
            | QDir::Files | QDir::Readable, QDir::Name);
    if (fileInfoList.size() <= 0) {
        MsgBoxUtil::warning(this, tr("目录下文件空！"));
        return;
    }

    // 3.create result dir
    QString dirNewStr(mDirName);
    dirNewStr.append("/result");
    qDebug() << "dirNewStr: " << dirNewStr;
    QDir dirNew;
    dirNew.mkdir(dirNewStr);
    if (!dirNew.exists()) {
        MsgBoxUtil::warning(this, tr("无法创建目录！"));
        return;
    }

    // 3.rename files
    int fileCount = fileInfoList.size();
    for (int i = 0; i < fileCount; i ++) {
        QFileInfo fileInfo = fileInfoList.at(i);
        QString filePath = fileInfo.absoluteFilePath();
        qDebug() << "filePath: " << filePath;
        QString fileName = fileInfo.fileName();
        qDebug() << "fileName: " << fileName;

        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(fileName);
        if (!mime.name().startsWith("image/")) {
            continue;
        }

        QImage img(filePath);
        int width = img.width();
        int height = img.height();
        QImage::Format fmt = img.format();
        qDebug() << "img width: " << width;
        qDebug() << "img height: " << height;
        QImage newImg(width, height, fmt);

        for (int w = 0; w < width; w ++) {
            for (int h = 0; h < height; h ++) {
                QColor rgb = img.pixel(w, h);
                rgb.setRgb(255-rgb.red(), 255-rgb.green(), 255-rgb.blue());
                newImg.setPixel(w, h, rgb.rgba());
            }
        }

        QString newImgFilePath(dirNewStr);
        newImgFilePath.append("/");
        newImgFilePath.append(fileName);
        qDebug() << "newImgFilePath: " << newImgFilePath;
        newImg.save(newImgFilePath);
    }

    QString result("处理结束！");
    MsgBoxUtil::warning(this, result);
}
