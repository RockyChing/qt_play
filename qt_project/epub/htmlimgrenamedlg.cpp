#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>

#include <QDebug>

#include "config/appsettings.h"
#include "htmlimgrenamedlg.h"
#include "ui_htmlimgrenamedlg.h"

HtmlImgRenameDlg::HtmlImgRenameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HtmlImgRenameDlg)
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

HtmlImgRenameDlg::~HtmlImgRenameDlg()
{
    delete ui;
}

void HtmlImgRenameDlg::initUI()
{
    mBtnOpen = ui->btnOpen;
    mBtnProcess = ui->btnProcess;

    mFilePath = ui->lineEditFilePath;

    connect(mBtnOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(mBtnProcess, SIGNAL(clicked()), this, SLOT(onBtnProcessClicked()));
}

void HtmlImgRenameDlg::initData()
{
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mFileAbsPath = s.value(AppSettings::HTML_PATH).toString();
    if (!mFileAbsPath.isEmpty()) {
        mFilePath->setText(mFileAbsPath);
    }
}

void HtmlImgRenameDlg::onBtnOpenClicked()
{
    QString filePath = mFilePath->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mFileAbsPath = QFileDialog::getOpenFileName(this, tr("Open file"), filePath);
    mFileAbsPath = QDir::toNativeSeparators(mFileAbsPath);
    if (mFileAbsPath.isEmpty()) {
        mFilePath->setText("no file!");
    } else {
        mFilePath->setText(mFileAbsPath);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::HTML_PATH, mFileAbsPath);
    }
}

void HtmlImgRenameDlg::onBtnProcessClicked()
{
    do {
        QFile htmlFile(mFileAbsPath);
        if (!htmlFile.exists()) {
            QMessageBox::warning(this, tr("Warning"), tr("No file!!!"));
            break;
        }

        QFileInfo htmlFileInfo(htmlFile);
        QString base = htmlFileInfo.absolutePath();
        qDebug() << "base: " << base;

        if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Warning"), tr("Open file!!!"));
            break;
        }

        QTextStream in(&htmlFile);
        int lineCnt = 0;
        while (!in.atEnd()) {
            QString line = in.readLine();
            //qDebug() << line;

            /* line format:
             * <p id="1" class="3"><img src="images/00717.jpeg" alt="00526.jpg"/></p>
             * */
            int n = line.indexOf("images");
            QString tmp = line.mid(n);

            int m = tmp.indexOf("\"");
            QString tmp2 = tmp.mid(0, m);
            //qDebug() << tmp2;

            QString srcImgName = base + QDir::separator() + tmp2;
            QFileInfo srcImgFileInfo(srcImgName);
            if (!srcImgFileInfo.exists()) {
                QMessageBox::warning(this, tr("Warning"), tr("no src img!!!"));
                break;
            }
            qDebug() << "srcImgName: " << srcImgName;

            if (srcImgFileInfo.size() < 500) {
                QFile::remove(srcImgName);
                continue;
            }
            QString suffix = srcImgFileInfo.suffix();
            qDebug() << "suffix: " << suffix;

            QString ddirName = base + QDir::separator() + "dst";
            //qDebug() << ddirName;
            QDir ddir(ddirName);
            if (!ddir.exists()) {
                ddir.mkdir(ddirName);
                if (!ddir.exists()) {
                    QMessageBox::warning(this, tr("Warning"), tr("mkdir!!!"));
                    break;
                }
            }

            lineCnt += 1;
            QString dstImgName("");
            dstImgName = QString::asprintf("%04d", lineCnt);
            //qDebug() << newImgName;

            QString dstImgPath = ddirName + QDir::separator() + dstImgName + "." + suffix;
            qDebug() << "dstImgPath: " << dstImgPath;

            bool res = false;
            res = QFile::rename(srcImgName, dstImgPath);
            if (!res) {
                QMessageBox::warning(this, tr("Warning"), tr("rename err!!!"));
                break;
            }
        }

        htmlFile.close();
        QMessageBox::information(this, "Info", tr("Work done!"));
    } while (0);
}
