#include <QSettings>
#include <QDebug>
#include <QIntValidator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include <QPalette>
#include "filenewdlg.h"
#include "ui_filenewdlg.h"
#include "config/appsettings.h"
#include "utils/msgboxutil.h"


FileNewDlg::FileNewDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileNewDlg)
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

FileNewDlg::~FileNewDlg()
{
    delete ui;
}

void FileNewDlg::initUI()
{
    mBtnDirOpen = ui->btnDirOpen;
    mBtnClear = ui->btnClear;
    mBtnCreate = ui->btnCreate;

    mEditDirShow = ui->lineEditDirShow;
    mEditSrcFile = ui->lineEditSrcFile;
    mEditFilePrefix = ui->lineEditFilePrefix;
    mEditStart = ui->lineEditStart;
    mEditTotal = ui->lineEditTotal;

    QIntValidator *v1 = new QIntValidator(0, 9999, this);
    mEditStart->setValidator(v1);

    QIntValidator *v2 = new QIntValidator(0, 9999, this);
    mEditTotal->setValidator(v2);

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::blue);
    ui->label->setPalette(palette);
    ui->label->setWordWrap(true);
    ui->label->setAlignment(Qt::AlignCenter);

    connect(mBtnDirOpen, SIGNAL(clicked()), this, SLOT(onBtnDirOpenClicked()));
    connect(mBtnClear, SIGNAL(clicked()), this, SLOT(onBtnClearClicked()));
    connect(mBtnCreate, SIGNAL(clicked()), this, SLOT(onBtnCreateClicked()));
}

void FileNewDlg::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mDirName = s.value(AppSettings::FILE_CDIR).toString();
    if (!mDirName.isEmpty()) {
        mEditDirShow->setText(mDirName);
    } else {
        mEditDirShow->setText(tr("未指定目录"));
    }

    mEditSrcFile->setText("demo.xhtml");
}

void FileNewDlg::onBtnDirOpenClicked()
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
        s.setValue(AppSettings::FILE_CDIR, mDirName);
    }
}

void FileNewDlg::onBtnClearClicked()
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

    QString demoFileName = mEditSrcFile->text();
    qDebug() << "demoFileName: " << demoFileName;

    // 2.traverse files
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::Readable, QDir::Name);
    if (fileInfoList.size() <= 0) {
        qWarning("目录下文件空");
        return;
    }

    // 3.delete non-demo files
    for (int i = 0; i < fileInfoList.size(); i ++) {
        QFileInfo fileInfo = fileInfoList.at(i);
        QString filePath = fileInfo.absoluteFilePath();
        if (filePath.contains(demoFileName)) {
            qWarning("found");
            continue;
        }

        QFile file(filePath);
        if (file.exists()) {
            file.remove();
        }
    }

    dir.refresh();
    MsgBoxUtil::information(this, tr("清除成功！"));
}

void FileNewDlg::onBtnCreateClicked()
{
    QString demoFileName = mEditSrcFile->text();
    QString demoFilePath = mDirName + QDir::separator() + demoFileName;
    qDebug() << "demoFilePath: " << demoFilePath;

    // 1.check demo file
    QFile file(demoFilePath);
    if (!file.exists()) {
        MsgBoxUtil::warning(this, tr("demo文件不存在！"));
        return;
    }

    // 2.check parameter
    QString strTotal = mEditTotal->text();
    if (strTotal.isEmpty()) {
        MsgBoxUtil::warning(this, tr("请输入生成文件个数！"));
        return;
    }

    // 3.data init
    bool ok;
    uint total = strTotal.toUInt(&ok, 10);
    if (!ok || 0 == total) {
        MsgBoxUtil::warning(this, tr("生成文件个数数据非法！"));
        return;
    }

    if (total >= 1000) {
        MsgBoxUtil::warning(this, tr("生成文件个数上限999！"));
        return;
    }

    uint start;
    QString strStart = mEditStart->text();
    if (strStart.isEmpty()) {
        start = 1;
    } else {
        start = strStart.toUInt(&ok, 10);
        if (!ok || start < 1) {
            MsgBoxUtil::warning(this, tr("起始编号数据非法！"));
            return;
        }
    }

    QString strFilePrefix = mEditFilePrefix->text();
    QFileInfo fi(demoFilePath);
    QString strFilePostfix = fi.completeSuffix();
    if (strFilePostfix.isEmpty()) {
        MsgBoxUtil::warning(this, tr("demo文件后缀异常！"));
        return;
    }

    // 4.copy file
    for (uint i = start; i < (total + start); i ++) {
        // 4.1 create file name
        QString fileName = strFilePrefix;
        if (i < 10) {
            fileName.append("0");
        }

        if (total >= 100 && total < 1000) {
            if (i < 100) {
                fileName.append("0");
            }
        }

        QString index = QString::number(i);
        fileName.append(index);
        fileName.append(".");
        fileName.append(strFilePostfix);

        QString filePath = mDirName + QDir::separator() + fileName;
        qDebug() << filePath;
        QFile::copy(demoFilePath, filePath);
    }

    MsgBoxUtil::information(this, tr("创建成功！"));
}
