#define LOG_TAG "main"
#include "log/log.h"

#include "qtmainwindow.h"
#include "ui_qtmainwindow.h"
#include "config/appsettings.h"
#include "nl_del/nl_del.h"
#include "edit/text2htmldlg.h"


QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    mMenuBar(NULL), mToolBar(NULL), mStatusBar(NULL)
{
    ui->setupUi(this);
    setWindowTitle(AppSettings::APP_VERSION());
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
    QMenu *menuEdit = mMenuBar->addMenu(tr("&Edit"));
    QMenu *menuHelp = mMenuBar->addMenu(tr("&Help"));

    /* set File actions */
    QAction *actNew = menuFile->addAction(QIcon(QPixmap(":/res/images/file_new.png")),
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

void QtMainWindow::onDelNewlineClicked()
{
    NewlineDelete *d = new NewlineDelete();
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->show();
}

void QtMainWindow::onText2HtmlClicked()
{
    Text2HtmlDlg *d = new Text2HtmlDlg(this);
    d->setAttribute(Qt::WA_DeleteOnClose);
    d->show();
}

void QtMainWindow::onBtnFinanceClicked()
{
    Finance *f = new Finance();
    f->show();
}
