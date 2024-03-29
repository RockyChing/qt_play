#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>

#include <QDebug>

#include "config/appsettings.h"
#include "nl_del.h"
#include "ui_nl_del.h"

//static const char *LOG_TAG = "nl_del";

NewlineDelete::NewlineDelete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewlineDelete)
{
    ui->setupUi(this);

    initUI();
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mFileAbsPath = s.value(AppSettings::NL_PATH).toString();
    if (!mFileAbsPath.isEmpty()) {
        mFilePath->setText(mFileAbsPath);
    }
}

NewlineDelete::~NewlineDelete()
{
    delete ui;
}

void NewlineDelete::initUI()
{
    mBtnOpen = ui->btnNlDelOpen;
    mBtnProcess = ui->btnNlDelProcess;

    mFilePath = ui->lineEditFilePath;
    mSpinBixLineNum = ui->spinBoxLineNum;

    connect(mBtnOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(mBtnProcess, SIGNAL(clicked()), this, SLOT(onBtnProcessClicked()));
}

void NewlineDelete::onBtnOpenClicked()
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
        s.setValue(AppSettings::NL_PATH, mFileAbsPath);
    }
}

void NewlineDelete::onBtnProcessClicked()
{
    if (mFileAbsPath.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No file selected!!!"));
    } else {
        QFile fileSrc(mFileAbsPath);
        if (fileSrc.exists()) {
            bool res = fileSrc.open(QIODevice::ReadOnly);
            if (res) {
                QFile fileDst("tmp.txt");
                if (!fileDst.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                    fileSrc.close();
                    return;
                }
                QTextStream out(&fileDst);

                const int lineNum = mSpinBixLineNum->value();
                QTextStream in(&fileSrc);
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    if (line.isEmpty()) {
                        continue;
                    }
                    line.remove(QRegExp("\\s"));
                    out << line;

                    if (1 == lineNum)
                        out << "\r\n";
                    else if (2 == lineNum)
                        out << "\r\n\r\n";
                    else if (3 == lineNum)
                        out << "\r\n\r\n\r\n";
                    else {}

#if 0
                    if (line.contains(QRegExp("[a-zA-Z]"))) {
                        qDebug() << line;
                        break;
                    }
#endif
                }

                out.flush();
                fileDst.close();
                fileSrc.close();
                QMessageBox::information(this, "Info", tr("Work done!"));
            } else {
                 QMessageBox::warning(this, tr("Warning"), tr("fileSrc.open failed!!!"));
            }
        } else {
            QMessageBox::warning(this, tr("Warning"), tr("No file exists!!!"));
        }
    }
}
