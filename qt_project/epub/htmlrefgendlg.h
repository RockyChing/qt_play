#ifndef HTMLREFGENDLG_H
#define HTMLREFGENDLG_H

#include <QDialog>

namespace Ui {
class HtmlRefGenDlg;
}

class HtmlRefGenDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlRefGenDlg(QWidget *parent = 0);
    ~HtmlRefGenDlg();

private:
    void initUI();
    void initSlots();
    void initData();

private slots:
    void onRefStyleChanged(int index);
    void onBtnOKClicked();
    void onBtnCancleClicked();

private:
    static QString mStyleDefaultString;
    Ui::HtmlRefGenDlg *ui;

    int mIndexStyle;
    int mItemCnt;
    int mItemCurCnt;
};

#endif // HTMLREFGENDLG_H
