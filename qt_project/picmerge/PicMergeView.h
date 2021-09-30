#ifndef PICMERGEVIEW_H
#define PICMERGEVIEW_H

#include <QWidget>

namespace Ui {
class PicMergeView;
}

class PicMergeView : public QWidget
{
    Q_OBJECT

public:
    explicit PicMergeView(QWidget *parent = 0);
    ~PicMergeView();

private:
    void initUI();
    void initSlot();

private slots:
    void onBtnMergeClicked();
    void onBtnSplitClicked();
    void onBtnClearClicked();

private:
    Ui::PicMergeView *ui;
};

#endif // PICMERGEVIEW_H
