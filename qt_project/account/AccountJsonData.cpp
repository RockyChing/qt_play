/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/8/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QObject>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "AccountJsonData.h"
#include "../crypto/qaes.h"

AccountJsonData::AccountJsonData()
{
    mPwd.clear();
}

AccountJsonData::AccountJsonData(const char *c_str)
{
    jsonParser(c_str);
}

AccountJsonData::AccountJsonData(const QString &str)
{
    const char *ptr = str.toLocal8Bit().data();
    jsonParser(ptr);
}

AccountJsonData::AccountJsonData(QFile &file)
{
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString str = in.readAll();
            if (str.size() > 0) {
                const char *ptr = str.toLocal8Bit().data();
                jsonParser(ptr);
            }

            file.close();
        }
    }
}

void AccountJsonData::jsonParser(const char *c_str)
{
    qDebug("json: %s", c_str);
    QByteArray byteArr(c_str, strlen(c_str));

    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArr, &jsonError);

    if (!jsonDoc.isNull() && !jsonDoc.isEmpty() && jsonError.error == QJsonParseError::NoError) {
        if (jsonDoc.isObject()) {
            QJsonObject obj = jsonDoc.object();
            if (obj.contains("type")) {
                QJsonValue type = obj.value("type");
                if (type.isString()) {
                    mDataType = type.toString();
                    qDebug() << mDataType;
                }
            }

            if (obj.contains("accounts")) {
                QJsonValue accounts;
                if (mDataType.contains("plain", Qt::CaseSensitive)) {
                    accounts = obj.value("accounts");
                } else {
                    QJsonValue tmp = obj.value("accounts");
                    QString b64Data = tmp.toString();

                    QByteArray b64Byte = b64Data.toLocal8Bit();
                    QByteArray enData = QByteArray::fromBase64(b64Byte);
                    int enDataLen = enData.size();

                    uint8_t *plainData = new uint8_t[enDataLen + 32];
                    memset(plainData, 0, enDataLen + 32);
                    int plainDataLen;

                    const uint8_t iv[] = { 0x01, 0x01, 0x07, 0x00, 0x08, 0x06, 0x05, 0x06 };
                    const uint8_t *key = (const uint8_t *) mPwd.toLocal8Bit().data();
                    int res = QAes::AESDecrypt(key, mPwd.size(), iv, 8, (uint8_t *)enData.data(), enDataLen, plainData, &plainDataLen);
                    if (res != 0) {
                        QMessageBox::warning(NULL, QObject::tr("Warning"), QObject::tr("解密错误！"));
                        return;
                    }

                    qDebug("decry res: %d, plainDataLen: %d", res, plainDataLen);
                    qDebug() << "plainData: " << QString::fromLocal8Bit((const char *)plainData, plainDataLen);
                    QString plainString;
                    plainString.clear();
                    plainString.append("{\"accounts\":");
                    plainString.append(QString::fromLocal8Bit((const char *)plainData, plainDataLen));
                    plainString = plainString.left(plainDataLen+12);
                    plainString.append("}");

                    delete plainData;

                    QJsonDocument doc = QJsonDocument::fromJson(plainString.toUtf8(), &jsonError);
                    if (jsonError.error == QJsonParseError::NoError) {
                        accounts = doc.object().value("accounts");
                    } else {
                        QMessageBox::warning(NULL, QObject::tr("Warning"), jsonError.errorString());
                    }
                }

                if (!accounts.isNull()) {
                    if (accounts.isArray()) {
                        QJsonArray ja = accounts.toArray();
                        for (int i = 0; i < ja.count(); i ++) {
                            QJsonValue aVal = ja.at(i);
                            QJsonObject aObj = aVal.toObject();
                            QStringList keys = aObj.keys();
                            QString key = keys.at(0);
                            QString pwd = aObj.value(key).toString();
                            mAccounts.insert(key, pwd);
                            qDebug() << key.toLocal8Bit() << pwd;
                        }

                        qDebug("Accounts: %d", mAccounts.size());
                    }
                }
            }
        }
    } else {
        QMessageBox::warning(NULL, QObject::tr("Warning"), jsonError.errorString());
    }
}

void AccountJsonData::setFileName(const QString &name)
{
    mFile.setFileName(name);
}

void AccountJsonData::setPassword(const QString &name)
{
    mPwd.append(name);
}

void AccountJsonData::clearPassword()
{
    mPwd.clear();
}

void AccountJsonData::doAccountParser()
{
    if (mFile.exists()) {
        if (mFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&mFile);
            QString str = in.readAll();
            mFile.close();
            if (str.size() > 0) {
                const char *ptr = str.toLocal8Bit().data();
                jsonParser(ptr);
            }
        }
    }
}
