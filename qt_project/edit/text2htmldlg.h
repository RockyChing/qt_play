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

private slots:
    void onBtnTransformClicked();
    void onBtnCopyClicked();

private:
    Ui::Text2HtmlDlg *ui;

    QPlainTextEdit *mTextHtmlEdit;
    QPushButton *mBtnTransform;
    QPushButton *mBtnCopy;
};

#endif // TEXT2HTMLDLG_H
