/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/1/12
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include "qtmainwindow.h"
#include <QApplication>

#include "usb/usb_util.h"
#include "config/appsettings.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtMainWindow w;
    w.show();

    AppSettings settings;
    //UsbUtil usbTest(0, 0);
    return a.exec();
}

