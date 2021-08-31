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
    const char *ptr = str.toLatin1().data();
    jsonParser(ptr);
}

AccountJsonData::AccountJsonData(QFile &file)
{
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            QString str = in.readAll();
            if (str.size() > 0) {
                const char *ptr = str.toLatin1().data();
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
                QJsonValue accounts = obj.value("accounts");
                if (!accounts.isNull()) {
                    if (accounts.isArray() && mDataType.contains("plain", Qt::CaseSensitive)) {
                        QJsonArray ja = accounts.toArray();
                        for (int i = 0; i < ja.count(); i ++) {
                            QJsonValue aVal = ja.at(i);
                            QJsonObject aObj = aVal.toObject();
                            QStringList keys = aObj.keys();
                            QString key = keys.at(0);
                            QString pwd = aObj.value(key).toString();
                            mAccounts.insert(key, pwd);
                            qDebug() << key << pwd;
                        }

                        qDebug("Accounts: %d", mAccounts.size());
                    } else if (accounts.isString() && mDataType.contains("encrypt", Qt::CaseSensitive)) {
                        mEncryptBase64 = accounts.toString();
                        qDebug() << mEncryptBase64;
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

void AccountJsonData::doAccountParser()
{
    if (mFile.exists()) {
        if (mFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&mFile);
            QString str = in.readAll();
            if (str.size() > 0) {
                const char *ptr = str.toLatin1().data();
                jsonParser(ptr);
            }

            mFile.close();
        }
    }
}
