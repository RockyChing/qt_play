#include <QString>
#include "ReminderDlg.h"
#include "ui_reminderdlg.h"
#include "config/appsettings.h"

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
    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    mReminderTime = s.value(AppSettings::R_TIMEROUT).toString();
    if (mReminderTime.isEmpty() || mReminderTime.size() > 2) {
        ui->timeLineEdit->setText("45");
        mReminderTime.clear();
        mReminderTime.append("45");
    } else {
        ui->timeLineEdit->setText(mReminderTime);
    }

    ui->timeLineEdit->setValidator(new QIntValidator(0, 100, this));

    isRepeat = s.value(AppSettings::R_REPEAT, false).toBool();
    ui->reptCheckBox->setChecked(isRepeat);
}

void ReminderDlg::initSlots()
{
    connect(ui->timeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTimeCfgChanged(QString)));
    connect(ui->reptCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onRepeatCfgChanged(int)));
}

void ReminderDlg::initTimeRepeat(int *time, int *repeat)
{
    pReminderTime = time;
    *pReminderTime = mReminderTime.toInt(); // default
    pReminderRepeat = repeat;
}

void ReminderDlg::updateTimeRepeat(void)
{
    QString text = ui->timeLineEdit->text();
    *pReminderTime = text.toInt();
    *pReminderRepeat = ui->reptCheckBox->isChecked() ? 1 : 0;

    mReminderTime.clear();
    mReminderTime.append(text);
    isRepeat = ui->reptCheckBox->isChecked();

    QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
    s.setValue(AppSettings::R_TIMEROUT, mReminderTime);
    s.setValue(AppSettings::R_REPEAT, isRepeat);
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
