#define LOG_TAG "main"
#include "log/log.h"

#include "qtmainwindow.h"
#include "ui_qtmainwindow.h"


QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    mMenuBar(NULL), mToolBar(NULL), mStatusBar(NULL)
{
    ui->setupUi(this);

    initMainWidow();
    initMenu();
    initToolBar();
    initStatusBar();

    initSignalSlots();
}

QtMainWindow::~QtMainWindow()
{
    delete ui;
}

void QtMainWindow::initMainWidow()
{
    mMenuBar = new QMenuBar(this);
    setMenuBar(mMenuBar);

    // mToolBar = new QToolBar(this);
    // addToolBar(mToolBar);

    mStatusBar = new QStatusBar(this);
    setStatusBar(mStatusBar);
}

void QtMainWindow::initMenu()
{
    QMenu *menuFile = mMenuBar->addMenu(tr("&File"));
    QMenu *menuedit = mMenuBar->addMenu(tr("&Edit"));
    QMenu *menuHelp = mMenuBar->addMenu(tr("&Help"));

    /* set File actions */
    QAction *actNew = menuFile->addAction(QIcon(QPixmap(":/res/images/file_new.png")),
            tr("&New"), this, SLOT(onNewFileClicked()), QKeySequence(tr("Ctrl+N")));
    actNew->setStatusTip(tr("Create a file..."));

    menuFile->addAction(tr("&Open"));
    menuFile->addAction(tr("&Save"));
    menuFile->addSeparator();
    menuFile->addAction(tr("&Exit"));
}

void QtMainWindow::initToolBar()
{

}

void QtMainWindow::initStatusBar()
{
    mStatusBar->showMessage(tr("Running..."));
}

void QtMainWindow::initSignalSlots()
{
    connect(ui->btnFinance, SIGNAL(clicked()), this, SLOT(onBtnFinanceClicked()));
}

void QtMainWindow::onNewFileClicked()
{
    log_info("onNewFileClicked");
}

void QtMainWindow::onBtnFinanceClicked()
{
    Finance *f = new Finance();
    f->show();
}
