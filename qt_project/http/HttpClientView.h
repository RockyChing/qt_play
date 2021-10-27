/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/27
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef HTTPCLIENTVIEW_H
#define HTTPCLIENTVIEW_H

#include <QWidget>
#include "HttpClient.h"

namespace Ui {
class HttpClientView;
}

class HttpClientView : public QWidget
{
    Q_OBJECT

public:
    explicit HttpClientView(QWidget *parent = 0);
    ~HttpClientView();

private:
    void init();
    void initUI();
    void initSlot();

private slots:
    void onBtnExecuteClicked();

    void postFinishSlot(int statusCode, QByteArray& response);
    void httpErrorSlot(QNetworkReply::NetworkError err);
private:
    Ui::HttpClientView *ui;
    HttpClient *mHttpClient;
};

#endif // HTTPCLIENTVIEW_H
