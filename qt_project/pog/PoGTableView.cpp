/*
 * Copyright (C) 2020-2021 RockyChing
 *
 * Date: 2021/9/26
 * Note: UTF-8, Unix(LF)
 *
 * RockyChing <zdh1021680772@163.com>
 * You may clone this project at: https://github.com/RockyChing/qt_play.git
 */
#include "PoGTableView.h"
#include "ui_pogtableview.h"

PoGTableView::PoGTableView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PoGTableView)
{
    ui->setupUi(this);
}

PoGTableView::~PoGTableView()
{
    delete ui;
}
