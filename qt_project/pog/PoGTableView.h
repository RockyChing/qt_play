/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/9/26
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#ifndef POGTABLEVIEW_H
#define POGTABLEVIEW_H

#include <QWidget>

namespace Ui {
class PoGTableView;
}

class PoGTableView : public QWidget
{
    Q_OBJECT

public:
    explicit PoGTableView(QWidget *parent = 0);
    ~PoGTableView();

private:
    Ui::PoGTableView *ui;
};

#endif // POGTABLEVIEW_H
