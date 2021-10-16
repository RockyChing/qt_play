/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/14
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QDebug>
#include <QThread>
#include <QDateTime>

#include "LogClient.h"


LogClient::LogClient(): mClient(NULL)
{
    mClient = new QLocalSocket();
    if (mClient) {
        // connect
        connect(mClient, SIGNAL(connected()), this, SLOT(connectedSlot()));

        // error occurred
        connect(mClient, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(errorSlot(QLocalSocket::LocalSocketError)));

        // disconnected
        connect(mClient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));
    }
}

LogClient::~LogClient()
{
    if (mClient) {
        mClient->disconnectFromServer();
        delete mClient;
    }
}

bool LogClient::isLogServerConnected()
{
    return mClient && (mClient->state() == QLocalSocket::ConnectedState);
}

void LogClient::logSend(const QByteArray &data)
{
    if (mClient)
        mClient->write(data);
}

void LogClient::connectServer()
{
    if (mClient) {
        mClient->connectToServer("logd");
        if (mClient->waitForConnected(5000)) {
            qDebug("Connected!");
        }
    }
}

void LogClient::connectedSlot()
{
    qDebug() << "connect server success.";
}

void LogClient::errorSlot(QLocalSocket::LocalSocketError err)
{
    QString errString = QString("client err %1").arg(err);
    qDebug() << errString;
    mClient->close();
}

void LogClient::disconnectedSlot()
{
    qDebug("client disconnected");
}
