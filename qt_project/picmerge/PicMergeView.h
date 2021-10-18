#ifndef PICMERGEVIEW_H
#define PICMERGEVIEW_H

#include <QWidget>
#include <QListWidget>

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
    void init();
    void initUI();
    void initSlot();

    void clearAllPicWidgetItem();
    void addPicWidgetItem(QString path);

private slots:
    void onBtnMergeClicked();
    void onBtnSplitClicked();
    void onBtnClearClicked();

private:
    Ui::PicMergeView *ui;
    QListWidget *mPicListWidget;
};

#endif // PICMERGEVIEW_H
