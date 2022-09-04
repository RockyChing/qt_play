#ifndef FILENEWDLG_H
#define FILENEWDLG_H

#include <QString>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>


namespace Ui {
class FileNewDlg;
}

class FileNewDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FileNewDlg(QWidget *parent = 0);
    ~FileNewDlg();

private:
    void initUI();
    void initData();

private slots:
    void onBtnDirOpenClicked();
    void onBtnClearClicked();
    void onBtnCreateClicked();
private:
    Ui::FileNewDlg *ui;
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

#endif // FILENEWDLG_H
