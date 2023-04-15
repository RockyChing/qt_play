#ifndef FILERENAMEDLG_H
#define FILERENAMEDLG_H

#include <QDialog>
#include <QString>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>

namespace Ui {
class FileRenameDlg;
}

class FileRenameDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileRenameDlg(QWidget *parent = 0);
    ~FileRenameDlg();

private:
    void initUI();
    void initData();

    bool isStringUpper(QString &s);

private slots:
    void onSeqTextChanged();
    void onBtnDirOpenClicked();
    void onBtnRenameClicked();

private:
    Ui::FileRenameDlg *ui;
    QString mDirName;

    QPushButton *mBtnDirOpen;
    QPushButton *mBtnRename;

    QLineEdit *mEditDirShow;
    QLineEdit *mEditFilePrefix;

    QCheckBox *mCBBackup;
};

#endif // FILERENAMEDLG_H
