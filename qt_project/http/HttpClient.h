/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H
#include <QString>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>


class HttpClient : public QObject
{
    Q_OBJECT

public:
    enum HttpMethod{
        POST = 0,
        GET, DELETE, PUT
    };
    Q_ENUM(HttpMethod)

    HttpClient();
    ~HttpClient();

    void doRequest(HttpClient::HttpMethod method, QString& url);

private slots:
    void postFinishSlot();
    void httpErrorSlot(QNetworkReply::NetworkError err);

signals:
    void postFinish(int statusCode, QByteArray& response);
    void httpError(QNetworkReply::NetworkError err);

private:
    QNetworkAccessManager *mAccessManager;
    QNetworkReply *mReply;
};

#endif // HTTPSCLIENT_H
