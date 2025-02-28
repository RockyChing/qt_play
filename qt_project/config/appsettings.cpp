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
const QString AppSettings::ACCOUNT_PATH = "account_path";
const QString AppSettings::POG_PATH = "pog_path";

const QString AppSettings::FILE_CDIR = "file_cdir";
const QString AppSettings::FILE_RDIR = "file_rdir";

const QString AppSettings::SYNC_SDIR = "sync_sdir";
const QString AppSettings::SYNC_DDIR = "sync_ddir";

const QString AppSettings::KNOTE_SDIR = "knote_sdir";
const QString AppSettings::KNOTE_DDIR = "knote_ddir";

const QString AppSettings::R_TIMEROUT = "reminder_to";
const QString AppSettings::R_REPEAT   = "reminder_repeat";
const QString AppSettings::R_SND_FILE = "reminder_snd_file";

const QString AppSettings::HTML_PATH  = "html_path";
const QString AppSettings::HTML_CLS   = "html_class";

const QString AppSettings::IMG_CROP_PATH  = "crop_path";
const QString AppSettings::IMG_REVT_PATH  = "revt_path";

const QString AppSettings::FAV_LIST_PATH  = "fav_list_path";
const QString AppSettings::FAV_SDIR       = "fav_sdir";
const QString AppSettings::FAV_DDIR       = "fav_ddir";

const QString AppSettings::QT_PLAY_MAJOR = "1";
const QString AppSettings::QT_PLAY_MINOR = "21";
const QString AppSettings::QT_PLAY_MICRO = "2";


AppSettings::AppSettings()
{
    // QCoreApplication::applicationDirPath(); //读取应用程序可执行文件所在的目录
    QCoreApplication::setOrganizationName(ORG_NAME);
    QCoreApplication::setApplicationName(APP_NAME);
}

QString AppSettings::APP_VERSION()
{
    return APP_NAME + " (v" + QT_PLAY_MAJOR + "." + QT_PLAY_MINOR + "." + QT_PLAY_MICRO + ")";
}
