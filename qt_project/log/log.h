/*
 * Copyright (C) 2020 RockyChing
 *
 * Date: 2021/1/12
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef _LOG_UTIL_H
#define _LOG_UTIL_H
#include <QString>


class Logger {
public:
    enum {
        LOG_FATAL = 0,
        LOG_ERROR = 1,
        LOG_WARN  = 2,
        LOG_INFO  = 3,
        LOG_DEBUG = 4
    };

public:
	static void log(int level, const char *tag, const char *fmt, ...);

    static void info(const QString tag, const QString s);

private:
	static int mLevel;
};


#define log_debug(x...)   Logger::log(Logger::LOG_DEBUG, LOG_TAG, x)
#define log_info(x...)    Logger::log(Logger::LOG_INFO,  LOG_TAG, x)
#define log_error(x...)   Logger::log(Logger::LOG_ERROR, LOG_TAG, x)


#endif /*_LOG_UTIL_H */

