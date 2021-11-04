#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QMainWindow>
#include "finance/finance.h"
#include "log/LogViewer.h"

namespace Ui {
class QtMainWindow;
}

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();

    void closeEvent(QCloseEvent *event);

private:
    void init();
    void initUI();
    void initToolBar();
    void initStatusBar();
    void initSlots();

private slots:
    void onFileActionNewClicked();
    void onFileActionOpenClicked();
    void onFileActionSaveClicked();

    void onEditActionNewlineClicked();
    void onEditActionText2HtmlClicked();
    void onEditActionLogClicked();

    void aboutSlot();
    void aboutQtSlot();

    void onBtnFinanceClicked();
    void onBtnAccountClicked();
    void onBtnHttpsClientClicked();
    void onBtnPoGClicked();
    void onBtnPicMergeClicked();
    void onBtnExitClicked();

private:
    Ui::QtMainWindow *ui;
    LogViewer *mLogViewer;

    QMenuBar *mMenuBar;
    QToolBar *mToolBar;
    QStatusBar *mStatusBar;

};

#endif // QTMAINWINDOW_H
