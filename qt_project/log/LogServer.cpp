/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/13 2021/10/14
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QtNetwork>
#include <QLocalSocket>
#include <QDebug>

#include "LogServer.h"


LogServer::LogServer() : mServer(NULL)
{
    mServer = new QLocalServer();
    if (mServer) {
        QLocalServer::removeServer("logd");
        mServer->listen("logd");
    }
}

LogServer::~LogServer()
{
    if (mServer) {
        disconnect(mServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
        delete mServer;
    }
}

bool LogServer::start()
{
    if (!mServer->isListening()) {
        return false;
    }

    connect(mServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    return true;
}

void LogServer::stop()
{
    if (mServer) {
        mServer->close();
    }

    for (int i = 0; i < mClients.size(); i ++) {
        QLocalSocket *sock = mClients.at(i);
        if (sock) {
            sock->disconnected();
            if (sock->state() != QLocalSocket::UnconnectedState) {
                sock->abort();
            }

            clientSignalDisconn(sock);
            delete sock;
        }
    }
    mClients.clear();
}

void LogServer::acceptConnection()
{
    for (int i = 0; i < mClients.size(); i ++) {
        QLocalSocket *sock = mClients.at(i);
        if (sock) {
            sock->disconnected();
            if (sock->state() != QLocalSocket::UnconnectedState) {
                sock->abort();
            }

            clientSignalDisconn(sock);
            delete sock;
        }
    }
    mClients.clear();

    bool hasConnections = false;
    while (mServer->hasPendingConnections()) {
        QLocalSocket *sock = mServer->nextPendingConnection();
        if (hasConnections) {
            sock->disconnected();
            if (sock->state() != QLocalSocket::UnconnectedState) {
                sock->abort();
            }

            delete sock;
        } else {
            hasConnections = true;
            clientSignalConn(sock);
            mClients.append(sock);
        }
    }
}


void LogServer::readyReadSlot()
{
    emit readyRead();
}

QByteArray LogServer::readAll()
{
    QLocalSocket *sock = mClients.at(0);
    return sock->readAll();
}

void LogServer::errorSlot(QLocalSocket::LocalSocketError err)
{
    QString errString = QString("Server err %1").arg(err);
    qDebug() << errString;
}

void LogServer::clientSignalConn(QLocalSocket *sock)
{
    // receive data
    connect(sock, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));

    // error occurred
    connect(sock, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(errorSlot(QLocalSocket::LocalSocketError)));

    // disconnected
    connect(sock, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

void LogServer::clientSignalDisconn(QLocalSocket *sock)
{
    disconnect(sock, SIGNAL(readyRead()), this, SLOT(readyReadSlot()));
    disconnect(sock, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(errorSlot(QLocalSocket::LocalSocketError)));
    disconnect(sock, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}
