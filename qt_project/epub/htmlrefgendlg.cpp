#include <QString>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QDebug>
#include <QDateTime>
#include <QClipboard>
#include <QMessageBox>

#include "htmlrefgendlg.h"
#include "ui_htmlrefgendlg.h"


QString HtmlRefGenDlg::mStyleDefaultString = "①②③④⑤⑥⑦⑧⑨⑩⑪⑫⑬⑭⑮⑯⑰⑱⑲⑳㉑㉒㉓㉔㉕㉖㉗㉘㉙㉚㉛㉜㉝㉞㉟㊱㊲㊳㊴㊵㊶㊷㊸㊹㊺㊻㊼㊽㊾㊿";

HtmlRefGenDlg::HtmlRefGenDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HtmlRefGenDlg),
    mIndexStyle(0), mItemCnt(0)
{
    ui->setupUi(this);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);

    initUI();
    initSlots();
    initData();
}

HtmlRefGenDlg::~HtmlRefGenDlg()
{
    delete ui;
}

void HtmlRefGenDlg::initUI()
{

}

void HtmlRefGenDlg::initSlots()
{
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onRefStyleChanged(int)));
    connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(onBtnOKClicked()));
    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(onBtnCancleClicked()));
}

void HtmlRefGenDlg::initData()
{
    mItemCurCnt = ui->editCurCnt->text().toInt();
}

void HtmlRefGenDlg::onRefStyleChanged(int index)
{
    mIndexStyle = index;
    qDebug() << "style: " << mIndexStyle;
}

void HtmlRefGenDlg::onBtnOKClicked()
{
    if (!ui->checkBoxSingle->isChecked()) {
        mItemCnt = ui->editItemCnt->text().toInt();
        if (mItemCnt <= 0 || (mIndexStyle != 0 && mIndexStyle != 1)) {
            QMessageBox::critical(NULL, QObject::tr("错误"), tr("请输入数量"));
            return;
        }

        mIndexStyle = ui->comboBox->currentIndex();
        if (mItemCnt > 50 && mIndexStyle == 0) {
            mIndexStyle = 1;
        }

        ui->textRef->clear();
        for (int i = 1; i <= mItemCnt; i ++) {
            QString refID("");

            if (mItemCnt >= 100) {
                refID.sprintf("zhu%03d", i);
            } else {
                refID.sprintf("zhu%02d", i);
            }

            QString tmp("");
            // <span><a id="zhu01_a" href="#zhu01_b"><sup>①</sup></a></span>
            tmp.append("<span><a id=\"");
            tmp.append(refID);
            tmp.append("_a\" href=\"#");
            tmp.append(refID);
            tmp.append("_b\"><sup>");

            QString style("");
            if (0 == mIndexStyle) {
                style.append(mStyleDefaultString.at(i-1));
            } else {
                style.sprintf("[%d]", i);
            }
            tmp.append(style);
            tmp.append("</sup></a></span>");
            ui->textRef->appendPlainText(tmp);
        }

        ui->textRef->appendPlainText("");
        for (int i = 1; i <= mItemCnt; i ++) {
            QString refID("");

            if (mItemCnt >= 100) {
                refID.sprintf("zhu%03d", i);
            } else {
                refID.sprintf("zhu%02d", i);
            }

            QString tmp("");
            //<p><a id="zhu01_b" href="#zhu01_a">①</a></p>
            tmp.clear();
            tmp.append("<p><a id=\"");
            tmp.append(refID);
            tmp.append("_b\" href=\"#");
            tmp.append(refID);
            tmp.append("_a\">");

            QString style("");
            if (0 == mIndexStyle) {
                style.append(mStyleDefaultString.at(i-1));
            } else {
                style.sprintf("[%d]", i);
            }
            tmp.append(style);
            tmp.append("</a>&nbsp;</p>");
            ui->textRef->appendPlainText(tmp);
        }
    } else {
        ui->textRef->clear();
        mItemCurCnt = ui->editCurCnt->text().toInt();
        QString refID("");
        refID.sprintf("zhu%02d", mItemCurCnt);

        QString tmp("");
        tmp.append("<span><a id=\"");
        tmp.append(refID);
        tmp.append("_a\" href=\"#");
        tmp.append(refID);
        tmp.append("_b\"><sup>");

        QString style("");
        if (0 == mIndexStyle) {
            style.append(mStyleDefaultString.at(mItemCurCnt-1));
        } else {
            style.sprintf("[%d]", mItemCurCnt);
        }
        tmp.append(style);
        tmp.append("</sup></a></span>");
        ui->textRef->appendPlainText(tmp);
        mItemCurCnt += 1;
        ui->editCurCnt->setText(QString::number(mItemCurCnt));
    }
    ui->textRef->selectAll();
    ui->textRef->copy();
}

void HtmlRefGenDlg::onBtnCancleClicked()
{
    this->close();
}
