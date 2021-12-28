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

private:
    void initUI();
    void initSlots();

    void updateTimeRepeat();
    void updateTimeRepeat(int time, int repeat);

private slots:
    void onTimeCfgChanged(const QString &text);
    void onRepeatCfgChanged(int state);

private:
    Ui::ReminderDlg *ui;

    int *pReminderTime;
    int *pReminderRepeat;
};

#endif // REMINDERDLG_H
