/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#define LOG_TAG "HttpView"
#include "../log/log.h"

#include <QImage>
#include <QPixmap>

#include "HttpClientView.h"
#include "ui_httpclientview.h"


HttpClientView::HttpClientView(QWidget *parent) :
    QWidget(parent), mHttpClient(NULL),
    ui(new Ui::HttpClientView)
{
    ui->setupUi(this);
    init();
    initUI();
    initSlot();
}

HttpClientView::~HttpClientView()
{
    if (mHttpClient) {
        delete mHttpClient;
        mHttpClient = NULL;
    }
    delete ui;
}

void HttpClientView::init()
{
    mHttpClient = new HttpClient();
    connect(mHttpClient, SIGNAL(postFinish(int,QByteArray&)), this, SLOT(postFinishSlot(int,QByteArray&)));
    connect(mHttpClient, SIGNAL(httpError(QNetworkReply::NetworkError)), this, SLOT(httpErrorSlot(QNetworkReply::NetworkError)));
}

void HttpClientView::initUI()
{
    ui->labelImage->setStyleSheet("{border:2px dotted #242424;}");
}

void HttpClientView::initSlot()
{
    connect(ui->btnExecute, SIGNAL(clicked()), this, SLOT(onBtnExecuteClicked()));
}

void HttpClientView::onBtnExecuteClicked()
{
    int scheme = ui->cBoxScheme->currentIndex();
    int method = ui->cBoxMethod->currentIndex();
    ui->plainTextEdit->appendPlainText("request...");
    log_info("http scheme %d, method %d", scheme, method);

    if (0 == method) {
        QString url("https://httpbin.org/post");
        mHttpClient->doRequest(HttpClient::POST, url);
    } else if (1 == method) {
        QString url("https://httpbin.org/image/png");
        mHttpClient->doRequest(HttpClient::GET, url);
    }
}

void HttpClientView::postFinishSlot(int statusCode, QByteArray& response)
{
    int size = response.size();
    ui->plainTextEdit->appendPlainText("request finish.");
    log_info("response code: %d, data size: %d", statusCode, size);
    if (size > 0) {
        if (0 == ui->cBoxMethod->currentIndex()) {
            ui->plainTextEdit->appendPlainText(QString(response));
        } else if (1 == ui->cBoxMethod->currentIndex()) {
            QImage img;
            bool res = img.loadFromData(response);
            if (res) {
                ui->labelImage->setAlignment(Qt::AlignCenter);
                ui->labelImage->setPixmap(QPixmap::fromImage(img));
            } else {
                log_error("error convert image from http response data!");
            }
        }
    }
}

void HttpClientView::httpErrorSlot(QNetworkReply::NetworkError err)
{
    log_error("http error: %d\r\n", err);
}
