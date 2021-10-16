/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/13 2021/10/14
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef LOGSERVER_H
#define LOGSERVER_H
#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>


class LogServer : public QObject
{
    Q_OBJECT

public:
    explicit LogServer();

    ~LogServer();

public:
    bool start();
    void stop();
    QByteArray readAll();

signals:
    void readyRead();

private slots:
    void acceptConnection();

    void readyReadSlot();
    void errorSlot(QLocalSocket::LocalSocketError err);

private:
    void clientSignalConn(QLocalSocket *sock);
    void clientSignalDisconn(QLocalSocket *sock);

private:
    QLocalServer *mServer;
    QList<QLocalSocket *> mClients;
};

#endif // LOGSERVER_H
