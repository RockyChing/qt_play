#ifndef REMINDERDLG_H
#define REMINDERDLG_H

#include <QDialog>

namespace Ui {
class ReminderDlg;
}

class ReminderDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ReminderDlg(QWidget *parent = 0);
    ~ReminderDlg();

    void initTimeRepeat(int *time, int *repeat);
    void initTimeRepeat(int *time, int *repeat, QString &sndFile);


private:
    void initUI();
    void initData();

    void updateTimeRepeat();
    void updateTimeRepeat(int time, int repeat);

private slots:
    void onTimeCfgChanged(const QString &text);
    void onRepeatCfgChanged(int state);
    void onBtnOpenClicked();

private:
    Ui::ReminderDlg *ui;
    QString mSndFilePath;
    QString mReminderTime;
    bool isRepeat;

    int *pReminderTime;
    int *pReminderRepeat;
};

#endif // REMINDERDLG_H
