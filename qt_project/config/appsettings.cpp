/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/3/22
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include <QCoreApplication>

#include <QDebug>
#include "appsettings.h"


const QString AppSettings::ORG_NAME = "Geegle";
const QString AppSettings::APP_NAME = "qt_play";
const QString AppSettings::APP_SETTINGS_FILE = "configs.ini";

const QString AppSettings::NL_PATH = "nlf_path";


AppSettings::AppSettings()
{
    // QCoreApplication::applicationDirPath(); //读取应用程序可执行文件所在的目录
    QCoreApplication::setOrganizationName(ORG_NAME);
    QCoreApplication::setApplicationName(APP_NAME);
}
