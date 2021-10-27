/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QSsl>
#include <QUrl>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>

#define LOG_TAG "HttpClient"
#include "../log/log.h"
#include "HttpClient.h"


HttpClient::HttpClient() : mAccessManager(NULL)
{
    log_debug("NetworkAccessManager init...");
    mAccessManager = new QNetworkAccessManager(this);
    log_debug("NetworkAccessManager init done.");
}

HttpClient::~HttpClient()
{
    log_info("desc");
    if (mAccessManager) {
        delete mAccessManager;
        mAccessManager = NULL;
    }
}

void HttpClient::doRequest(HttpClient::HttpMethod method, QString &url)
{
    log_info("do request...");
    QNetworkRequest request;

    QSslConfiguration cfg = request.sslConfiguration();
    cfg.setPeerVerifyMode(QSslSocket::VerifyNone);
    cfg.setProtocol(QSsl::AnyProtocol);

    request.setSslConfiguration(cfg);
    request.setUrl(QUrl(url));

    if (method == POST) {
        request.setRawHeader("Content-Type: ", "application/json;charset=utf-8");
        request.setRawHeader("Accept", "application/json");

        QByteArray data;
        data.clear();
        mReply = mAccessManager->post(request, data);
    } else if (method == GET) {
        request.setRawHeader("Content-Type: ", "image/png");
        request.setRawHeader("Accept", "image/png");
        mReply = mAccessManager->get(request);
    }
    connect(mReply, SIGNAL(finished()), this, SLOT(postFinishSlot()));
    connect(mReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpErrorSlot(QNetworkReply::NetworkError)));
}

void HttpClient::postFinishSlot()
{
    QVariant statusCode = mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int code = statusCode.toInt();
    QByteArray resp = mReply->readAll();
    QList<QByteArray> hdrNames = mReply->rawHeaderList();

    log_info("response headers:");
    for (int i = 0; i < hdrNames.size(); i ++) {
        QByteArray hdrName = hdrNames.at(i);
        QByteArray hdrCtx = mReply->rawHeader(hdrName);
        log_debug("%s: %s", hdrName.constData(), hdrCtx.constData());
    }
    emit postFinish(code, resp);
}

void HttpClient::httpErrorSlot(QNetworkReply::NetworkError err)
{
    emit httpError(err);
}
