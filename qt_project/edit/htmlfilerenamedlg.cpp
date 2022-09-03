#include <QSettings>
#include <QDebug>
#include <QIntValidator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QMessageBox>
#include <QPalette>
#include <QRegExp>
#include "htmlfilerenamedlg.h"
#include "ui_htmlfilerenamedlg.h"
#include "config/appsettings.h"
#include "utils/rfileutil.h"

HtmlFileRenameDlg::HtmlFileRenameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HtmlFileRenameDlg)
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

HtmlFileRenameDlg::~HtmlFileRenameDlg()
{
    delete ui;
}

void HtmlFileRenameDlg::initUI()
{
    mBtnDirOpen = ui->btnDirOpen;
    mBtnRename = ui->btnRename;

    mEditDirShow = ui->lineEditDirShow;
    mEditFilePrefix = ui->lineEditFilePrefix;

    mCBBackup = ui->checkBoxBackup;

    connect(mBtnDirOpen, SIGNAL(clicked()), this, SLOT(onBtnDirOpenClicked()));
    connect(mBtnRename, SIGNAL(clicked()), this, SLOT(onBtnRenameClicked()));
}

void HtmlFileRenameDlg::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mDirName = s.value(AppSettings::HTML_RDIR).toString();
    if (!mDirName.isEmpty()) {
        mEditDirShow->setText(mDirName);
    } else {
        mEditDirShow->setText(tr("未指定目录"));
    }
}

void HtmlFileRenameDlg::msgWarning(QString text)
{
    QMessageBox::warning(this, tr("警告"), text);
}

void HtmlFileRenameDlg::onBtnDirOpenClicked()
{
    QString filePath = mEditDirShow->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mDirName = QFileDialog::getExistingDirectory(this, tr("Open file"), filePath);
    mDirName = QDir::toNativeSeparators(mDirName);
    qDebug() << mDirName;
    if (mDirName.isEmpty()) {
        mEditDirShow->setText(tr("未指定目录"));
    } else {
        mEditDirShow->setText(mDirName);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::HTML_RDIR, mDirName);
    }
}

void HtmlFileRenameDlg::onBtnRenameClicked()
{
    // 1.check parameter
    if (mDirName.isEmpty()) {
        msgWarning(tr("未指定目录！"));
        return;
    }

    QDir dir(mDirName);
    if (!dir.exists()) {
        msgWarning(tr("指定目录不存在！"));
        return;
    }

    QString filePrefix = mEditFilePrefix->text();
    if (filePrefix.isEmpty()) {
        msgWarning(tr("未指定文件前缀！"));
        return;
    }

    // 2.traverse files
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Readable, QDir::Name);
    if (fileInfoList.size() <= 0) {
        qWarning("目录下文件空");
        return;
    }

    if (mCBBackup->isChecked()) {
        bool res = RFileUtil::copyDir(mDirName, mDirName + "bak", false);
        qDebug() << "copyDir result: " << res;
    }

    // 3.rename files
    int pos;
    int fileCount = fileInfoList.size();
    for (int i = 0; i < fileCount; i ++) {
        QFileInfo fileInfo = fileInfoList.at(i);
        QString filePath = fileInfo.absoluteFilePath();
        QString fileName = fileInfo.fileName();

        QString postFix("");
        if (fileName.endsWith("xhtml")) {
            postFix.append(".xhtml");
        } else {
            if (fileName.endsWith("html")) {
                postFix.append(".html");
            } else {
                continue;
            }
        }
        //qDebug() << fileName;

        QRegExp rx("\\d+");
        pos = rx.indexIn(fileName, 0);
        if (-1 == pos)
            continue;
        int index = rx.cap(0).toInt();
        QString strIndex;
        if (fileCount >= 100) {
            strIndex = strIndex.sprintf("%03d", index);
        } else {
            strIndex = strIndex.sprintf("%02d", index);
        }
        //qDebug() << strIndex;

        // 3.1 build file path
        QString newFileName = filePrefix + strIndex + postFix;
        QString newFilePath = mDirName + QDir::separator() + newFileName;
        //qDebug() << newFilePath;

        // 3.2 rename file
        bool res = QFile::rename(filePath, newFilePath);
        qDebug() << res;
    }

    dir.refresh();
}
