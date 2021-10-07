#define LOG_TAG "main"
#include "log/log.h"

#include "qtmainwindow.h"
#include "ui_qtmainwindow.h"
#include "config/appsettings.h"
#include "nl_del/nl_del.h"
#include "edit/text2htmldlg.h"
#include "account/AccountView.h"
#include "pog/PoGTableView.h"
#include "picmerge/PicMergeView.h"


QtMainWindow::QtMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QtMainWindow),
    mMenuBar(NULL), mToolBar(NULL), mStatusBar(NULL)
{
    ui->setupUi(this);
    setWindowTitle(AppSettings::APP_VERSION());
    init();
    initToolBar();
    //initStatusBar();

    initSlots();
}

QtMainWindow::~QtMainWindow()
{
    delete ui;
}

void QtMainWindow::init()
{
    mMenuBar = ui->menuBar;
    mToolBar = ui->toolBar;
    mStatusBar = ui->statusBar;
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

    connect(ui->btnFinance, SIGNAL(clicked()), this, SLOT(onBtnFinanceClicked()));
    connect(ui->btnAccount, SIGNAL(clicked()), this, SLOT(onBtnAccountClicked()));
    connect(ui->btnPoG, SIGNAL(clicked()), this, SLOT(onBtnPoGClicked()));
    connect(ui->btnPicMerge, SIGNAL(clicked()), this, SLOT(onBtnPicMergeClicked()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(onBtnExitClicked()));
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
    this->close();
}

