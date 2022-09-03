#ifndef HTMLFILENEWDLG_H
#define HTMLFILENEWDLG_H

#include <QString>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>





namespace Ui {
class HtmlFileNewDlg;
}

class HtmlFileNewDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlFileNewDlg(QWidget *parent = 0);
    ~HtmlFileNewDlg();

private:
    void initUI();
    void initData();

    void msgWarning(QString text);

private slots:
    void onBtnDirOpenClicked();
    void onBtnClearClicked();
    void onBtnCreateClicked();
private:
    Ui::HtmlFileNewDlg *ui;
    QString mDirName;

    QPushButton *mBtnDirOpen;
    QPushButton *mBtnClear;
    QPushButton *mBtnCreate;

    QLineEdit *mEditDirShow;
    QLineEdit *mEditSrcFile;
    QLineEdit *mEditFilePrefix;
    QLineEdit *mEditStart;
    QLineEdit *mEditTotal;
};

#endif // HTMLFILENEWDLG_H
