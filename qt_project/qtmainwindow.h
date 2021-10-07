#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QMainWindow>
#include "finance/finance.h"

namespace Ui {
class QtMainWindow;
}

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();

private:
    void init();
    void initToolBar();
    void initStatusBar();
    void initSlots();

private slots:
    void onFileActionNewClicked();
    void onFileActionOpenClicked();
    void onFileActionSaveClicked();

    void onEditActionNewlineClicked();
    void onEditActionText2HtmlClicked();

    void onBtnFinanceClicked();
    void onBtnAccountClicked();
    void onBtnPoGClicked();
    void onBtnPicMergeClicked();
    void onBtnExitClicked();

private:
    Ui::QtMainWindow *ui;

    QMenuBar *mMenuBar;
    QToolBar *mToolBar;
    QStatusBar *mStatusBar;

};

#endif // QTMAINWINDOW_H
