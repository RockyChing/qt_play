#ifndef FINANCE_H
#define FINANCE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class Finance;
}

class Finance : public QWidget
{
    Q_OBJECT

public:
    explicit Finance(QWidget *parent = 0);
    ~Finance();

private:
    void initUI();

private slots:
    void onInputCalClicked();
    void onFuliCalClicked();

public:
    enum {
        MONTH_DAY = 30,
        YEAR_DAY = 365
    };

private:
    Ui::Finance *ui;

    QLineEdit *mInputSrc;
    QLineEdit *mInputTime;
    QLineEdit *mInputDest;
    QLineEdit *mInputRate;

    QLineEdit *mFuliSrc;
    QLineEdit *mFuliTime;
    QLineEdit *mFuliDest;
    QLineEdit *mFuliRate;

    QPushButton *mBtnInputCalc;
    QPushButton *mBtnFuliCalc;
};

#endif // FINANCE_H
