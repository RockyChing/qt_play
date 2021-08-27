#include <QObject>
#include <QString>
#include <QByteArray>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "AccountJsonData.h"

AccountJsonData::AccountJsonData(const char *str)
{
    qDebug("json: %s", str);
    QByteArray byteArr(str, strlen(str));

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
