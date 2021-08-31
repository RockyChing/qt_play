#ifndef ACCOUNT_JSON_DATA_H
#define ACCOUNT_JSON_DATA_H
#include <QFile>
#include <QTextStream>

/**
 * plain data
 *
{
    "type":"plain",
    "accounts": [
        {
            "a0":"100"
        },

        {
            "a1":"101"
        }
    ]
}
 */
#define JSON_DATA_PLAIN "{\"type\":\"plain\",\"accounts\": [{\"a0\":\"100\"},{\"a1\":\"101\"},{\"a2\":\"102\"}]}"

/**
 * encrypt data
 *
{
    "type":"encrypt",
    "accounts": "base64_data"
}
 */
#define JSON_DATA_ENCRYPT "{\"type\":\"encry\",\"accounts\": \"MTIzNDU2NzgKIA==\"}"


class AccountJsonData {
public:
    AccountJsonData();
    AccountJsonData(const char *c_str);
    AccountJsonData(const QString& str);
    AccountJsonData(QFile& file);

public:
    void setFileName(const QString &name);
    void setPassword(const QString &name);
    void clearPassword();
    void doAccountParser();

private:
    void jsonParser(const char *c_str);

private:
    QString mDataType;
    QString mPwd;
    QFile mFile;

public:
    QMap<QString, QString> mAccounts;
    QString mEncryptBase64;
};


#endif // ACCOUNT_JSON_DATA_H
