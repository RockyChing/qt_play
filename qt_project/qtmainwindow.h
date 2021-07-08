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
    void initMainWidow();
    void initMenu();
    void initToolBar();
    void initStatusBar();
    void initSignalSlots();

private slots:
    void onNewFileClicked();
    void onDelNewlineClicked();
    void onText2HtmlClicked();
    void onBtnFinanceClicked();
    void onBtnExitClicked();

private:
    Ui::QtMainWindow *ui;

    QMenuBar *mMenuBar;
    QToolBar *mToolBar;
    QStatusBar *mStatusBar;

};

#endif // QTMAINWINDOW_H
