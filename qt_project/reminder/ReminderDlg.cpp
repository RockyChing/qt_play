#include <QString>
#include <QSettings>
#include <QDebug>
#include <QIntValidator>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileInfoList>
#include "ReminderDlg.h"
#include "ui_reminderdlg.h"
#include "config/appsettings.h"

ReminderDlg::ReminderDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReminderDlg)
{
    ui->setupUi(this);
    initUI();
    initData();
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

    mSndFilePath.clear();
    mSndFilePath = s.value(AppSettings::R_SND_FILE).toString();
    if (!mSndFilePath.isEmpty()) {
        ui->lineEditDirShow->setText(mSndFilePath);
    } else {
        ui->lineEditDirShow->setText(tr("未指定目录"));
    }
}

void ReminderDlg::initData()
{
    connect(ui->timeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(onTimeCfgChanged(QString)));
    connect(ui->reptCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onRepeatCfgChanged(int)));
    connect(ui->btnDirOpen, SIGNAL(clicked()), this, SLOT(onBtnOpenClicked()));
}

void ReminderDlg::initTimeRepeat(int *time, int *repeat)
{
    pReminderTime = time;
    *pReminderTime = mReminderTime.toInt(); // default
    pReminderRepeat = repeat;
}

void ReminderDlg::initTimeRepeat(int *time, int *repeat, QString &sndFile)
{
    initTimeRepeat(time, repeat);
    sndFile.clear();
    sndFile.append(mSndFilePath);
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

void ReminderDlg::onBtnOpenClicked()
{
    QString filePath = ui->lineEditDirShow->text();
    if (filePath.isEmpty()) {
        /* load file from currentPath */
        filePath = QDir::currentPath();
    }

    /* getOpenFileName() returns an existing file selected by the user */
    mSndFilePath = QFileDialog::getOpenFileName(this, tr("Open file"), filePath, tr("*.mp3 *.wav"));
    mSndFilePath = QDir::toNativeSeparators(mSndFilePath);
    qDebug() << mSndFilePath;
    if (mSndFilePath.isEmpty()) {
        ui->lineEditDirShow->setText(tr("未指定目录"));
    } else {
        ui->lineEditDirShow->setText(mSndFilePath);
        QSettings s(AppSettings::APP_SETTINGS_FILE, QSettings::IniFormat);
        s.setValue(AppSettings::R_SND_FILE, mSndFilePath);
    }
}
