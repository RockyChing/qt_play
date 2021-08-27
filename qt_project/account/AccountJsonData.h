#ifndef ACCOUNT_JSON_DATA_H
#define ACCOUNT_JSON_DATA_H


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
#define JSON_DATA_ENCRYPT "{\"type\":\"encrypt\",\"accounts\": \"MTIzNDU2NzgKIA==\"}"


class AccountJsonData {
public:
    AccountJsonData(const char *str);

    QString mDataType;
    QMap<QString, QString> mAccounts;
    QString mEncryptBase64;
};


#endif // ACCOUNT_JSON_DATA_H
