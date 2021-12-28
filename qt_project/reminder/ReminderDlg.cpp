#include <QString>
#include "ReminderDlg.h"
#include "ui_reminderdlg.h"

ReminderDlg::ReminderDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReminderDlg)
{
    ui->setupUi(this);
    initUI();
    initSlots();
}

ReminderDlg::~ReminderDlg()
{
    delete ui;
}

void ReminderDlg::initUI()
{
    ui->timeLineEdit->setText("45");
    ui->timeLineEdit->setValidator(new QIntValidator(0, 100, this));
}

void ReminderDlg::initSlots()
{
    connect(ui->timeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTimeCfgChanged(QString)));
    connect(ui->reptCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onRepeatCfgChanged(int)));
}

void ReminderDlg::initTimeRepeat(int *time, int *repeat)
{
    pReminderTime = time;
    *pReminderTime = 45; // default
    pReminderRepeat = repeat;
}

void ReminderDlg::updateTimeRepeat(void)
{
    QString text = ui->timeLineEdit->text();
    *pReminderTime = text.toInt();
    *pReminderRepeat = ui->reptCheckBox->isChecked() ? 1 : 0;
}

void ReminderDlg::updateTimeRepeat(int time, int repeat)
{
    *pReminderTime = time;
    *pReminderRepeat = repeat;
}

void ReminderDlg::onTimeCfgChanged(const QString &text)
{
    updateTimeRepeat();
}

void ReminderDlg::onRepeatCfgChanged(int state)
{
    updateTimeRepeat();
}
