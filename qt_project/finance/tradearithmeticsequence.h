#ifndef TRADEARITHMETICSEQUENCE_H
#define TRADEARITHMETICSEQUENCE_H

#include <QDialog>

namespace Ui {
class TradeArithmeticSequence;
}

class TradeArithmeticSequence : public QDialog
{
    Q_OBJECT

public:
    explicit TradeArithmeticSequence(QWidget *parent = 0);
    ~TradeArithmeticSequence();

private:
    void initUI();

private slots:
    void onTotalMoneyClicked();

private:
    Ui::TradeArithmeticSequence *ui;
};

#endif // TRADEARITHMETICSEQUENCE_H
