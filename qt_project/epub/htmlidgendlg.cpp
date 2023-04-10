#include "htmlidgendlg.h"
#include "ui_HtmlIdGenDlg.h"

#include <QString>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QDebug>
#include <QDateTime>
#include <QClipboard>


HtmlIdGenDlg::HtmlIdGenDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HtmlIdGenDlg)
{
    ui->setupUi(this);
    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;
    setWindowFlags(windowFlag);

    initUI();
    initSlots();
}

HtmlIdGenDlg::~HtmlIdGenDlg()
{
    delete ui;
}

void HtmlIdGenDlg::initUI()
{
    mResText = ui->plainTextEdit;
    mResText->setReadOnly(true);
    ui->lenLineEdit->setText(ui->lenLineEdit->placeholderText());
    ui->postLineEdit->setText(ui->postLineEdit->placeholderText());
}

void HtmlIdGenDlg::initSlots()
{
    connect(ui->btnOK, SIGNAL(clicked()), this, SLOT(onBtnOKClicked()));
    connect(ui->btnCancle, SIGNAL(clicked()), this, SLOT(onBtnCancleClicked()));
}

QString HtmlIdGenDlg::getRandomSerials(int length)
{
    const char chars[] = "1234567890abcdefghigklmnopqrstuvwxyzABCDEFGHIGKLMNOPQRSTUVWXYZ";
    const int size = sizeof(chars);

    char array[length + 1] = { 0, };
    qsrand(QDateTime::currentMSecsSinceEpoch());
    int index;
    for (int i = 0; i < length; i ++) {
        index = rand() % (size - 1);
        array[i] = chars[index];
    }

    QString result(array);
    return result;
}

void HtmlIdGenDlg::onBtnOKClicked()
{
    QString preText = ui->prexLineEdit->text();
    QString postText = ui->postLineEdit->text();
    // preText: hlm001 -> chapText: chap001
    QString chapText("chap");

    int length = ui->lenLineEdit->text().toInt();
    qDebug() << length;
    QString random = getRandomSerials(length);
    QString randomSerial("");
    randomSerial.append(preText);

    if (!preText.isEmpty()) {
        randomSerial.append("_");
        for (int i = 0; i < preText.size(); i ++) {
            QChar c = preText.at(i);
            if (c.isDigit()) {
                chapText.append(c);
            }
        }
        qDebug() << "chapText: " << chapText;
    }
    randomSerial.append(random);

    mResText->clear();

    // fmt: <a id="Bzc_4SXx9a" href="zici.xhtml#Bzc_4SXx9">text</a>
    mResText->appendPlainText("<span>");
    mResText->insertPlainText("<a id=\"");
    mResText->insertPlainText(randomSerial);
    mResText->insertPlainText("a\" href=\"zici.");
    mResText->insertPlainText(postText);
    mResText->insertPlainText("#");
    mResText->insertPlainText(randomSerial);
    mResText->insertPlainText("\">");
    mResText->insertPlainText(ui->ctxLineEdit->text());
    mResText->insertPlainText("</a>");
    mResText->insertPlainText("</span>");

    mResText->insertPlainText("\n");
    // fmt: <p><a id="jpm002_Ev9rO" class="zici_zhu" href="chap002.html#jpm002_Ev9rOa"><b></b></a>：</p>
    mResText->appendPlainText("<p>");
    mResText->insertPlainText("<a id=\"");
    mResText->insertPlainText(randomSerial);
    mResText->insertPlainText("\" class=\"zici_zhu\"");
    mResText->insertPlainText(" href=\"");
    mResText->insertPlainText(chapText);
    mResText->insertPlainText(".");
    mResText->insertPlainText(postText);
    mResText->insertPlainText("#");
    mResText->insertPlainText(randomSerial);
    mResText->insertPlainText("a\"><b>");
    mResText->insertPlainText(ui->ctxLineEdit->text());
    mResText->insertPlainText("</b></a>：");
    mResText->insertPlainText("</p>");

    mResText->selectAll();
    mResText->copy();
}

void HtmlIdGenDlg::onBtnCancleClicked()
{
    this->close();
}
