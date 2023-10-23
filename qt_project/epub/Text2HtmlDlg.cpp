#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QStringList>
#include "Text2HtmlDlg.h"
#include "ui_text2htmldlg.h"
#include "config/appsettings.h"


Text2HtmlDlg::Text2HtmlDlg(QWidget *parent) :
    QDialog(parent), mClassString(""),
    ui(new Ui::Text2HtmlDlg)
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

Text2HtmlDlg::~Text2HtmlDlg()
{
    delete ui;
}

void Text2HtmlDlg::initUI()
{
    mTextHtmlEdit = ui->textHtmlEdit;
    mBtnSrc = ui->btnSrc;
    mBtnTransform = ui->btnTransform;
    mBtnClear = ui->btnClear;

    connect(mBtnSrc, SIGNAL(clicked()), this, SLOT(onBtnSrcClicked()));
    connect(mBtnTransform, SIGNAL(clicked()), this, SLOT(onBtnTransformClicked()));
    connect(mBtnClear, SIGNAL(clicked()), this, SLOT(onBtnClearClicked()));
    //connect(mBtnCopy, SIGNAL(clicked()), this, SLOT(onBtnCopyClicked()));
    connect(ui->btnClass, SIGNAL(clicked()), this, SLOT(onBtnAddClsClicked()));
    connect(ui->cBoxClass, SIGNAL(currentIndexChanged(int)), this, SLOT(onClassIndexChanged(int)));
}

void Text2HtmlDlg::initData()
{
    ui->spinBoxNlineNum->setValue(1);

    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    QStringList listItems = s.value(AppSettings::HTML_CLS).toStringList();
    if (listItems.size() > 0) {
        ui->cBoxClass->clear();
        ui->cBoxClass->addItems(listItems);
    }
}

void Text2HtmlDlg::addClsItem(QString& items)
{
    if (!items.isEmpty()){
        QStringList listItems = items.split(",", QString::SkipEmptyParts);
        int itemCnt = listItems.size();
        for (int i = 0; i < itemCnt; i ++) {
            QString item = listItems.at(i);
            if (-1 == ui->cBoxClass->findText(item)) {
                ui->cBoxClass->addItem(item);
            }
        }

        updateCfg();
    }
}

void Text2HtmlDlg::updateCfg()
{
    int itemCnt = ui->cBoxClass->count();
    if (itemCnt > 0) {
        QStringList listItems;
        listItems.clear();
        for (int i = 0; i < itemCnt; i ++) {
            QString item = ui->cBoxClass->itemText(i);
            listItems.append(item);
            qDebug() << "item " << i << ": " << item;
        }
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::HTML_CLS, listItems);
    }
}

void Text2HtmlDlg::onBtnSrcClicked()
{
    mTextHtmlEdit->clear();

    QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasImage()) {
        qDebug() << "hasImage";
    } else if (mimeData->hasHtml()) {
        qDebug() << "hasHtml";
        QString htmlString = mimeData->html();
        mTextHtmlEdit->appendHtml(htmlString);
        mTextHtmlEdit->appendPlainText("*** html code:");
        mTextHtmlEdit->appendPlainText(htmlString);
    } else if (mimeData->hasText()) {
        qDebug() << "hasText";
    } else {
        qDebug() << "not support";
    }
}

void Text2HtmlDlg::onBtnTransformClicked()
{
    QString s = mTextHtmlEdit->toPlainText();
    QTextStream in(&s);

    QString result;
    result.clear();
    //result.append("<p class=\"indent-para\">");

    const int lineNum = ui->spinBoxNlineNum->value();
    bool isNumZero = false;
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) {
            continue;
        }

        if (ui->cBoxDelSpace->isChecked()) {
            line.remove(QRegExp("\\s"));
        }

        if (!isNumZero) {
            result.append("<p");
            if (mClassString.isEmpty()) {
                result.append(">");
            } else {
                result.append(" class=\"");
                result.append(mClassString);
                result.append("\">");
            }
        }

        if (ui->cBoxNote->isChecked()) {
            int pos = -1;
            int len = -1;
            QString tmp("");

            if ((pos = line.indexOf("①")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu01_a\" href=\"#zhu01_b\"><sup>①</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("②")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu02_a\" href=\"#zhu02_b\"><sup>②</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("③")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu03_a\" href=\"#zhu03_b\"><sup>③</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("④")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu04_a\" href=\"#zhu04_b\"><sup>④</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑤")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu05_a\" href=\"#zhu05_b\"><sup>⑤</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑥")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu06_a\" href=\"#zhu06_b\"><sup>⑥</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑦")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu07_a\" href=\"#zhu07_b\"><sup>⑦</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑧")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu08_a\" href=\"#zhu08_b\"><sup>⑧</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑨")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu09_a\" href=\"#zhu09_b\"><sup>⑨</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑩")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu10_a\" href=\"#zhu10_b\"><sup>⑩</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑪")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu11_a\" href=\"#zhu11_b\"><sup>⑪</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑫")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu12_a\" href=\"#zhu12_b\"><sup>⑫</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑬")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu13_a\" href=\"#zhu13_b\"><sup>⑬</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑭")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu14_a\" href=\"#zhu14_b\"><sup>⑭</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑮")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu15_a\" href=\"#zhu15_b\"><sup>⑮</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑯")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu16_a\" href=\"#zhu16_b\"><sup>⑯</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑰")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu17_a\" href=\"#zhu17_b\"><sup>⑰</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑱")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu18_a\" href=\"#zhu18_b\"><sup>⑱</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑲")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu19_a\" href=\"#zhu19_b\"><sup>⑲</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("⑳")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu20_a\" href=\"#zhu20_b\"><sup>⑳</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉑")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu21_a\" href=\"#zhu21_b\"><sup>㉑</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉒")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu22_a\" href=\"#zhu22_b\"><sup>㉒</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉓")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu23_a\" href=\"#zhu23_b\"><sup>㉓</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉔")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu24_a\" href=\"#zhu24_b\"><sup>㉔</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉕")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu25_a\" href=\"#zhu25_b\"><sup>㉕</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉖")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu26_a\" href=\"#zhu26_b\"><sup>㉖</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉗")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu27_a\" href=\"#zhu27_b\"><sup>㉗</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉘")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu28_a\" href=\"#zhu28_b\"><sup>㉘</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉙")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu29_a\" href=\"#zhu29_b\"><sup>㉙</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉚")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu30_a\" href=\"#zhu30_b\"><sup>㉚</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉛")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu31_a\" href=\"#zhu31_b\"><sup>㉛</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉜")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu32_a\" href=\"#zhu32_b\"><sup>㉜</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉝")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu33_a\" href=\"#zhu33_b\"><sup>㉝</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉞")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu34_a\" href=\"#zhu34_b\"><sup>㉞</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㉟")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu35_a\" href=\"#zhu35_b\"><sup>㉟</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊱")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu36_a\" href=\"#zhu36_b\"><sup>㊱</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊲")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu37_a\" href=\"#zhu37_b\"><sup>㊲</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊳")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu38_a\" href=\"#zhu38_b\"><sup>㊳</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊴")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu39_a\" href=\"#zhu39_b\"><sup>㊴</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊵")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu40_a\" href=\"#zhu40_b\"><sup>㊵</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊶")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu41_a\" href=\"#zhu41_b\"><sup>㊶</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊷")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu42_a\" href=\"#zhu42_b\"><sup>㊷</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊸")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu43_a\" href=\"#zhu43_b\"><sup>㊸</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊹")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu44_a\" href=\"#zhu44_b\"><sup>㊹</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊺")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu45_a\" href=\"#zhu45_b\"><sup>㊺</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊻")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu46_a\" href=\"#zhu46_b\"><sup>㊻</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊼")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu47_a\" href=\"#zhu47_b\"><sup>㊼</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊽")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu48_a\" href=\"#zhu48_b\"><sup>㊽</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊾")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu49_a\" href=\"#zhu49_b\"><sup>㊾</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }

            if ((pos = line.indexOf("㊿")) != -1) {
                result.append(line.left(pos));
                result.append("<span><a id=\"zhu50_a\" href=\"#zhu50_b\"><sup>㊿</sup></a></span>");
                len = line.length();
                line = line.right(len - pos - 1);
            }
#if 0
            if ((pos = line.indexOf("")) != -1) {
                result.append(line.left(pos));
                result.append("");
                len = line.length();
                line = line.right(len - pos - 1);
            }
#endif
        }
        result.append(line);

        if (0 == lineNum) {
            isNumZero = true;
            continue;
        }

        if (!isNumZero) {
            result.append("</p>");
        }

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
    if (0 == lineNum) {
        result.append("</p>");
        mTextHtmlEdit->setPlainText(result);
    } if (1 == lineNum) {
        mTextHtmlEdit->setPlainText(result.left(result.length()-2));
    } else if (2 == lineNum) {
        mTextHtmlEdit->setPlainText(result.left(result.length()-4));
    } else {
    }

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

void Text2HtmlDlg::onBtnAddClsClicked()
{
    QString items = ui->textHtmlEdit->toPlainText();
    addClsItem(items);
}

void Text2HtmlDlg::onClassIndexChanged(int index)
{
    mClassString.clear();
    if (0 != ui->cBoxClass->currentText().compare("none")) {
        mClassString.append(ui->cBoxClass->currentText());
    }
    qDebug() << "class index: " << index << "ctx: " << mClassString;
}
