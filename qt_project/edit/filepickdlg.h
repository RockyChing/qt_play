#ifndef FavFilePick_H
#define FavFilePick_H

#include <QDialog>

namespace Ui {
class FilePickDlg;
}

class FilePickDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FilePickDlg(QWidget *parent = 0);
    ~FilePickDlg();

private:
    void initUI();
    void initData();

    bool isCfgAvailable();

private slots:
    void onBtnDirListClicked();
    void onBtnDirSrcClicked();
    void onBtnDirDstClicked();
    void onBtnPickClicked();

private:
    Ui::FilePickDlg *ui;

    QString mDirListName;
    QString mDirSrcName;
    QString mDirDstName;
};

#endif // FavFilePick_H
