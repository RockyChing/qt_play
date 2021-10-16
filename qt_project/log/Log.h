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


#define LOG_FATAL    (0)
#define LOG_ERROR    (1)
#define LOG_WARN     (2)
#define LOG_INFO     (3)
#define LOG_DEBUG    (4)


class Logger {
public:
	static void log(int level, const char *tag, const char *fmt, ...);

private:
	static int mLevel;
};

#define log_fatal(x...)   Logger::log(LOG_FATAL, LOG_TAG, x)
#define log_error(x...)   Logger::log(LOG_ERROR, LOG_TAG, x)
#define log_warn(x...)    Logger::log(LOG_WARN,  LOG_TAG, x)
#define log_info(x...)    Logger::log(LOG_INFO,  LOG_TAG, x)
#define log_debug(x...)   Logger::log(LOG_DEBUG, LOG_TAG, x)

#endif /*_LOG_UTIL_H */
