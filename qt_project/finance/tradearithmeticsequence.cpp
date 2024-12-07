#include <QWidget>
#include <QDebug>
#include "utils/toast.h"

#include "tradearithmeticsequence.h"
#include "ui_tradearithmeticsequence.h"

TradeArithmeticSequence::TradeArithmeticSequence(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TradeArithmeticSequence)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    initUI();
}

TradeArithmeticSequence::~TradeArithmeticSequence()
{
    delete ui;
}

void TradeArithmeticSequence::initUI()
{
    ui->inputFirst->setValidator(new QIntValidator(ui->inputFirst));
    ui->inputIncrease->setValidator(new QIntValidator(ui->inputIncrease));
    ui->inputCnt->setValidator(new QIntValidator(ui->inputCnt));

    connect(ui->btnTotalMoney, SIGNAL(clicked()), this, SLOT(onTotalMoneyClicked()));
}

void TradeArithmeticSequence::onTotalMoneyClicked()
{
    int firstMoney = ui->inputFirst->text().toInt();
    int increaseMoney = ui->inputIncrease->text().toInt();
    int cnt = ui->inputCnt->text().toInt();

    if ((firstMoney <= 0) || (increaseMoney <= 0) || (cnt <= 0)) {
        Toast::display(tr("输入数据错误！"), nullptr);
        return;
    }

    ui->textMoney->clear();
    for (int i = 0; i < cnt; i ++) {
        int need = firstMoney + increaseMoney * i;
        QString needStr = QString::number(need);
        QString iStr = QString::number(i + 1);
        QString tmp = iStr + "：" + needStr;
        ui->textMoney->append(tmp);
    }

    int totalMoney = (cnt * firstMoney) + (((cnt * (cnt - 1)) * increaseMoney) / 2);
    QString result = QString::number(totalMoney);
    QString text(tr("资金总量："));
    text.append(result);
    ui->textMoney->append(text);
}
