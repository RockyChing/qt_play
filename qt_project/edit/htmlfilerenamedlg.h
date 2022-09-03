#ifndef HTMLFILERENAMEDLG_H
#define HTMLFILERENAMEDLG_H

#include <QDialog>
#include <QString>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>

namespace Ui {
class HtmlFileRenameDlg;
}

class HtmlFileRenameDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlFileRenameDlg(QWidget *parent = 0);
    ~HtmlFileRenameDlg();

private:
    void initUI();
    void initData();

    void msgWarning(QString text);

private slots:
    void onBtnDirOpenClicked();
    void onBtnRenameClicked();

private:
    Ui::HtmlFileRenameDlg *ui;
    QString mDirName;

    QPushButton *mBtnDirOpen;
    QPushButton *mBtnRename;

    QLineEdit *mEditDirShow;
    QLineEdit *mEditFilePrefix;

    QCheckBox *mCBBackup;
};

#endif // HTMLFILERENAMEDLG_H
