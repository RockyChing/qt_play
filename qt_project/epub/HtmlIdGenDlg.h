#ifndef HTML_ID_GEN_DLG_H
#define HTML_ID_GEN_DLG_H

#include <QDialog>
#include <QPushButton>
#include <QPlainTextEdit>

namespace Ui {
class HtmlIdGenDlg;
}

class HtmlIdGenDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlIdGenDlg(QWidget *parent = 0);
    ~HtmlIdGenDlg();

private:
    void initUI();
    void initSlots();

    QString getRandomSerials(int length);

private slots:
    void onBtnOKClicked();
    void onBtnCancleClicked();

private:
    Ui::HtmlIdGenDlg *ui;

    QPlainTextEdit *mResText;
};

#endif // HTML_ID_GEN_DLG_H
