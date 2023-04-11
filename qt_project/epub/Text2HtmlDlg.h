#ifndef TEXT2HTMLDLG_H
#define TEXT2HTMLDLG_H
#include <QPushButton>
#include <QPlainTextEdit>
#include <QDialog>

namespace Ui {
class Text2HtmlDlg;
}

class Text2HtmlDlg : public QDialog
{
    Q_OBJECT

public:
    explicit Text2HtmlDlg(QWidget *parent = 0);
    ~Text2HtmlDlg();

private:
    void initUI();
    void onBtnCopyClicked();

private slots:
    void onBtnSrcClicked();
    void onBtnTransformClicked();
    void onBtnClearClicked();


private:
    Ui::Text2HtmlDlg *ui;

    QPlainTextEdit *mTextHtmlEdit;
    QPushButton *mBtnSrc;
    QPushButton *mBtnTransform;
    QPushButton *mBtnClear;
    QPushButton *mBtnCopy;
};

#endif // TEXT2HTMLDLG_H
