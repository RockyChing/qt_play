#ifndef FILESYNCDLG_H
#define FILESYNCDLG_H

#include <QDialog>
#include <QDialog>
#include <QString>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>


namespace Ui {
class FileSyncDlg;
}

class FileSyncDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileSyncDlg(QWidget *parent = 0);
    ~FileSyncDlg();



private:
    void initUI();
    void initData();

private slots:
    void onBtnDirSrcClicked();


private:
    Ui::FileSyncDlg *ui;
    QString mDirSrcName;
    QString mDirDstName;

    QPushButton *mBtnDirSrc;
    QPushButton *mBtnDirDst;

    QLineEdit *mEditDirSrc;
    QLineEdit *mEditDirDst;
};

#endif // FILESYNCDLG_H
