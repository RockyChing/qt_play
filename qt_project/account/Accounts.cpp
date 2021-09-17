/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/8/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QSettings>
#include <QInputDialog>
#include <QDebug>

#include "../config/appsettings.h"
#include "../crypto/qaes.h"
#include "AccountJsonData.h"
#include "Accounts.h"
#include "ui_accounts.h"

Accounts::Accounts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Accounts)
{
    ui->setupUi(this);
    mJsonData = new AccountJsonData();
    initUI();
    initSlot();

#if 0
    char intext[] = "Some Crypto Text";
    unsigned char key[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    unsigned char iv[] = {1,2,3,4,5,6,7,8};

    uint8_t out[1024];
    int outLen;
    QAes::AESEncrypt(key, iv, (uint8_t *)intext, strlen(intext), out, &outLen);
    qDebug("outLen: %d, %02x %02x %02x", outLen, out[0], out[1], out[2]);

    uint8_t deout[1024];
    memset(deout, 0, sizeof(deout));
    int deLen;
    QAes::AESDecrypt(key, iv, (uint8_t *)out, outLen, deout, &deLen);
    qDebug("deLen: %d, %s", deLen, deout);
#endif
}

Accounts::~Accounts()
{
    delete mJsonData;
    delete ui;
}

void Accounts::initUI()
{
    mAccountsTable = ui->tableView;
    mLineEditFile = ui->lineEditAcc;
    mBtnFileOpen = ui->btnAccountOpen;
    mBtnSave = ui->btnAccountSave;
    mCboxEncry = ui->cBoxEncrypt;

    mTableModel = new QStandardItemModel();
    mAccountsTable->setModel(mTableModel);

    QStringList tabHdr;
    //tabHdr.append(tr("ID"));
    tabHdr.append(tr("Account"));
    tabHdr.append(tr("Password"));
    mTableModel->setHorizontalHeaderLabels(tabHdr);

    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mFileAbsPath = s.value(AppSettings::ACCOUNT_PATH).toString();
    if (!mFileAbsPath.isEmpty()) {
        mLineEditFile->setText(mFileAbsPath);
    }
}

void Accounts::initSlot()
{
    connect(mBtnFileOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(mBtnSave, SIGNAL(clicked()), this, SLOT(onBtnSaveClicked()));
    connect(ui->btnAccountAdd, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
}

void Accounts::onBtnOpenClicked()
{
    QString filePath = mLineEditFile->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mFileAbsPath = QFileDialog::getOpenFileName(this, tr("Open file"), filePath);
    mFileAbsPath = QDir::toNativeSeparators(mFileAbsPath);
    if (mFileAbsPath.isEmpty()) {
        mLineEditFile->setText("no file!");
    } else {
        mLineEditFile->setText(mFileAbsPath);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::ACCOUNT_PATH, mFileAbsPath);

        if (mCboxEncry->isChecked()) {
            bool ok;
            QString pwd;
            pwd = QInputDialog::getText(this, tr("Password"), tr("Enter"), QLineEdit::Password, 0, &ok);
            mJsonData->setPassword(pwd);
        } else {
            mJsonData->clearPassword();
        }

        mJsonData->setFileName(mFileAbsPath);
        mJsonData->doAccountParser();

        QMap<QString, QString>::iterator iter = mJsonData->mAccounts.begin();
        int i = 0;
        while (iter != mJsonData->mAccounts.end()) {
            mTableModel->setItem(i, 0, new QStandardItem(QString::fromLocal8Bit(iter.key().toLocal8Bit())));
            mTableModel->setItem(i, 1, new QStandardItem(iter.value()));

            iter ++;
            i++;
        }
    }
}

void Accounts::onBtnAddClicked()
{
    bool ok = false;
    QString text;
    text = QInputDialog::getMultiLineText(this, tr("Account&Password"), tr("Enter"), tr(""), &ok);
    if (ok && !text.isEmpty()) {
        QStringList account = text.split("\n");
        int index = mJsonData->mAccounts.size();
        QString name = account.at(0);
        QString pwd = account.at(1);
        mTableModel->setItem(index, 0, new QStandardItem(name));
        mTableModel->setItem(index, 1, new QStandardItem(pwd));

        mJsonData->mAccounts.insert(name, pwd);
    }
}

void Accounts::onBtnSaveClicked()
{
    QString jsonArray;
    jsonArray.clear();
    jsonArray.append("[");
    for (int i = 0; i < mTableModel->rowCount(); i ++) {
        QStandardItem *item = mTableModel->item(i, 0);
        jsonArray.append("{\"");
        jsonArray.append(item->text());
        jsonArray.append("\":\"");

        item = mTableModel->item(i, 1);
        jsonArray.append(item->text());
        jsonArray.append("\"}");
        if (i != (mTableModel->rowCount() - 1))
            jsonArray.append(",");
    }
    jsonArray.append("]");

    QFile file("./tmp.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return;
    }

    if (mCboxEncry->isChecked()) {
        QString jsonEncry;
        jsonEncry.clear();
        jsonEncry.append("{\"type\":\"encry\",\"accounts\":");

        QByteArray plainByte = jsonArray.toLocal8Bit();
        int plainDataLen = plainByte.size();
        uint8_t *plainData = new uint8_t[plainDataLen+1];
        memcpy(plainData, plainByte.data(), plainDataLen);
        plainData[plainDataLen] = '\0';
        qDebug("plainDataLen: %d", plainDataLen);
        qDebug() << "plainData: " << QString::fromLocal8Bit((const char *)plainData);

        bool ok;
        QString pwd = QInputDialog::getText(this, tr("Password"), tr("Enter"), QLineEdit::Password, 0, &ok);
        if (ok && !pwd.isEmpty()) {
            uint8_t *enData = new uint8_t[plainDataLen + 32];
            int enDataLen;

            const uint8_t iv[] = { 0x01, 0x01, 0x07, 0x00, 0x08, 0x06, 0x05, 0x06 };
            const uint8_t *key = (const uint8_t *) pwd.toLocal8Bit().data();
            int keyLen = pwd.toLocal8Bit().size();
            QAes::AESEncrypt(key, keyLen, iv, 8, plainData, plainDataLen, enData, &enDataLen);

            QByteArray enBase64((const char *)enData, enDataLen);

            jsonEncry.append("\"");
            jsonEncry.append(enBase64.toBase64());
            jsonEncry.append("\"}");

            const char *jsonData = jsonEncry.toLocal8Bit().data();
            qint64 jsonDataLen = jsonEncry.toLocal8Bit().size();
            file.write(jsonData, jsonDataLen);
            delete enData;
        }
    } else {
        QString jsonPlain;
        jsonPlain.clear();
        jsonPlain.append("{\"type\":\"plain\",\"accounts\":");

        jsonPlain.append(jsonArray);
        jsonPlain.append("}");
        file.write(jsonPlain.toLocal8Bit());
    }

    file.close();
    QMessageBox::information(this, "Info", tr("Work done!"));
}
