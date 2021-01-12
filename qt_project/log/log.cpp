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

#include <QDebug>

#include "log.h"


#define BUF_SIZE (1024)

static const char g_log_level_char[] = {
    'F', 'E', 'W', 'I', 'D' };

int Logger::mLevel = LOG_DEBUG;


void Logger::log(int level, const char *tag, const char *fmt, ...)
{
	char buff[BUF_SIZE];
	va_list ap;

	if (level > mLevel || level < 0) {
		return;
	}

	va_start(ap, fmt);
	memset(buff, 0, sizeof(buff));
	vsnprintf(buff, sizeof(buff) - 1, fmt, ap);
	if (strstr(buff, "%s") != NULL) {
		qDebug("WARN: log_debug called with '%%s' formatted string!");
		va_end(ap);
		return;
	}

    qDebug("%c/%s %s", g_log_level_char[level], tag, buff);
	va_end(ap);
}

