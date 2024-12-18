#include <QDebug>
#include "returncal.h"
#include "ui_returncal.h"

ReturnCal::ReturnCal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReturnCal)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    initUI();
}

ReturnCal::~ReturnCal()
{
    delete ui;
}

void ReturnCal::initUI()
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

    connect(mBtnInputCalc, SIGNAL(clicked()), this, SLOT(onInputCalClicked()));
    connect(mBtnFuliCalc, SIGNAL(clicked()), this, SLOT(onFuliCalClicked()));
}

void ReturnCal::onInputCalClicked()
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

void ReturnCal::onFuliCalClicked()
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
