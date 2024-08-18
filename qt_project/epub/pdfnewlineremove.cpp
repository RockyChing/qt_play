#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include <QStringList>
#include <QDialog>
#include <QLineEdit>
#include <QDebug>
#include <QRegExp>


#include "utils/msgboxutil.h"
#include "pdfnewlineremove.h"
#include "ui_pdfnewlineremove.h"

pdfnewlineremove::pdfnewlineremove(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pdfnewlineremove)
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

pdfnewlineremove::~pdfnewlineremove()
{
    delete ui;
}

void pdfnewlineremove::initUI()
{
    mBtnOpen = ui->btnOpen;
    mBtnProcess = ui->btnProcess;

    //mFilePath = ui->lineEditFilePath;

    connect(mBtnOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
    connect(mBtnProcess, SIGNAL(clicked()), this, SLOT(onBtnProcessClicked()));
}

void pdfnewlineremove::initData()
{
    mFileAbsPaths.clear();
}

void pdfnewlineremove::onBtnOpenClicked()
{
    QString filePath = QDir::currentPath();
    /* getOpenFileName() returns an existing file selected by the user */
    mFileAbsPaths = QFileDialog::getOpenFileNames(this, tr("Open file"), filePath);
    QFileInfo fInfo(mFileAbsPaths.at(0));
    ui->lineEditFilePath->setText(fInfo.absolutePath());
}

void pdfnewlineremove::onBtnProcessClicked()
{
    const QString newLine("\r\n");
    if (0 == mFileAbsPaths.size()) {
        MsgBoxUtil::warning(this, tr("请选择文件！"));
        return;
    }
    for (int i = 0; i < mFileAbsPaths.size(); i++) {
        QString filePath = mFileAbsPaths.at(i);
        qDebug() << "filePath: " << filePath;

        QFileInfo fInfo(filePath);
        QString fName = fInfo.baseName();
        qDebug() << "fName: " << fName;

        QString newFilePath(filePath);
        newFilePath.replace(fName, fName + "_new");
        qDebug() << "newFilePath: " << newFilePath;

        // create new file
        QFile newFile(newFilePath);
        if (!newFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            MsgBoxUtil::warning(this, tr("无法创建新文件！"));
            return;
        }

        QTextStream out(&newFile);
        /* format: utf-8 */
        out.setCodec("utf-8");

        // process
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            MsgBoxUtil::warning(this, tr("无法创建新文件！"));
            return;
        }

        QTextStream in(&file);
        in.setCodec("utf-8");
        bool isNewLine = true;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.isEmpty()) {
                continue;
            }
            // is page number
            QRegExp re("\\d*");
            if (re.exactMatch(line)) {
                qDebug() << "page: " << line;
                continue;
            }

            if (isNewLine && ui->checkBoxToHtml->isChecked()) {
                out << "<p>";
                isNewLine = false;
            }

            out << line;
            if (line.endsWith("。") || line.endsWith("？") || line.endsWith("！") ||
                    line.endsWith("」") || line.endsWith("”") ||
                    line.endsWith(".") || line.endsWith("?") ||
                    line.endsWith("!") || line.endsWith("\"")) {
                if (ui->checkBoxToHtml->isChecked()) {
                    out << "</p>";
                }
                out << newLine;
                isNewLine = true;
            } else if (line.startsWith("作者")) {
                if (ui->checkBoxToHtml->isChecked()) {
                    out << "</p>";
                }
                out << "\n";
                isNewLine = true;
            } else {

            }
        }
        file.close();
        newFile.close();
    }

    MsgBoxUtil::information(this, "转换完成！");
}
