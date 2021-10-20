/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/1/12
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <QString>
#include <QDebug>
#include <QDateTime>
#include <QThread>

#include "LogClient.h"
#include "Log.h"


#define BUF_SIZE (5 * 1024)

static LogClient *mLogClient = NULL;

static const char *g_log_level_char[LOG_DEBUG+1] = {
    "F", "E", "W", "I", "D" };

int Logger::mLevel = LOG_DEBUG;


void Logger::log(int level, const char *tag, const char *fmt, ...)
{
    char buff[BUF_SIZE];
    va_list ap;

    if (level > mLevel || level < LOG_FATAL) {
        qDebug("Invalid log level %d", level);
        return;
    }

    if (NULL == mLogClient) {
        mLogClient = new LogClient();
        mLogClient->connectServer();
    }

    if (NULL != mLogClient) {
        do {
            if (mLogClient->isLogServerConnected())
                break;

            mLogClient->connectServer();
            QThread::msleep(10);
        } while (1);
    }

    va_start(ap, fmt);
    memset(buff, 0, sizeof(buff));
    vsnprintf(buff, sizeof(buff) - 1, fmt, ap);
    if (strstr(buff, "%s") != NULL) {
        qDebug("WARN: log_debug called with '%%s' formatted string!");
        va_end(ap);
        return;
    }

    if (mLogClient) {
        QString dateString = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]");
        dateString.append("[");
        dateString.append(g_log_level_char[level]);
        dateString.append("/");
        dateString.append(tag);
        dateString.append("] ");
        dateString.append(buff);
        dateString.append("\r\n");
        mLogClient->logSend(dateString.toUtf8());
    }

    va_end(ap);
}
