#define LOG_TAG "main"
#include "log/Log.h"
#include <QMessageBox>
#include <QThread>
#include <QTime>

#include "qtmainwindow.h"
#include "ui_qtmainwindow.h"
#include "config/appsettings.h"
#include "nl_del/nl_del.h"
#include "edit/text2htmldlg.h"
#include "account/AccountView.h"
#include "pog/PoGTableView.h"
#include "picmerge/PicMergeView.h"
#include "http/HttpClientView.h"
#include "reminder/ReminderDlg.h"


QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow), mLogViewer(NULL),
    mMenuBar(NULL), mToolBar(NULL), mStatusBar(NULL),
    mReminderTimer(NULL)
{
    ui->setupUi(this);
    setWindowTitle(AppSettings::APP_VERSION());
    init();
    initToolBar();
    initStatusBar();

    initUI();
    initSlots();
}

QtMainWindow::~QtMainWindow()
{
    delete mLogViewer;
    mLogViewer = NULL;
    delete ui;
}

void QtMainWindow::setVisible(bool visible)
{
    mMinAction->setEnabled(visible);
    mMaxAction->setEnabled(!isMaximized());
    mRestoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}

void QtMainWindow::showNotification()
{
    QString info = QString("您已连续工作%1分钟，休息一下。").arg(mReminderTime, 0, 10);
    mTrayIcon->showMessage(tr("定时时间到"), info,
                           QSystemTrayIcon::Information, 0x7fffffff);
}

void QtMainWindow::closeEvent(QCloseEvent *event)
{
    if (mTrayIcon->isVisible()) {
        QMessageBox msgBox;
        msgBox.setInformativeText(tr("后台运行？"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Discard);
        int res = msgBox.exec();
        switch (res) {
        case QMessageBox::Yes:
            hide();
            event->ignore();
            break;
        case QMessageBox::No:
            if (mLogViewer) {
                mLogViewer->close();
            }
            event->accept();
            break;
        case QMessageBox::Discard:
            /* do nothing */
            event->ignore();
            break;
        }
    }
}

void QtMainWindow::init()
{
    mMenuBar = ui->menuBar;
    mToolBar = ui->toolBar;
    mStatusBar = ui->statusBar;
    mLogViewer = new LogViewer();
}

void QtMainWindow::initUI()
{
    createActions();
    createTrayIcon();
    setIcon();
    mTrayIcon->show();
}

void QtMainWindow::initToolBar()
{
    mToolBar->hide();
}

#if 0
void QtMainWindow::initMenu()
{
    QMenu *menuFile = mMenuBar->addMenu(tr("&File"));
    QMenu *menuEdit = mMenuBar->addMenu(tr("&Edit"));
    QMenu *menuHelp = mMenuBar->addMenu(tr("&Help"));

    /* set File actions */
    QAction *actNew = menuFile->addAction(QIcon(QPixmap(":/res/images/icon_file_new.png")),
            tr("&New"), this, SLOT(onNewFileClicked()), QKeySequence(tr("Ctrl+N")));
    actNew->setStatusTip(tr("Create a file..."));

    menuFile->addAction(tr("&Open"));
    menuFile->addAction(tr("&Save"));
    menuFile->addSeparator();
    menuFile->addAction(tr("&Exit"));

    /* set Edit/DelNewline actions */
    QAction *actDelNewline = menuEdit->addAction(tr("段落间隔转换"), this,
            SLOT(onDelNewlineClicked()), QKeySequence(tr("Ctrl+D")));
    actDelNewline->setStatusTip(tr("Delete new line in a file..."));
    /* set Edit/Text2Html actions */
    QAction *actText2Html = menuEdit->addAction(tr("Text to Html"), this,
            SLOT(onText2HtmlClicked()), QKeySequence(tr("Ctrl+T")));
    actText2Html->setStatusTip(tr("Text to Html..."));
}
#endif

void QtMainWindow::initStatusBar()
{
    mStatusBar->showMessage(tr("Running..."));
}

void QtMainWindow::initSlots()
{
    connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(onFileActionNewClicked()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(onFileActionOpenClicked()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(onFileActionSaveClicked()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(onBtnExitClicked()));

    connect(ui->actionNewLine, SIGNAL(triggered(bool)), this, SLOT(onEditActionNewlineClicked()));
    connect(ui->actionText2Html, SIGNAL(triggered(bool)), this, SLOT(onEditActionText2HtmlClicked()));
    connect(ui->actionLog, SIGNAL(triggered(bool)), this, SLOT(onEditActionLogClicked()));

    connect(ui->actionAboutThis, SIGNAL(triggered(bool)), this, SLOT(aboutSlot()));
    connect(ui->actionAboutQt, SIGNAL(triggered(bool)), this, SLOT(aboutQtSlot()));

    connect(ui->btnFinance, SIGNAL(clicked()), this, SLOT(onBtnFinanceClicked()));
    connect(ui->btnAccount, SIGNAL(clicked()), this, SLOT(onBtnAccountClicked()));
    connect(ui->btnHttpsClient, SIGNAL(clicked()), this, SLOT(onBtnHttpsClientClicked()));
    connect(ui->btnReminder, SIGNAL(clicked()), this, SLOT(onBtnReminderClicked()));
    connect(ui->btnPoG, SIGNAL(clicked()), this, SLOT(onBtnPoGClicked()));
    connect(ui->btnPicMerge, SIGNAL(clicked()), this, SLOT(onBtnPicMergeClicked()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(onBtnExitClicked()));

    connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
                       SLOT(onIconActivated(QSystemTrayIcon::ActivationReason)));
}

void QtMainWindow::createActions()
{
    mMinAction = new QAction(tr("Mi&nimize"), this);
    connect(mMinAction, &QAction::triggered, this, &QWidget::hide);

    mMaxAction = new QAction(tr("Ma&ximize"), this);
    connect(mMaxAction, &QAction::triggered, this, &QWidget::showMaximized);

    mRestoreAction = new QAction(tr("&Restore"), this);
    connect(mRestoreAction, &QAction::triggered, this, &QWidget::showNormal);

    mQuitAction = new QAction(tr("&Quit"), this);
    connect(mQuitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void QtMainWindow::createTrayIcon()
{
    mTrayIconMenu = new QMenu(this);
    mTrayIconMenu->addAction(mMinAction);
    mTrayIconMenu->addAction(mMaxAction);
    mTrayIconMenu->addAction(mRestoreAction);
    mTrayIconMenu->addSeparator();
    mTrayIconMenu->addAction(mQuitAction);

    mTrayIcon = new QSystemTrayIcon(this);
    mTrayIcon->setContextMenu(mTrayIconMenu);
}

void QtMainWindow::setIcon()
{
    QIcon icon(":/res/images/icon_play.png");
    mTrayIcon->setIcon(icon);
    setWindowIcon(icon);
}

void QtMainWindow::updateStatusBar(QString &text)
{
    mStatusBar->showMessage(text, 0x7fffffff);
}

void QtMainWindow::onFileActionNewClicked()
{
    log_info("file new");
}

void QtMainWindow::onFileActionOpenClicked()
{
    log_info("file open");
}

void QtMainWindow::onFileActionSaveClicked()
{
    log_info("file save");
}

void QtMainWindow::onEditActionNewlineClicked()
{
    NewlineDelete *d = new NewlineDelete();
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->show();
}

void QtMainWindow::onEditActionText2HtmlClicked()
{
    Text2HtmlDlg *d = new Text2HtmlDlg(this);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->show();
}

void QtMainWindow::onEditActionLogClicked()
{
    if (NULL != mLogViewer && mLogViewer->isHidden()) {
        mLogViewer->show();
        log_info("logd started\r\n");
    }
}

void QtMainWindow::aboutSlot()
{
    QString text = QString("<b>qt_play V%1.%2.%3</b><br/><br/>").arg(AppSettings::QT_PLAY_MAJOR).arg(AppSettings::QT_PLAY_MINOR).arg(AppSettings::QT_PLAY_MICRO);

    text.append("Based on Qt ");
    text.append(QT_VERSION_STR);
    text.append("<br/><br/>");

    text.append("Built on ");
    text.append(__DATE__);
    text.append(" ");
    text.append(__TIME__);
    text.append("<br/><br/>");

    text.append("Copyright (C) 2020-2021 RockyChing &#60;zdh1021680772@163.com&#62;<br/><br/>");

    text.append("The program is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDEING THE WARRANTY OF DESIGN, MERCHANTABLITY AND FITNESS FOR A PARTICULAR PURPOSE.");
    QMessageBox::about(this, tr("About qt_play"), text);
}

void QtMainWindow::aboutQtSlot()
{
    QMessageBox::aboutQt(this);
}

void QtMainWindow::onBtnFinanceClicked()
{
    Finance *f = new Finance();
    f->show();
}

void QtMainWindow::onBtnAccountClicked()
{
    AccountView *a = new AccountView();
    a->show();
}

void QtMainWindow::onBtnHttpsClientClicked()
{
    HttpClientView *f = new HttpClientView();
    f->show();
}

void QtMainWindow::onReminderTimeout()
{
    showNotification();
    QString text;
    if (0 == mReminderRepeat) {
        text.append("Reminder expires once.");
    } else {
        QTime now = QTime::currentTime();
        QTime future = now.addSecs(mReminderTime * 60);
        QString futureTimeString = future.toString("HH:mm:ss.zzz");

        text.append("Reminder will expires at ");
        text.append(futureTimeString);
    }
    updateStatusBar(text);
}

void QtMainWindow::onBtnReminderClicked()
{
    ReminderDlg *dlg = new ReminderDlg(this);
    mReminderTime = 0;
    mReminderRepeat = 0;
    dlg->initTimeRepeat(&mReminderTime, &mReminderRepeat);
    int res = dlg->exec(); // OK: 1, Cancel: 0
    if (res) {
        qDebug("timer: %d, repeat: %d", mReminderTime, mReminderRepeat);
        if (NULL == mReminderTimer) {
           mReminderTimer = new QTimer(this);
        }

        QString text;
        if (0 == mReminderTime) {
            text.append("Reminder disabled.");
        } else {
            mReminderTimer->setSingleShot((0 == mReminderRepeat) ? true : false);
            mReminderTimer->setInterval(mReminderTime * 60000);
            connect(mReminderTimer, SIGNAL(timeout()), this, SLOT(onReminderTimeout()));
            mReminderTimer->start();

            QTime now = QTime::currentTime();
            QTime future = now.addSecs(mReminderTime * 60);
            QString futureTimeString = future.toString("HH:mm:ss.zzz");

            text.append("Reminder will expires at ");
            text.append(futureTimeString);
        }

        updateStatusBar(text);
    }
}

void QtMainWindow::onBtnPoGClicked()
{
    PoGTableView *v = new PoGTableView();
    v->show();
}

void QtMainWindow::onBtnPicMergeClicked()
{
    PicMergeView *v = new PicMergeView();
    v->show();
}

void QtMainWindow::onBtnExitClicked()
{
    mLogViewer->close();
    this->close();
}

void QtMainWindow::onIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        break;
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}
