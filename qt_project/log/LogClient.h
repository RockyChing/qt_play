/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/14
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef LOGCLIENT_H
#define LOGCLIENT_H
#include <QLocalSocket>
#include <QTimer>


class LogClient : public QObject
{
    Q_OBJECT

public:
    LogClient();
    ~LogClient();

public:
    void connectServer();

    bool isLogServerConnected();
    void logSend(const QByteArray &data);

private slots:
    void connectedSlot();
    void errorSlot(QLocalSocket::LocalSocketError err);
    void disconnectedSlot();

private:
    QLocalSocket *mClient;
};

#endif // LOGCLIENT_H
