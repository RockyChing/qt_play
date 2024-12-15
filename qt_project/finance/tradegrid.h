#ifndef TRADEGRID_H
#define TRADEGRID_H

#include <QDialog>

namespace Ui {
class TradeGrid;
}

class TradeGrid : public QDialog
{
    Q_OBJECT

public:
    explicit TradeGrid(QWidget *parent = 0);
    ~TradeGrid();

    void initUI();
    void initData();

private slots:
    void onBtnGridNewClicked();
    void onBtnGridCreateClicked();

    void gridModeChanged(int state);

private:
    Ui::TradeGrid *ui;
};

#endif // TRADEGRID_H
