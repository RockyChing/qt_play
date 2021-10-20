/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/10/13
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QPlainTextEdit>
#include <QTextCursor>
#include <QScrollBar>
#include <QStyle>

#define LOG_TAG "LogViewer"

#include "LogViewer.h"
#include "Log.h"
#include "ui_logviewer.h"

LogViewer::LogViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogViewer), mTimer(NULL), mLogServer(NULL), mCount(0)
{
    ui->setupUi(this);
    resize(755, 448);
    /* 屏蔽窗口关闭按钮 */
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    init();
    initUI();
}

LogViewer::~LogViewer()
{
    if (mLogServer) {
        mLogServer->stop();
        delete mLogServer;
    }

    if (mTimer) {
        mTimer->stop();
        delete mTimer;
    }
    delete ui;
}

void LogViewer::init()
{
    mLogServer = new LogServer();
    if (mLogServer) {
        mLogServer->start();
        connect(mLogServer, SIGNAL(readyRead()), this, SLOT(logDataHandler()));

        mTimer = new QTimer();
        connect(mTimer, SIGNAL(timeout()), this, SLOT(timeoutSlot()));
        mTimer->start(1000);
    }
}

void LogViewer::initUI()
{
}

void LogViewer::logDataHandler()
{
    if (mLogServer) {
        QByteArray data = mLogServer->readAll();
        QTextCursor tc = QTextCursor(ui->logTextEdit->document());
        tc.movePosition(QTextCursor::End);
        tc.insertText(QString::fromUtf8(data.data(), data.size()));
        //ui->logTextEdit->insertPlainText(text);
        QScrollBar *scrollBar = ui->logTextEdit->verticalScrollBar();
        if (scrollBar) {
            scrollBar->setSliderPosition(scrollBar->maximum());
        }
    }
}

void LogViewer::timeoutSlot()
{
    mTimer->stop();
}
