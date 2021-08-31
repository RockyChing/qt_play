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
        }

        mJsonData->setFileName(mFileAbsPath);
        mJsonData->doAccountParser();

        QMap<QString, QString>::iterator iter = mJsonData->mAccounts.begin();
        int i = 0;
        while (iter != mJsonData->mAccounts.end()) {
            mTableModel->setItem(i, 0, new QStandardItem(iter.key()));
            mTableModel->setItem(i, 1, new QStandardItem(iter.value()));

            iter ++;
            i++;
        }
    }
}

void Accounts::onBtnSaveClicked()
{
    QString jsonStr;
    jsonStr.clear();
    jsonStr.append("{\"type\":\"plain\",\"accounts\":");

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
        int enSize = jsonArray.size();
        const char *enData = jsonArray.toLatin1().data();
        bool ok;
        QString pwd = QInputDialog::getText(this, tr("Password"), tr("Enter"), QLineEdit::Password, 0, &ok);
        if (ok && !pwd.isEmpty()) {
            uint8_t *out = new uint8_t[enSize + 32];
            int outLen;

            const uint8_t iv[] = { 0x01, 0x01, 0x07, 0x00, 0x08, 0x06, 0x05, 0x06 };
            const uint8_t *key = (const uint8_t *) pwd.toLatin1().data();
            QAes::AESEncrypt(key, iv, (uint8_t *)enData, enSize, out, &outLen);
            QByteArray enBase64;
            enBase64.clear();
            enBase64.append((const char *)out, outLen);

            jsonStr.append("\"");
            jsonStr.append(enBase64.toBase64());
            jsonStr.append("\"}");

            file.write((const char *)jsonStr.toLatin1().data(), jsonStr.size());
            delete out;
        }
    } else {
        jsonStr.append(jsonArray);
        jsonStr.append("}");
        file.write(jsonStr.toLatin1());
    }

    file.close();
    QMessageBox::information(this, "Info", tr("Work done!"));
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
