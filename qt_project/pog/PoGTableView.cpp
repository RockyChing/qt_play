/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/9/26
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileDialog>
#include <QTemporaryFile>
#include <QTextStream>
#include <QIODevice>
#include <QSettings>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QList>

#define LOG_TAG "PoGTableView"
#include "../log/log.h"
#include "config/appsettings.h"
#include "PoGTableView.h"
#include "ui_pogtableview.h"
#include "PoGItem.h"


const int PoGTableView::PoG_YEAR_START = 2020;
const int PoGTableView::PoG_MON_START  = 9;
const int PoGTableView::PoG_DAY_START  = 20;

PoGTableView::PoGTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PoGTableView)
{
    ui->setupUi(this);

    init();
    initUI();
    initSlot();
}

PoGTableView::~PoGTableView()
{
    delete ui;
}

void PoGTableView::init()
{
    mTableColumnCount = 0;

    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mFileAbsPath = s.value(AppSettings::POG_PATH).toString();
}

void PoGTableView::initUI()
{
    // table header
#if 0
    QStringList hdrList;
    hdrList.append(tr("日期"));
    hdrList.append(tr("苹果"));
    mItemModel->setHorizontalHeaderLabels(hdrList);
#endif
}

void PoGTableView::initSlot()
{
    connect(ui->btnPoGOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(ui->btnPoGFilter, SIGNAL(clicked()), this, SLOT(onBtnFilterClicked()));
}

void PoGTableView::initTableModel()
{
    if (mTableColumnCount) {
        mItemModel = new QStandardItemModel(0, mTableColumnCount, this);
        mItemSelect = new QItemSelectionModel(mItemModel);
        ui->tableView->setModel(mItemModel);
        ui->tableView->setSelectionModel(mItemSelect);
        ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    } else {
        QMessageBox::warning(NULL, QObject::tr("警告"), tr("mTableColumnCount=0"));
    }
}

void PoGTableView::initTableHeader(QStringList& hdrList)
{
    mItemModel->setHorizontalHeaderLabels(hdrList);
}

void PoGTableView::initTableDate()
{
    QDate date = QDate::currentDate();
    int nRow = 0;

    QStandardItem *item;
    while (date.year() >= PoG_YEAR_START) {
        if (date.month() == PoG_MON_START && date.day() < PoG_DAY_START)
            break;

        item = new QStandardItem(date.toString("yyyy-MM-dd"));
        mItemModel->setItem(nRow, 0, item);

        nRow += 1;
        date = date.addDays(-1);
    }
}

int PoGTableView::fileParserOne(QString filePath, QList<PoGItem>& items)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;

    if (file.size() == 0) {
        file.close();
        return 0;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.startsWith("#"))
            continue;
        QStringList tmpList = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        PoGItem item;
        item.date = QDate::fromString(tmpList.at(0), "yyyy-MM-dd");
        item.price = tmpList.at(1);
        items.append(item);
        //qDebug() << item.date.toString();
        //qDebug() << item.price;
    }

    file.close();
    return 0;
}

int PoGTableView::fileFillTable(int column, QList<PoGItem> &items)
{
    qDebug("items %d", items.size());
    for (int i = 0; i < items.size(); i ++) {
        PoGItem pogItem = items.at(i);

        QStandardItem *tabItem;
        int nRow = mItemModel->rowCount();
        for (int j = i; j < nRow; j ++) {
            tabItem = mItemModel->item(j, 0);
            QDate date = QDate::fromString(tabItem->text(), "yyyy-MM-dd");
            if (0 == date.daysTo(pogItem.date)) {
                QStandardItem *tmpItem = new QStandardItem(pogItem.price);
                mItemModel->setItem(j, column, tmpItem);
            }
        }
    }
    return 0;
}

void PoGTableView::clearTableItemAll()
{
    if (mItemModel != NULL) {
        mItemModel->removeRows(0, mItemModel->rowCount());
    }
}

void PoGTableView::onBtnOpenClicked()
{
    if (mFileAbsPath.isEmpty()) {
        /* load file from currentPath */
        mFileAbsPath = QDir::currentPath();
    }

    QString dirName = QFileDialog::getExistingDirectory(this, tr("选择目录"), mFileAbsPath,
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qDebug() << "dirName: " << dirName;

    dirName = QDir::toNativeSeparators(dirName);
    if (dirName.isEmpty()) {
        QMessageBox::critical(NULL, QObject::tr("错误"), tr("选择目录"));
        return;
    } else {
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::POG_PATH, dirName);
    }

    /* 遍历目录下所有文件 */
    QDir dir(dirName);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Readable, QDir::Name);
    if (fileInfoList.size() <= 0) {
        QMessageBox::warning(NULL, QObject::tr("警告"), tr("文件不存在"));
        return;
    }

    QVector<int> emptyFileIndex;
    emptyFileIndex.clear();
    for (int i = 0; i < fileInfoList.size(); i ++) {
        if (fileInfoList.at(i).size() == 0) {
            emptyFileIndex.append(i);
        }
    }

    for (int i = 0; i < emptyFileIndex.size(); i ++)
        fileInfoList.removeAt(i);
    qDebug() << "fileInfoList size: " << fileInfoList.size();

    QStringList hdrList;
    hdrList.append("日期");
    for (int i = 0; i < fileInfoList.size(); i ++) {
        /* 获取文件属性 */
        QFileInfo file = fileInfoList.at(i);
        hdrList.append(file.baseName().mid(2));
    }

    mTableColumnCount = hdrList.size();
    qDebug() << "hdrList size: " << mTableColumnCount;
    initTableModel();
    initTableHeader(hdrList);
    initTableDate();

    for (int i = 0; i < fileInfoList.size(); i ++) {
        QFileInfo file = fileInfoList.at(i);

        QList<PoGItem> items;
        items.clear();
        int res = fileParserOne(file.absoluteFilePath(), items);
        if (res) {
            QString errMsg = QString("解析错误码：%1，文件：%2").arg(file.fileName()).arg(res);
            QMessageBox::critical(NULL, QObject::tr("错误"), errMsg);
            return;
        }

        res = fileFillTable(i + 1, items);
        if (res) {
            QString errMsg = QString("填表错误码：%1，文件：%2").arg(file.fileName()).arg(res);
            QMessageBox::critical(NULL, QObject::tr("错误"), errMsg);
            return;
        }
    }
}

void PoGTableView::onBtnFilterClicked()
{
    if (NULL == mItemModel)
        return;

    // 1. save table data to a temp file & filter null rows
    QTemporaryFile tmpFile;
    if (!tmpFile.open()) {
        log_error("create tmpFile failed!");
        return;
    }

    tmpFile.setAutoRemove(true);
    log_info("tmp file: %s", tmpFile.fileName().toUtf8().data());
    int rowCount = mItemModel->rowCount();
    int columnCount = mItemModel->columnCount();
    if (0 == rowCount || 0 == columnCount) {
        log_error("zero row or column");
        return;
    }
    log_info("table %d rows, %d columns", rowCount, columnCount);

    int nRow, mCol;
    for (nRow = 0; nRow < rowCount; nRow++) {
        bool isNull = true;
        QString rowString;
        QStandardItem *dateItem = mItemModel->item(nRow, 0);
        rowString.clear();
        rowString.append(dateItem->text());
        for (mCol = 1; mCol < columnCount; mCol ++) {
            rowString.append(",");
            QStandardItem *tmpItem = mItemModel->item(nRow, mCol);
            if (tmpItem != NULL) {
                isNull = false;
                rowString.append(tmpItem->text());
            } else {
                rowString.append("");
            }
        }

        if (!isNull) { // this row non null, save it
            rowString.append("\r\n");
            //log_debug("row %02d: %s", nRow, rowString.toUtf8().data());
            tmpFile.write(rowString.toUtf8().data(), rowString.toUtf8().size());
        }
    }

    // 2. clear table
    clearTableItemAll();

    // 3. fill table from the temp file data
    log_info("tmpFile size: %d", tmpFile.size());
    columnCount = mItemModel->columnCount();
    tmpFile.seek(0);
    nRow = 0;
    while (!tmpFile.atEnd()) {
        QString line = tmpFile.readLine();
        QStringList tmpList = line.split(",");
        if (columnCount == tmpList.size()) {
            for (int i = 0; i < columnCount; i ++) {
                if (!tmpList.at(i).isEmpty()) {
                    QStandardItem *tmpItem = new QStandardItem(tmpList.at(i));
                    mItemModel->setItem(nRow, i, tmpItem);
                }
            }
        } else {
            log_warn("tmpList: %d", tmpList.size());
        }

        nRow += 1;
    }

    tmpFile.close();
}
