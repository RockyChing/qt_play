#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMessageBox>
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

    void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void init();
    void initUI();
    void initToolBar();
    void initStatusBar();
    void initSlots();

    void createActions();   // 创建托盘右键菜单
    void createTrayIcon();  // 创建托盘对象
    void setIcon();         // 设置托盘图标

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
    void onBtnWebBrowerClicked();
    void onBtnPoGClicked();
    void onBtnPicMergeClicked();
    void onBtnExitClicked();

    void onIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::QtMainWindow *ui;
    LogViewer *mLogViewer;

    QMenuBar *mMenuBar;
    QToolBar *mToolBar;
    QStatusBar *mStatusBar;

    QSystemTrayIcon *mTrayIcon;
    QMenu *mTrayIconMenu;

    QAction *mMinAction;
    QAction *mMaxAction;
    QAction *mRestoreAction;
    QAction *mQuitAction;
};

#endif // QTMAINWINDOW_H
