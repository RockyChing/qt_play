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
#include <QString>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include "PoGItem.h"

namespace Ui {
class PoGTableView;
}

class PoGTableView : public QWidget
{
    Q_OBJECT

public:
    const static int PoG_YEAR_START;
    const static int PoG_MON_START;
    const static int PoG_DAY_START;

public:
    explicit PoGTableView(QWidget *parent = 0);
    ~PoGTableView();

private:
    void init();
    void initUI();
    void initSlot();

    void initTableModel();
    void initTableHeader(QStringList& hdrList);
    void initTableDate();

    int fileParserOne(QString filePath, QList<PoGItem>& items);
    int fileFillTable(int column, QList<PoGItem>& items);

    void clearEmptyItem();

private slots:
    void onBtnOpenClicked();

private:
    /* ui members */
    Ui::PoGTableView *ui;

private:
    /* members */
    QString mFileAbsPath;
    QStandardItemModel *mItemModel;
    QItemSelectionModel *mItemSelect;

    int mTableColumnCount;
};

#endif // POGTABLEVIEW_H
