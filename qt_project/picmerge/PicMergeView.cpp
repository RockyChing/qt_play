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
#include <QSize>
#include <QScrollBar>
#include <QPixmap>
#include <QIcon>
#include <QListWidgetItem>

#define LOG_TAG "PicMerge"
#include "../log/log.h"
#include "PicMergeView.h"
#include "ui_picmergeview.h"
#include "PicMerge.h"

#define PIC_ITEM_SIZE (240)


PicMergeView::PicMergeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicMergeView), mPicListWidget(NULL)
{
    ui->setupUi(this);
    init();
    initUI();
    initSlot();
}

PicMergeView::~PicMergeView()
{
    clearAllPicWidgetItem();
    delete ui;
}

void PicMergeView::init()
{
    mPicListWidget = ui->picListWidget;

    QSize iconSize = QSize(PIC_ITEM_SIZE, PIC_ITEM_SIZE);
    mPicListWidget->setIconSize(iconSize);
    mPicListWidget->setResizeMode(QListView::Adjust);
    mPicListWidget->setViewMode(QListView::IconMode);
    mPicListWidget->setMovement(QListView::Static);
    mPicListWidget->setSpacing(5);
    QScrollBar *bar = mPicListWidget->horizontalScrollBar();
    bar->setDisabled(true);
}

void PicMergeView::initUI()
{
    ui->infoText->hide();
    ui->guideText->setText(tr("安全考虑，把多张图片合并成一个文件"));
}

void PicMergeView::initSlot()
{
    connect(ui->btnPicMerge, SIGNAL(clicked()), this, SLOT(onBtnMergeClicked()));
    connect(ui->btnPicSplit, SIGNAL(clicked()), this, SLOT(onBtnSplitClicked()));
    connect(ui->btnInfoClear, SIGNAL(clicked()), this, SLOT(onBtnClearClicked()));
}

void PicMergeView::clearAllPicWidgetItem()
{
    do {
        QListWidgetItem *item = mPicListWidget->takeItem(0);
        if (NULL == item)
            break;
        log_debug("remove item: %p\r\n", item);
        delete item;
        item = NULL;
    } while (1);
}

void PicMergeView::addPicWidgetItem(QString path)
{
    QListWidgetItem *picItem = new QListWidgetItem();
    QPixmap pic(path);
    if (pic.width() > PIC_ITEM_SIZE || pic.height() > PIC_ITEM_SIZE) {
        pic = pic.scaled(PIC_ITEM_SIZE, PIC_ITEM_SIZE, Qt::KeepAspectRatio);
    }

    picItem->setIcon(QIcon(pic));
    mPicListWidget->addItem(picItem);
}

void PicMergeView::onBtnMergeClicked()
{
    ui->mergeProgress->reset();
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Open directory"), "./",
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dirName.isEmpty()) {
        QMessageBox::warning(this, tr("图片合并"), tr("请选择目录!"));
        return;
    }

    qDebug() << "dirName: " << dirName;

    /* 遍历目录下所有文件 */
    QDir dir(dirName);
    QFileInfoList filesInfo = dir.entryInfoList(QDir::Files | QDir::Readable, QDir::Name);
    if (filesInfo.size() <= 0)
        return;

    PicMerge picMerge;
    picMerge.setMergedFileName(dirName + "/pic_merge.bin");

    ui->mergeProgress->setValue(10);
    ui->infoText->appendPlainText("合并的文件：");
    clearAllPicWidgetItem();

    int nPic = 0;
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
        } else if (file.suffix().contains("webp")) {
            item.format = PIC_FMT_WEBP;
        } else {
            continue;
        }

        ui->infoText->appendPlainText(file.fileName());
        picMerge.append(&item, file.absoluteFilePath());
        nPic += 1;

        addPicWidgetItem(file.absoluteFilePath());
    }

    if (0 == nPic) {
        QMessageBox::warning(this, tr("图片合并"), tr("没有图片!"));
        return;
    }

    picMerge.fileHeaderFlush();
    ui->mergeProgress->setValue(50);
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
    ui->mergeProgress->setValue(100);
    QMessageBox::information(this, tr("图片合并"), tr("成功!"));
}

void PicMergeView::onBtnSplitClicked()
{
    ui->mergeProgress->reset();

    /* load file from currentPath */
    QString curPath = QDir::currentPath();
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), curPath, "(*.bin)");
    filePath = QDir::toNativeSeparators(filePath);
    ui->mergeProgress->setValue(10);
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
            ui->mergeProgress->setValue(100);

            clearAllPicWidgetItem();
            QStringList& files = picMerge.getNeedSplitPicFiles();
            for (int i = 0; i < files.size(); i ++) {
                QString file = files.at(i);
                addPicWidgetItem(file);
                log_debug("split %s\r\n", file.toUtf8().data());
            }

            QMessageBox::information(this, tr("图片拆分"), tr("成功!"));
            ui->infoText->appendPlainText(tr("成功"));
        }
    }
}

void PicMergeView::onBtnClearClicked()
{
    ui->infoText->clear();
}
