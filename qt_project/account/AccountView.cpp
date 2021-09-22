/*
 * Copyright (C) 2021 RockyChing
 *
 * Date: 2021/9/18
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QIcon>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>
#include <QDir>
#include <QAbstractItemView>
#include <QHeaderView>

#include "../config/appsettings.h"
#include "../crypto/qaes.h"
#include "AccountView.h"
#include "ui_accountview.h"

AccountView::AccountView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccountView)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("账号管理"));
    mTableColumnCount = 4;

    initUI();
    initSlot();
}

AccountView::~AccountView()
{
    delete ui;
}

void AccountView::aesTest()
{
    char intext[] = "Some Crypto Text";
    unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    unsigned char iv[] = {1,2,3,4,5,6,7,8};

    uint8_t out[1024];
    int outLen;
    QAes::AESEncrypt(key, sizeof(key), iv, sizeof(iv),
                     (uint8_t *)intext, strlen(intext), out, &outLen);
    qDebug("outLen: %d, %02x %02x %02x", outLen, out[0], out[1], out[2]);

    uint8_t deout[1024];
    memset(deout, 0, sizeof(deout));
    int deLen;
    QAes::AESDecrypt(key, sizeof(key), iv, sizeof(iv), (uint8_t *)out, outLen, deout, &deLen);
    qDebug("deLen: %d, %s", deLen, deout);
}

void AccountView::initUI()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mFileAbsPath = s.value(AppSettings::ACCOUNT_PATH).toString();
    if (!mFileAbsPath.isEmpty()) {
        ui->lineEditFile->setText(mFileAbsPath);
    }

    ui->cBoxEncrypt->setChecked(true);
    mItemModel = new QStandardItemModel(0, mTableColumnCount, this);
    mItemSelect = new QItemSelectionModel(mItemModel);
    ui->tableView->setModel(mItemModel);
    ui->tableView->setSelectionModel(mItemSelect);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // table header
    QStringList hdrList;
    hdrList.append(tr("平台"));
    hdrList.append(tr("账号"));
    hdrList.append(tr("密码"));
    hdrList.append(tr("手机"));
    mItemModel->setHorizontalHeaderLabels(hdrList);
}

void AccountView::initSlot()
{
    connect(ui->btnAccountOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(ui->btnAccountSave, SIGNAL(clicked()), this, SLOT(onBtnSaveClicked()));
    connect(ui->btnAccountAdd, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
    connect(ui->btnAccountDel, SIGNAL(clicked()), this, SLOT(onBtnDelClicked()));

    connect(mItemSelect, SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this, SLOT(onTabViewCurrentChanged(QModelIndex,QModelIndex)));
}

void AccountView::initModelFromStringList(QStringList& sList)
{
    int nRow = sList.count();
    mItemModel->setRowCount(nRow - 1);

    // table item
    QStandardItem *item;
    int i, j, k;
    for (i = 1, k = 0; i < nRow; i ++) {
        QStringList tmpList = sList.at(i).split(QRegExp("\\s+"), QString::SkipEmptyParts);
        if (tmpList.size() != mTableColumnCount)
            continue;

        for (j = 0; j < mTableColumnCount; j ++) {
            item = new QStandardItem(tmpList.at(j));
            mItemModel->setItem(k, j, item);
        }
        k += 1;
    }

    connect(mItemModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onItemChanged(QStandardItem*)));
}

void AccountView::openEncrypt(const QStringList &encrypt)
{
    bool ok;
    QString pwd = QInputDialog::getText(this, tr("Password"), tr("Enter"), QLineEdit::Password, 0, &ok);
    if (ok && !pwd.isEmpty()) {
        QString bs64 = encrypt.at(0);
        QByteArray b64Byte = bs64.toLocal8Bit();
        QByteArray enData = QByteArray::fromBase64(b64Byte);
        int enDataSize = enData.size();

        uint8_t *plainData = new uint8_t[enDataSize + 64];
        memset(plainData, 0, enDataSize + 64);
        int plainDataSize;

        const uint8_t iv[] = { 0x01, 0x01, 0x07, 0x00, 0x08, 0x06, 0x05, 0x06 };
        const uint8_t *key = (const uint8_t *) pwd.toLocal8Bit().data();
        int res = QAes::AESDecrypt(key, pwd.toLocal8Bit().size(), iv, 8, (uint8_t *)enData.data(), enDataSize, plainData, &plainDataSize);
        if (res != 0) {
            QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("解密错误！"));
            return;
        }

        qDebug("decry res: %d, plainDataLen: %d", res, plainDataSize);
        QString plainDataString = QString::fromLocal8Bit((const char *)plainData, plainDataSize);
        qDebug() << "plainData: " << plainDataString;

        QStringList sList = plainDataString.split("\r\n");
        QString lastItem = sList.at(sList.size() - 1);
        if (lastItem.isEmpty()) {
            sList.removeLast();
        }
        initModelFromStringList(sList);
    }
}

void AccountView::saveEncrypt(QTextStream &outStream, const QString &plain)
{
    bool ok;
    QString pwd = QInputDialog::getText(this, tr("Password"), tr("Enter"), QLineEdit::Password, 0, &ok);
    if (ok && !pwd.isEmpty()) {
        QByteArray plainByte = plain.toLocal8Bit();
        int plainByteSize = plainByte.size();
        uint8_t *plainData = new uint8_t[plainByteSize+1];
        memcpy(plainData, plainByte.data(), plainByteSize);
        plainData[plainByteSize] = '\0';

        uint8_t *enData = new uint8_t[plainByteSize + 64];
        int enDataSize;

        const uint8_t iv[] = { 0x01, 0x01, 0x07, 0x00, 0x08, 0x06, 0x05, 0x06 };
        const uint8_t *key = (const uint8_t *) pwd.toLocal8Bit().data();
        int keyLen = pwd.toLocal8Bit().size();
        int res = QAes::AESEncrypt(key, keyLen, iv, 8, plainData, plainByteSize, enData, &enDataSize);
        if (res) {
            QMessageBox::information(this, tr("账号"), tr("加密失败！"));
            delete plainData;
            delete enData;
            return;
        }

        QByteArray enBase64((const char *)enData, enDataSize);
        QByteArray bs64 = enBase64.toBase64();
        QString result = bs64;
        qDebug() << "result: " << result;
        outStream << result;
        delete plainData;
        delete enData;
    }
}

void AccountView::onBtnOpenClicked()
{
    QString filePath = ui->lineEditFile->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mFileAbsPath = QFileDialog::getOpenFileName(this, tr("Open file"), filePath, "(*.txt)");
    mFileAbsPath = QDir::toNativeSeparators(mFileAbsPath);
    if (mFileAbsPath.isEmpty()) {
        ui->lineEditFile->setText("no file!");
    } else {
        ui->lineEditFile->setText(mFileAbsPath);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::ACCOUNT_PATH, mFileAbsPath);

        QStringList fileContent;
        QFile file(mFileAbsPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            while (!stream.atEnd()) {
                QString s = stream.readLine();
                fileContent.append(s);
            }

            file.close();
            if (ui->cBoxEncrypt->isChecked()) {
                // encrypt data
                openEncrypt(fileContent);
            } else {
                // plain data
                initModelFromStringList(fileContent);
            }
        }
    }
}

void AccountView::onBtnSaveClicked()
{
    QFile file("./tmp.txt");
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate)) {
        return;
    }

    QTextStream stream(&file);
    QStandardItem *item;
    QString s("");
    int i, j;

    for (i = 0; i < mItemModel->columnCount(); i ++) {
        item = mItemModel->horizontalHeaderItem(i);
        s = s + item->text() + "\t\t";
    }
    s = s +  "\r\n";

    for (i = 0; i < mItemModel->rowCount(); i ++) {
        for (j = 0; j < mItemModel->columnCount(); j ++) {
            //qDebug("i, j: %d, %d", i, j);
            item = mItemModel->item(i, j);
            if (item == NULL) {
                s = s + "" + "\t\t";
            } else {
                s = s + item->text() + "\t\t";
            }
        }
        s = s +  "\r\n";
    }

    if (ui->cBoxEncrypt->isChecked()) {
        // save as encrypt data
        saveEncrypt(stream, s);
    } else {
        // save as plain data
        stream << s;
    }

    file.close();
    QMessageBox::information(this, tr("账号保存"), tr("Work done!"));
}

void AccountView::onBtnAddClicked()
{
    int nRow = mItemModel->rowCount();
    QStandardItem *item;
    for (int i = 0; i < mTableColumnCount; i ++) {
        item = new QStandardItem("");
        mItemModel->setItem(nRow, 0, item);
    }
}

void AccountView::onBtnDelClicked()
{
    QModelIndex cur = mItemSelect->currentIndex();
    if (cur.row() == mItemModel->rowCount() - 1) { // delete the last row
        mItemModel->removeRow(cur.row());
    } else {
        mItemModel->removeRow(cur.row());
        mItemSelect->setCurrentIndex(cur, QItemSelectionModel::Select);
    }
}

void AccountView::onTabViewCurrentChanged(const QModelIndex &current, const QModelIndex &prev)
{
    qDebug() << QString::asprintf("当前单元格： %d行， %d列", current.row(), current.column());
    QStandardItem *item = mItemModel->itemFromIndex(current);
    qDebug() << "单元格内容：" << item->text();
}

void AccountView::onItemChanged(QStandardItem *item)
{
    //qDebug() << QString::asprintf("%d行发生变化", item->column());
    //ui->tableView->resizeColumnToContents(item->column());
    //ui->tableView->horizontalHeader()->setSectionResizeMode(item->column(), QHeaderView::ResizeToContents);
}
