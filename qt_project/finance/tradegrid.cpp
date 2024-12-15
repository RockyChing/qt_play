#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QMessageBox>
#include <QVector>

#include <QDebug>

#include "config/appsettings.h"
#include "utils/toast.h"

#include "tradegrid.h"
#include "ui_tradegrid.h"

TradeGrid::TradeGrid(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TradeGrid)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    Qt::WindowFlags windowFlag = Qt::Dialog;
    windowFlag |= Qt::WindowMinimizeButtonHint;
    windowFlag |= Qt::WindowMaximizeButtonHint;
    windowFlag |= Qt::WindowCloseButtonHint;    

    setWindowFlags(windowFlag);
    initUI();
    initData();
}

TradeGrid::~TradeGrid()
{
    delete ui;
}

void TradeGrid::initUI()
{
    ui->lineEditNewPrice->setValidator(new QIntValidator(ui->lineEditNewPrice));
    ui->lineEditNewHigh->setValidator(new QIntValidator(ui->lineEditNewHigh));
    ui->lineEditNewLow->setValidator(new QIntValidator(ui->lineEditNewLow));

    ui->lineEditGridHigh->setValidator(new QDoubleValidator(ui->lineEditGridHigh));
    ui->lineEditGridLow->setValidator(new QDoubleValidator(ui->lineEditGridLow));
    ui->lineEditGrid->setValidator(new QDoubleValidator(ui->lineEditGrid));
    ui->lineEditGridMoney->setValidator(new QDoubleValidator(ui->lineEditGridMoney));
    ui->lineEditGridPrice->setValidator(new QDoubleValidator(ui->lineEditGridPrice));

    connect(ui->btnGirdNew, SIGNAL(clicked()), this, SLOT(onBtnGridNewClicked()));
    connect(ui->btnGirdCreate, SIGNAL(clicked()), this, SLOT(onBtnGridCreateClicked()));
    connect(ui->checkBoxPcnt, SIGNAL(stateChanged(int)), this, SLOT(gridModeChanged(int)));
}

void TradeGrid::initData()
{

}

void TradeGrid::gridModeChanged(int state)
{
    if (ui->checkBoxPcnt->isChecked()) {
        ui->lineEditGrid->setPlaceholderText("0.01");
    } else {
        ui->lineEditGrid->setPlaceholderText("格子");
    }
}


void TradeGrid::onBtnGridNewClicked()
{
    if (true) {
        Toast::display("暂不支持！");
        return;
    }

    QString tgtName = ui->lineEditNewName->text();
    if (tgtName.isEmpty()) {
        Toast::display("请输入标的名称！");
        return;
    }

    QString tgtPrice = ui->lineEditNewPrice->text();
    if (tgtPrice.isEmpty()) {
        Toast::display("请输入标的价格！");
        return;
    }

    QString gridHigh =  ui->lineEditNewHigh->text();
    if (gridHigh.isEmpty()) {
        Toast::display("请输入标的上浮百分数！");
        return;
    }

    QString gridlow = ui->lineEditNewLow->text();
    if (gridlow.isEmpty()) {
        Toast::display("请输入标的下跌百分数！");
        return;
    }

    QString saveTgtFileName = QFileDialog::getSaveFileName(this, tr("Open file"), "./");
    qDebug() << "saveTgtFileName: " << saveTgtFileName;

    if (saveTgtFileName.isEmpty()) {
        Toast::display("请设置保存目录！");
        return;
    }
}

void TradeGrid::onBtnGridCreateClicked()
{
    const double priceHigh = ui->lineEditGridHigh->text().toDouble();
    const double priceLow = ui->lineEditGridLow->text().toDouble();
    const double priceInit = ui->lineEditGridPrice->text().toDouble();
    const double priceGrid = ui->lineEditGrid->text().toDouble();
    const double priceGridMoney = ui->lineEditGridMoney->text().toDouble();

    int gridCnt;

    if ((priceHigh <= priceLow) || (priceInit <= priceLow) ||
            (priceInit >= priceHigh)) {
        Toast::display("参数错误！");
        return;
    }

    if (ui->checkBoxPcnt->isChecked()) {
        if (priceGrid >= 1) {
            Toast::display("网格参数错误！");
            return;
        }
    }

    ui->infoText->clear();

    QVector<double> priceHighCtx;
    double priceCur = priceInit;
    gridCnt = 0;
    while (priceCur <= priceHigh) {
        priceHighCtx.append(priceCur);

        if (ui->checkBoxPcnt->isChecked()) {
            priceCur = priceCur + (priceCur * priceGrid);
        } else {
            priceCur = priceCur + priceGrid;
        }

        gridCnt += 1;
        if (gridCnt > 1000) {
            Toast::display("格子数超限（1000）！");
            return;
        }
    }

    for (int i = priceHighCtx.size() - 1; i >= 0; i --) {
        double tmpV = priceHighCtx.at(i);
        QString tmp = QString::number(tmpV, 'f', 0);

        double gridMoney = priceGridMoney * i;
        QString gridMoneyStr = QString::number(gridMoney, 'f', 0);

        QString result = tmp + '\t' + gridMoneyStr;
        ui->infoText->appendPlainText(result);
    }

    ui->infoText->appendPlainText("==============");

    QVector<double> priceLowCtx;
    priceCur = priceInit;
    gridCnt = 0;
    while (priceCur >= priceLow) {
        priceLowCtx.append(priceCur);

        if (ui->checkBoxPcnt->isChecked()) {
            priceCur = priceCur - (priceCur * priceGrid);
        } else {
            priceCur = priceCur - priceGrid;
        }

        gridCnt += 1;
        if (gridCnt > 1000) {
            Toast::display("格子数超限（1000）");
            return;
        }
    }

    for (int i = 0; i < priceLowCtx.size(); i ++) {
        double tmpV = priceLowCtx.at(i);
        QString tmp = QString::number(tmpV, 'f', 0);

        double gridMoney = priceGridMoney * i;
        QString gridMoneyStr = QString::number(gridMoney, 'f', 0);

        QString result = tmp + '\t' + gridMoneyStr;
        ui->infoText->appendPlainText(result);
    }
}
