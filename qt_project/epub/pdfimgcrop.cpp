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

#include "config/appsettings.h"
#include "utils/msgboxutil.h"
#include "pdfimgcrop.h"
#include "ui_pdfimgcrop.h"

PdfImgCrop::PdfImgCrop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PdfImgCrop), mCropRect(NULL)
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

PdfImgCrop::~PdfImgCrop()
{
    delete ui;
}

void PdfImgCrop::initUI()
{
    mBtnOpen = ui->btnOpen;
    mBtnProcess = ui->btnProcess;

    //mFilePath = ui->lineEditFilePath;

    connect(mBtnOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(mBtnProcess, SIGNAL(clicked()), this, SLOT(onBtnProcessClicked()));
}

void PdfImgCrop::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mDirName = s.value(AppSettings::IMG_CROP_PATH).toString();
    if (!mDirName.isEmpty()) {
        ui->lineEditFilePath->setText(mDirName);
    } else {
        ui->lineEditFilePath->setText(tr("未指定目录"));
    }
}

bool PdfImgCrop::getCropParam()
{
    QString paramText = ui->lineEditParam->text();
    qDebug() << "paramText: " << paramText;
    QStringList strParams = paramText.split(",");
    if (strParams.size() != 4) {
        return false;
    }

    QString xStr = strParams.at(0);
    QString yStr = strParams.at(1);
    QString wStr = strParams.at(2);
    QString hStr = strParams.at(3);

    QString params("");
    params.append(xStr);
    params.append(",");
    params.append(yStr);
    params.append(",");
    params.append(wStr);
    params.append(",");
    params.append(hStr);
    qDebug() << "params: " << params;

    if (NULL == mCropRect) {
        mCropRect = new QRect();
    }

    mCropRect->setX(xStr.toInt());
    mCropRect->setY(yStr.toInt());
    mCropRect->setWidth(wStr.toInt());
    mCropRect->setHeight(hStr.toInt());

    return true;
}

void PdfImgCrop::onBtnOpenClicked()
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
        s.setValue(AppSettings::IMG_CROP_PATH, mDirName);
    }
}

void PdfImgCrop::onBtnProcessClicked()
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

    if (!getCropParam()) {
        MsgBoxUtil::warning(this, tr("未设定裁剪参数！"));
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

        QImage img(filePath);
        qDebug() << "img width: " << img.width();
        qDebug() << "img height: " << img.height();
        QImage newImg = img.copy(*mCropRect);
        QString newImgFilePath(dirNewStr);
        newImgFilePath.append("/");
        newImgFilePath.append(fileName);
        qDebug() << "newImgFilePath: " << newImgFilePath;
        newImg.save(newImgFilePath);

        if (ui->checkParamsAdjust->isChecked()) {
            break;
        }
    }

    QString result("处理结束！");
    MsgBoxUtil::warning(this, result);
}
