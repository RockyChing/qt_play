#include <QWidget>
#include "utils/toast.h"
#include "finance.h"
#include "ui_finance.h"
#include <QDebug>

Finance::Finance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Finance)
{
    ui->setupUi(this);

    initUI();
}

Finance::~Finance()
{
    delete ui;
}

void Finance::initUI()
{
    mInputSrc = ui->inputSrc;
    mInputTime = ui->inputTime;
    mInputDest = ui->inputDest;
    mInputRate = ui->inputRate;
    mInputRate->setFocusPolicy(Qt::NoFocus);

    mFuliSrc = ui->inputFuliSrc;
    mFuliTime = ui->inputFuliTime;
    mFuliDest = ui->inputFuliDest;
    mFuliDest->setFocusPolicy(Qt::NoFocus);
    mFuliRate = ui->inputFuliRate;

    mBtnInputCalc = ui->btnCalcRate;
    mBtnFuliCalc = ui->btnCalcFuli;

    mInputSrc->setValidator(new QDoubleValidator(mInputSrc));
    mInputTime->setValidator(new QIntValidator(mInputTime));
    mInputDest->setValidator(new QDoubleValidator(mInputDest));
    mInputRate->setValidator(new QDoubleValidator(mInputRate));

    mFuliSrc->setValidator(new QDoubleValidator(mFuliSrc));
    mFuliTime->setValidator(new QIntValidator(mFuliTime));
    mFuliDest->setValidator(new QDoubleValidator(mFuliDest));
    mFuliRate->setValidator(new QDoubleValidator(mFuliRate));

    ui->inputFirst->setValidator(new QIntValidator(ui->inputFirst));
    ui->inputIncrease->setValidator(new QIntValidator(ui->inputIncrease));
    ui->inputCnt->setValidator(new QIntValidator(ui->inputCnt));

    connect(mBtnInputCalc, SIGNAL(clicked()), this, SLOT(onInputCalClicked()));
    connect(mBtnFuliCalc, SIGNAL(clicked()), this, SLOT(onFuliCalClicked()));
    connect(ui->btnTotalMoney, SIGNAL(clicked()), this, SLOT(onTotalMoneyClicked()));
}

void Finance::onInputCalClicked()
{
    double src = mInputSrc->text().toDouble();
    double time = mInputTime->text().toDouble();
    double dst = mInputDest->text().toDouble();

    double income = dst - src;
    double incomeRate = (YEAR_DAY * income) / (src * time);

    QString incomeStr = QString::number(income, 'f', 2);
    QString incomeRateStr = QString::number(incomeRate * 100, 'f', 2);
    incomeRateStr.append('\%');

    QString text = incomeStr + " (" + incomeRateStr + ")";
    mInputRate->setText(text);
}

void Finance::onFuliCalClicked()
{
    double src = mFuliSrc->text().toDouble();
    int time = mFuliTime->text().toInt();
    double rate = mFuliRate->text().toDouble();

    double tmp = src;
    int i;

    for (i = 0; i < time; i ++) {
        tmp = tmp * (rate + 1);
    }

    QString text = QString::number(tmp, 'f', 2);
    mFuliDest->setText(text);
}

void Finance::onTotalMoneyClicked()
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
