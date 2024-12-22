#include <QSettings>
#include <QDebug>
#include <QIntValidator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QPalette>
#include <QRegExp>
#include <QStringList>

#include "filepickdlg.h"
#include "ui_filepickdlg.h"
#include "config/appsettings.h"
#include "utils/fileutil.h"
#include "utils/msgboxutil.h"
#include "utils/toast.h"


FilePickDlg::FilePickDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilePickDlg)
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

FilePickDlg::~FilePickDlg()
{
    delete ui;
}

void FilePickDlg::initUI()
{
    connect(ui->btnDirList, SIGNAL(clicked()), this, SLOT(onBtnDirListClicked()));
    connect(ui->btnDirSrc, SIGNAL(clicked()), this, SLOT(onBtnDirSrcClicked()));
    connect(ui->btnDirDst, SIGNAL(clicked()), this, SLOT(onBtnDirDstClicked()));
    connect(ui->btnPick, SIGNAL(clicked()), this, SLOT(onBtnPickClicked()));
}

void FilePickDlg::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);

    mDirListName = s.value(AppSettings::FAV_LIST_PATH).toString();
    if (!mDirListName.isEmpty()) {
        ui->lineEditDirList->setText(mDirListName);
    } else {
        ui->lineEditDirList->setText(tr("未指定目录"));
    }

    mDirSrcName = s.value(AppSettings::FAV_SDIR).toString();
    if (!mDirSrcName.isEmpty()) {
        ui->lineEditDirSrc->setText(mDirSrcName);
    } else {
        ui->lineEditDirSrc->setText(tr("未指定目录"));
    }

    mDirDstName = s.value(AppSettings::FAV_DDIR).toString();
    if (!mDirDstName.isEmpty()) {
        ui->lineEditDirDst->setText(mDirDstName);
    } else {
        ui->lineEditDirDst->setText(tr("未指定目录"));
    }
}

bool FilePickDlg::isCfgAvailable()
{
    QString path = ui->lineEditDirList->text();
    if (path.isEmpty()) {
        MsgBoxUtil::warning(this, tr("列表文件未配置！"));
        return false;
    }

    QFile file(path);
    if (!file.exists()) {
        MsgBoxUtil::warning(this, tr("列表文件不存在！"));
        return false;
    }

    path = ui->lineEditDirSrc->text();
    if (path.isEmpty()) {
        MsgBoxUtil::warning(this, tr("源目录未配置！"));
        return false;
    }

    QDir dir(path);
    if (!dir.exists()) {
        MsgBoxUtil::warning(this, tr("源目录不存在！"));
        return false;
    }

    path = ui->lineEditDirSrc->text();
    if (path.isEmpty()) {
        MsgBoxUtil::warning(this, tr("源目录未配置！"));
        return false;
    }

    return true;
}

void FilePickDlg::onBtnDirListClicked()
{
    QString filePath = ui->lineEditDirList->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mDirListName = QFileDialog::getOpenFileName(this, tr("Open file"), filePath);
    if (mDirListName.isEmpty()) {
        ui->lineEditDirList->setText("no file!");
    } else {
        ui->lineEditDirList->setText(mDirListName);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::FAV_LIST_PATH, mDirListName);
    }
}

void FilePickDlg::onBtnDirSrcClicked()
{
    QString filePath = ui->lineEditDirSrc->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mDirSrcName = QFileDialog::getExistingDirectory(this, tr("Select Directory"), filePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //mFileAbsPath = QDir::toNativeSeparators(mFileAbsPath);
    if (mDirSrcName.isEmpty()) {
        ui->lineEditDirSrc->setText("no file!");
    } else {
        ui->lineEditDirSrc->setText(mDirSrcName);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::FAV_SDIR, mDirSrcName);
    }
}

void FilePickDlg::onBtnDirDstClicked()
{
    QString filePath = ui->lineEditDirDst->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mDirDstName = QFileDialog::getExistingDirectory(this, tr("Select Directory"), filePath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //mFileAbsPath = QDir::toNativeSeparators(mFileAbsPath);
    if (mDirDstName.isEmpty()) {
        ui->lineEditDirDst->setText("no file!");
    } else {
        ui->lineEditDirDst->setText(mDirDstName);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::FAV_DDIR, mDirDstName);
    }
}

void FilePickDlg::onBtnPickClicked()
{
    bool res = false;

    // check config
    QString favListPath = ui->lineEditDirList->text();
    if (favListPath.isEmpty()) {
        MsgBoxUtil::warning(this, tr("列表文件未配置！"));
        return;
    }

    QFile favListFile(favListPath);
    if (!favListFile.exists()) {
        MsgBoxUtil::warning(this, tr("列表文件不存在！"));
        return;
    }

    QString srcDirStr = ui->lineEditDirSrc->text();
    if (srcDirStr.isEmpty()) {
        MsgBoxUtil::warning(this, tr("源目录未配置！"));
        return;
    }

    QDir srcDir(srcDirStr);
    if (!srcDir.exists()) {
        MsgBoxUtil::warning(this, tr("源目录不存在！"));
        return;
    }

    QString dstDirStr = ui->lineEditDirDst->text();
    if (dstDirStr.isEmpty()) {
        MsgBoxUtil::warning(this, tr("目标目录未配置！"));
        return;
    }

    QDir dstDir(dstDirStr);
    if (!dstDir.exists()) {
        MsgBoxUtil::warning(this, tr("目标目录不存在！"));
        return;
    }

    Toast::display("开始复制...");
    ui->info->clear();
    ui->info->append("开始复制...");
    ui->btnPick->setEnabled(false);

    // get src files
    QList<QString> srcFiles;
    res = FileUtil::fileRecursive(srcDirStr, srcFiles);
    if (!res) {
        ui->btnPick->setEnabled(true);
        MsgBoxUtil::warning(this, tr("获取原文件失败！"));
        return;
    }
    qDebug() << "srcFiles count: " << srcFiles.size();
#if 0
    QStringList dirUnderSrcDir = srcDir.entryList(QDir::NoSymLinks | QDir::AllDirs  | QDir::Dirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < dirUnderSrcDir.size(); i ++) {
        QString newPath = dstDirStr + "/" + dirUnderSrcDir.at(i);
        //qDebug() << "mkdir: " << newPath;

        QDir dir(newPath);
        if (!dir.exists()) {
            dir.mkdir(newPath);
            if (!dir.exists()) {
                ui->btnPick->setEnabled(true);
                return;
            }
        }
    }
#endif
    // parse favorites.txt
    if (!favListFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->btnPick->setEnabled(true);
        MsgBoxUtil::warning(this, tr("列表文件open failed！"));
        return;
    }

    // get favorite items
    QTextStream in(&favListFile);
    in.setCodec("utf-8");
    QString failMsg("");
    int favCnt = 0, copyCnt = 0;
    while (!in.atEnd()) {
        bool found = false;
        QString line = in.readLine();
        //qDebug() << line;
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }
        favCnt += 1;

        for (int i = 0; i < srcFiles.size(); i ++) {
            QString tmp = srcFiles.at(i);
            QFileInfo srcFilInfo(tmp);
            qDebug() << "srcFile name: " << srcFilInfo.fileName();
            qDebug() << "find name: " << line;

            if (0 == QString::compare(srcFilInfo.fileName(), line)) { // found
                QString copyFilePath = tmp.replace(srcDirStr, dstDirStr);
                //qDebug() << "copyFilePath: " << copyFilePath;

                QFileInfo copyFilInfo(copyFilePath);
                QDir dir(copyFilInfo.absolutePath());
                if (!dir.exists()) {
                    dir.mkpath(copyFilInfo.absolutePath());
                    if (!dir.exists()) {
                        failMsg.append(copyFilInfo.absolutePath());
                        failMsg.append(" not exist!");
                        failMsg.append("\n");
                        break;
                    }
                }

                bool copyRes = QFile::copy(srcFiles.at(i), copyFilePath);
                if (copyRes) {
                    copyCnt += 1;
                    srcFiles.removeAt(i);
                } else {
                    failMsg.append(line);
                    failMsg.append(" copy failed!");
                    failMsg.append("\n");
                }

                found = true;
                break;
            }
        }

        if (!found) {
            QString tmp = line + " not found!";
            failMsg.append(tmp);
            failMsg.append("\n");
        }
    }

    qDebug() << "favCnt: " << favCnt;
    qDebug() << "copyCnt: " << copyCnt;
    favListFile.close();
    ui->btnPick->setEnabled(true);
    ui->info->append("复制完成");
    ui->info->append(failMsg);
}
