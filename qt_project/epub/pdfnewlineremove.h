#ifndef PDFNEWLINEREMOVE_H
#define PDFNEWLINEREMOVE_H
#include <QLineEdit>
#include <QDialog>

namespace Ui {
class pdfnewlineremove;
}

class pdfnewlineremove : public QDialog
{
    Q_OBJECT

public:
    explicit pdfnewlineremove(QWidget *parent = 0);
    ~pdfnewlineremove();

    void initUI();
    void initData();

private slots:
    void onBtnOpenClicked();
    void onBtnProcessClicked();
private:
    Ui::pdfnewlineremove *ui;    QPushButton *mBtnOpen;
    QPushButton *mBtnProcess;

    QLineEdit  *mFilePath;

private:
    QStringList mFileAbsPaths;
};

#endif // PDFNEWLINEREMOVE_H
