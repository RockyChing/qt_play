#include <QSettings>
#include <QDebug>
#include <QIntValidator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QPalette>
#include <QRegExp>
#include <QRadioButton>
#include <QButtonGroup>
#include "filerenamedlg.h"
#include "ui_filerenamedlg.h"
#include "config/appsettings.h"
#include "utils/msgboxutil.h"
#include "utils/fileutil.h"

FileRenameDlg::FileRenameDlg(QWidget *parent) :
    QDialog(parent), mFileIndex(-1),
    ui(new Ui::FileRenameDlg)
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

FileRenameDlg::~FileRenameDlg()
{
    delete ui;
}

void FileRenameDlg::initUI()
{
    // QRadioButton setExclusive
    QButtonGroup *btnBox = new QButtonGroup;
    btnBox->setExclusive(true);
    btnBox->addButton(ui->rBtnPostfix);
    btnBox->addButton(ui->rBtnNamePart);
    btnBox->addButton(ui->rBtnIndexAuto);
    ui->rBtnPostfix->setChecked(true);

    connect(ui->lineEditSeq, SIGNAL(textChanged(QString)), this, SLOT(onSeqTextChanged()));
    //connect(ui->checkPartReplace, SIGNAL(stateChanged(int)), this, SLOT(onCBReplaceChanged(int)));
    connect(ui->btnDirOpen, SIGNAL(clicked()), this, SLOT(onBtnDirOpenClicked()));
    connect(ui->btnRename, SIGNAL(clicked()), this, SLOT(onBtnRenameClicked()));
}

void FileRenameDlg::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mDirName = s.value(AppSettings::FILE_RDIR).toString();
    if (!mDirName.isEmpty()) {
        ui->lineEditDirShow->setText(mDirName);
    } else {
        ui->lineEditDirShow->setText(tr("未指定目录"));
    }
}

bool FileRenameDlg::isStringUpper(QString &s)
{
    const QString upper("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    bool res = false;
    for (int i = 0; i < s.size(); i ++) {
        QString tmp(s.at(i));
        if (upper.contains(tmp)) {
            res = true;
            break;
        }
    }

    return res;
}

void FileRenameDlg::onSeqTextChanged()
{
    int index = ui->lineEditSeq->text().toInt();
    if (index < 0) {
        MsgBoxUtil::warning(this, tr("输入序号错误！"));
    } else {
        mFileIndex = index;
    }
}

void FileRenameDlg::onCBReplaceChanged(int i)
{
    ui->lineEditFilePrefix->clear();
}

void FileRenameDlg::onBtnDirOpenClicked()
{
    QString filePath = ui->lineEditDirShow->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mDirName = QFileDialog::getExistingDirectory(this, tr("Open file"), filePath);
    mDirName = QDir::toNativeSeparators(mDirName);
    qDebug() << mDirName;
    if (mDirName.isEmpty()) {
        ui->lineEditDirShow->setText(tr("未指定目录"));
    } else {
        ui->lineEditDirShow->setText(mDirName);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::FILE_RDIR, mDirName);
    }
}

void FileRenameDlg::onBtnRenameClicked()
{
    // 1.check parameter
    if (mDirName.isEmpty()) {
        MsgBoxUtil::warning(this, tr("未指定目录！"));
        return;
    }

    QDir dir(mDirName);
    if (!dir.exists()) {
        MsgBoxUtil::warning(this, tr("指定目录不存在！"));
        return;
    }

    if (ui->rBtnNamePart->isChecked()) {
        mNewNamePart = ui->lineEditAfter->text();
        mSrcNamePart = ui->lineEditBefore->text();
        if (mSrcNamePart.isEmpty()) {
            MsgBoxUtil::warning(this, tr("未输入待替字段！"));
            return;
        }
    }

    if (ui->rBtnIndexAuto->isChecked()) {
        if (ui->lineEditSeq->text().isEmpty()) {
            MsgBoxUtil::warning(this, tr("未输入序号！"));
            return;
        }

        mFileIndex = ui->lineEditSeq->text().toInt();
    }

    // 2.traverse files
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot
            | QDir::Files | QDir::Readable, QDir::Name);
    int fileCount = fileInfoList.size();
    if (fileCount <= 0) {
        qWarning("目录下文件空");
        return;
    }

    if (ui->checkBoxBackup->isChecked()) {
        bool res = false;
        res = FileUtil::copyDir(mDirName, mDirName + "_bak", false);
        qDebug() << "copyDir result: " << res;
    }

    // 3.rename files
    for (int i = 0; i < fileCount; i ++) {
        QFileInfo fileInfo = fileInfoList.at(i);
        QString filePath = fileInfo.absoluteFilePath();
        qDebug() << "srcFilePath: " << filePath;
        QString baseName = fileInfo.baseName();
        QString postfix = fileInfo.suffix();
        QString absolutePath = fileInfo.absolutePath();
        QString newFilePath = absolutePath + "/";
        if (ui->rBtnPostfix->isChecked()) {
            QString newPostfix = ui->lineEditNewPostfix->text();
            if (newPostfix.isEmpty()) {
                MsgBoxUtil::warning(this, tr("未输入新后缀！"));
                return;
            }

            newFilePath.append(baseName);
            newFilePath.append(".");
            newFilePath.append(newPostfix);
        } else if (ui->rBtnNamePart->isChecked()) {
            baseName.replace(mSrcNamePart, mNewNamePart);
            if (baseName.isEmpty()) {
                continue;
            }

            newFilePath.append(baseName);
            newFilePath.append(".");
            newFilePath.append(postfix);
        } else if (ui->rBtnIndexAuto->isChecked()) {
            mFileIndex += 1;
            QString strIndex;
            if ((mFileIndex + fileCount - i) >= 100) {
                strIndex = strIndex.sprintf("%03d", mFileIndex);
            } else {
                strIndex = strIndex.sprintf("%02d", mFileIndex);
            }

            newFilePath.append(ui->lineEditFilePrefix->text());
            newFilePath.append(strIndex);
            newFilePath.append(".");
            newFilePath.append(postfix);
        } else {
            MsgBoxUtil::warning(this, tr("未指定操作！"));
            return;
        }

        qDebug() << "newFilePath: " << newFilePath;
        if (0 == filePath.compare(newFilePath)) {
            continue;
        }
        QFile::rename(filePath, newFilePath);
    }

    dir.refresh();
    MsgBoxUtil::information(this, tr("操作成功！"));
}
