#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

#include "log/log.h"
#include "nl_del.h"
#include "ui_nl_del.h"

static const char *LOG_TAG = "nl_del";

NewlineDelete::NewlineDelete(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewlineDelete)
{
    ui->setupUi(this);

    initUI();
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

                QTextStream in(&fileSrc);
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    if (line.isEmpty()) {
                        log_info("newline");
                        continue;
                    }
                    line.remove(QRegExp("\\s"));
                    out << line;

                    int lineNum = mSpinBixLineNum->value();
                    if (1 == lineNum)
                        out << "\r\n";
                    else if (2 == lineNum)
                        out << "\r\n\r\n";
                    else if (3 == lineNum)
                        out << "\r\n\r\n\r\n";
                    else {}
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
