#include "text2htmldlg.h"
#include "ui_text2htmldlg.h"
#include <QDebug>

Text2HtmlDlg::Text2HtmlDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Text2HtmlDlg)
{
    ui->setupUi(this);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;

    setWindowFlags(windowFlag);
    initUI();
}

Text2HtmlDlg::~Text2HtmlDlg()
{
    delete ui;
}

void Text2HtmlDlg::initUI()
{
    mTextHtmlEdit = ui->textHtmlEdit;
    mBtnTransform = ui->btnTransform;
    mBtnClear = ui->btnClear;

    ui->spinBoxNlineNum->setValue(1);

    connect(mBtnTransform, SIGNAL(clicked()), this, SLOT(onBtnTransformClicked()));
    connect(mBtnClear, SIGNAL(clicked()), this, SLOT(onBtnClearClicked()));
    //connect(mBtnCopy, SIGNAL(clicked()), this, SLOT(onBtnCopyClicked()));
}

void Text2HtmlDlg::onBtnTransformClicked()
{
    QString s = mTextHtmlEdit->toPlainText();
    QTextStream in(&s);

    QString result;
    result.clear();
    //result.append("<p class=\"indent-para\">");

    const int lineNum = ui->spinBoxNlineNum->value();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) {
            continue;
        }

        if (ui->cBoxDelSpace->isChecked()) {
            line.remove(QRegExp("\\s"));
        }

        result.append("<p>");
        result.append(line);
        result.append("</p>");

        if (ui->cBoxNewLine->isChecked()) {
            result.append("\r\n");
            result.append("<p><br/></p>");
        }

        if (1 == lineNum)
            result.append("\r\n");
        else if (2 == lineNum)
            result.append("\r\n\r\n");
        else {}
    }

    //result.append("</p>");
    mTextHtmlEdit->clear();
    if (1 == lineNum)
        mTextHtmlEdit->setPlainText(result.left(result.length()-2));
    else if (2 == lineNum)
        mTextHtmlEdit->setPlainText(result.left(result.length()-4));
    else {}

    onBtnCopyClicked();
}

void Text2HtmlDlg::onBtnClearClicked()
{
    mTextHtmlEdit->clear();
}

void Text2HtmlDlg::onBtnCopyClicked()
{
    mTextHtmlEdit->selectAll();
    mTextHtmlEdit->copy();
}
