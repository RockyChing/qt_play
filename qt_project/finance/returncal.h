#ifndef RETURNCAL_H
#define RETURNCAL_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

namespace Ui {
class ReturnCal;
}

class ReturnCal : public QDialog
{
    Q_OBJECT

public:
    explicit ReturnCal(QWidget *parent = 0);
    ~ReturnCal();

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
    Ui::ReturnCal *ui;

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

#endif // RETURNCAL_H
