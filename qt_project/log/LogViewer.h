/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/13
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef LOGVIEWER_H
#define LOGVIEWER_H

#include <QWidget>
#include <QTimer>
#include "LogServer.h"

namespace Ui {
class LogViewer;
}

class LogViewer : public QWidget
{
    Q_OBJECT

public:
    explicit LogViewer(QWidget *parent = 0);
    ~LogViewer();

private:
    void init();
    void initUI();

private slots:
    void logDataHandler();
    void timeoutSlot();

private:
    Ui::LogViewer *ui;
    QTimer *mTimer;
    LogServer *mLogServer;
    int mCount;
};

#endif // LOGVIEWER_H
