#include "filesyncdlg.h"
#include "ui_filesyncdlg.h"
#include <QSettings>
#include <QDebug>
#include <QIntValidator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QPalette>
#include <QRegExp>

#include "config/appsettings.h"
#include "utils/fileutil.h"


FileSyncDlg::FileSyncDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSyncDlg)
{
    ui->setupUi(this);

    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;

    setWindowFlags(windowFlag);
    initUI();
    initData();
}

FileSyncDlg::~FileSyncDlg()
{
    delete ui;
}

void FileSyncDlg::initUI()
{
    mBtnDirSrc = ui->btnDirSrc;
    mBtnDirDst = ui->btnDirDst;

    mEditDirSrc = ui->lineEditDirSrc;
    mEditDirDst = ui->lineEditDirDst;

    connect(mBtnDirSrc, SIGNAL(clicked()), this, SLOT(onBtnDirSrcClicked()));
}

void FileSyncDlg::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mDirSrcName = s.value(AppSettings::SYNC_SDIR).toString();
    if (!mDirSrcName.isEmpty()) {
        mEditDirSrc->setText(mDirSrcName);
    } else {
        mEditDirSrc->setText(tr("未指定目录"));
    }

    mDirDstName = s.value(AppSettings::SYNC_DDIR).toString();
    if (!mDirDstName.isEmpty()) {
        mEditDirDst->setText(mDirDstName);
    } else {
        mEditDirDst->setText(tr("未指定目录"));
    }
}

void FileSyncDlg::onBtnDirSrcClicked()
{
    QString filePath = mEditDirSrc->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mDirSrcName = QFileDialog::getExistingDirectory(this, tr("Open file"), filePath);
    mDirSrcName = QDir::toNativeSeparators(mDirSrcName);
    qDebug() << mDirSrcName;
    if (mDirSrcName.isEmpty()) {
        mEditDirSrc->setText(tr("未指定目录"));
    } else {
        mEditDirSrc->setText(mDirSrcName);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::SYNC_SDIR, mDirSrcName);
    }

    QList<QString> fileList;
    //bool res = FileUtil::fileRecursive(mDirSrcName, fileList);
    for (int i = 0; i < fileList.size(); i ++) {
        qDebug() << fileList.at(i);
    }
}
