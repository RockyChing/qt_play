/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/9/26
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef POGTABLEINFO_H
#define POGTABLEINFO_H
#include <QString>

/**
 * @brief The PoGTableInfo class
 * PoG - Price of Goods
 */
class PoGTableInfo
{
public:
    PoGTableInfo();

private:
    int mTableColumn;
    QString mGoodsUnit; /* RBM/500g */
};

#endif // POGTABLEINFO_H
