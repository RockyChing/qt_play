/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/9/28
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QFileDialog>
#include <QFileInfoList>
#include <QMessageBox>
#include <QDir>
#include <QDebug>

#include "PicMergeView.h"
#include "ui_picmergeview.h"
#include "PicMerge.h"


PicMergeView::PicMergeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicMergeView)
{
    ui->setupUi(this);
    initUI();
    initSlot();
}

PicMergeView::~PicMergeView()
{
    delete ui;
}

void PicMergeView::initUI()
{
    ui->guideText->setText(tr("安全考虑，把多张图片合并成一个文件"));
}

void PicMergeView::initSlot()
{
    connect(ui->btnPicMerge, SIGNAL(clicked()), this, SLOT(onBtnMergeClicked()));
    connect(ui->btnPicSplit, SIGNAL(clicked()), this, SLOT(onBtnSplitClicked()));
    connect(ui->btnInfoClear, SIGNAL(clicked()), this, SLOT(onBtnClearClicked()));
}

void PicMergeView::onBtnMergeClicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open directory"), "./",
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qDebug() << "dirName: " << dirName;

    /* 遍历目录下所有文件 */
    QDir dir(dirName);
    QFileInfoList filesInfo = dir.entryInfoList(QDir::Files | QDir::Readable, QDir::Name);
    if (filesInfo.size() <= 0)
        return;

    PicMerge picMerge;
    picMerge.setMergedFileName(dirName + "/pic_merge.bin");

    ui->infoText->appendPlainText("合并的文件：");
    for (int i = 0; i < filesInfo.size(); i ++) {
        /* 获取文件属性 */
        QFileInfo file = filesInfo.at(i);
#if 0
        qDebug() << "filePath: " << file.filePath();                   //F:/IoT/qt_play/data/pog.txt
        qDebug() << "absoluteFilePath: " << file.absoluteFilePath();   //F:/IoT/qt_play/data/pog.txt
        qDebug() << "canonicalFilePath: " << file.canonicalFilePath(); //F:/IoT/qt_play/data/pog.txt
        qDebug() << "fileName: " << file.fileName();                   //pog.txt
        qDebug() << "baseName: " << file.baseName();                   //pog
        qDebug() << "completeBaseName: " << file.completeBaseName();   //pog
        qDebug() << "suffix: " << file.suffix();                       //txt
        qDebug() << "bundleName: " << file.bundleName();               //""
        qDebug() << "completeSuffix: " << file.completeSuffix();       //txt
        qDebug() << "path: " << file.path();                           //F:/IoT/qt_play/data
        qDebug() << "absolutePath: " << file.absolutePath();           //F:/IoT/qt_play/data
        qDebug() << "canonicalPath: " << file.canonicalPath();         //F:/IoT/qt_play/data
        qDebug() << "size: " << file.size();
#endif
        PicInfo item;
        item.size = file.size();
        if (file.suffix().contains("jpg")) {
            item.format = PIC_FMT_JPG;
        } else if (file.suffix().contains("png")) {
            item.format = PIC_FMT_PNG;
        } else if (file.suffix().contains("gif")) {
            item.format = PIC_FMT_GIF;
        } else {
            continue;
        }

        ui->infoText->appendPlainText(file.fileName());
        picMerge.append(&item, file.absoluteFilePath());
    }

    picMerge.fileHeaderFlush();
    ui->infoText->appendPlainText("合并为：");
    ui->infoText->appendPlainText(picMerge.getMergedFileName());

    int result = picMerge.fileMerge();
    if (result) {
        QString errMsg = QString("合并错误！\r\n错误码：%1").arg(result);
        QMessageBox::critical(NULL, QObject::tr("错误"), errMsg);
    }

    result = picMerge.fileMergeCheck();
    QString chkResult("校验...");
    if (result) {
        QString errMsg = QString("校验错误！\r\n错误码：%1").arg(result);
        QMessageBox::critical(NULL, QObject::tr("错误"), errMsg);
    }

    chkResult.append("成功");
    ui->infoText->appendPlainText(chkResult);
    QMessageBox::information(this, tr("图片合并"), tr("成功!"));
}

void PicMergeView::onBtnSplitClicked()
{
    /* load file from currentPath */
    QString curPath = QDir::currentPath();
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), curPath, "(*.bin)");
    filePath = QDir::toNativeSeparators(filePath);
    if (filePath.isEmpty()) {
        ui->infoText->appendPlainText(tr("文件不存在！"));
    } else {
        ui->infoText->appendPlainText(tr("待拆分的文件："));
        ui->infoText->appendPlainText(filePath);
        PicMerge picMerge;
        int result = picMerge.fileSplit(filePath);
        if (result) {
            QString errMsg = QString("图片拆分错误！\r\n错误码：%1").arg(result);
            QMessageBox::critical(NULL, QObject::tr("错误"), errMsg);
        } else {
            QMessageBox::information(this, tr("图片拆分"), tr("成功!"));
            ui->infoText->appendPlainText(tr("成功"));
        }
    }
}

void PicMergeView::onBtnClearClicked()
{
    ui->infoText->clear();
}
