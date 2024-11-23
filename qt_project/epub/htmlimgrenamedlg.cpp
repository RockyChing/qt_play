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

QString HtmlImgRenameDlg::getImageName(QString& line)
{
    /* line format:
     * <p class="calibre1"><a id="calibre_link-1"></a><img src="images/000076.jpg" alt="Image 1" class="calibre2" /></p>
     * */
    int idx0 = line.indexOf("<img");
    if (-1 == idx0) {
        return "";
    }

    QString imgElement = line.mid(idx0);
    //qDebug() << "imgElement: " << imgElement;
    int idx1 = imgElement.indexOf("images/");
    if (-1 == idx1) {
        return "";
    }

    QString tmp = imgElement.mid(idx1);
    //qDebug() << "tmp: " << tmp;
    int idx2 = tmp.indexOf("\"");
    if (-1 == idx2) {
        return "";
    }

    QString result = tmp.mid(7, idx2 - 7);
    return result;
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
    QFile htmlFile(mFileAbsPath);
    if (!htmlFile.exists()) {
        QMessageBox::warning(this, tr("Warning"), tr("No file!!!"));
        return;
    }

    if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Warning"), tr("Open file!!!"));
        return;
    }

    QFileInfo htmlFileInfo(htmlFile);
    QString base = htmlFileInfo.absolutePath();
    QString srcImgDir = base +"/images";
    QString dstImgDir = base +"/dst";
    qDebug() << "srcImgDir: " << srcImgDir;
    qDebug() << "dstImgDir: " << dstImgDir;

    QDir ddir(dstImgDir);
    if (!ddir.exists()) {
        ddir.mkdir(dstImgDir);
        if (!ddir.exists()) {
            QMessageBox::warning(this, tr("Warning"), tr("´´½¨dstÄ¿Â¼Ê§°Ü£¡"));
            return;
        }
    }

    QTextStream in(&htmlFile);
    int lineCnt = 1;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QString imageName = getImageName(line);
        if (imageName.isEmpty()) {
            continue;
        }

        QString srcImgPath = srcImgDir + "/" + imageName;
        qDebug() << "srcImgPath: " << srcImgPath;
        QFileInfo srcImgFileInfo(srcImgPath);
        if (!srcImgFileInfo.exists()) {
            qDebug() << srcImgPath << "not exist!";
            continue;
        }

        QString suffix = srcImgFileInfo.suffix();
        QString dstImgName("");
        dstImgName = QString::asprintf("%04d", lineCnt);
        QString dstImgPath = dstImgDir + "/" + dstImgName + "." + suffix;
        qDebug() << "dstImgPath: " << dstImgPath;

        bool res = false;
        res = QFile::rename(srcImgPath, dstImgPath);
        if (!res) {
            qDebug() << srcImgPath << "rename err!!!";
        }

        lineCnt += 1;
    }

    htmlFile.close();
    QMessageBox::information(this, "Info", tr("Work done!"));
}
