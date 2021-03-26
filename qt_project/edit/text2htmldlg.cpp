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
    mBtnCopy = ui->btnCopy;

    connect(mBtnTransform, SIGNAL(clicked()), this, SLOT(onBtnTransformClicked()));
    connect(mBtnCopy, SIGNAL(clicked()), this, SLOT(onBtnCopyClicked()));
}

void Text2HtmlDlg::onBtnTransformClicked()
{
    QString s = mTextHtmlEdit->toPlainText();
    QTextStream in(&s);

    QString result;
    result.append("<p>\r\n");

    const int lineNum = ui->spinBoxNlineNum->value();
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) {
            continue;
        }
        line.remove(QRegExp("\\s"));

        if (1 == lineNum)
            line.append("<br/>\r\n");
        else if (2 == lineNum)
            line.append("<br/><br/>\r\n\r\n");
        else {}
        result.append(line);
    }

    result.append("&#160;</p>");
    mTextHtmlEdit->clear();
    mTextHtmlEdit->setPlainText(result);
}

void Text2HtmlDlg::onBtnCopyClicked()
{
    mTextHtmlEdit->selectAll();
    mTextHtmlEdit->copy();
}
