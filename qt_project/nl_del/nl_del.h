#ifndef NL_DEL_H
#define NL_DEL_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class NewlineDelete;
}

class NewlineDelete : public QDialog
{
    Q_OBJECT

public:
    explicit NewlineDelete(QWidget *parent = 0);
    ~NewlineDelete();

    void initUI();

private slots:
    void onBtnOpenClicked();
    void onBtnProcessClicked();

private:
    Ui::NewlineDelete *ui;

    QPushButton *mBtnOpen;
    QPushButton *mBtnProcess;

    QLineEdit  *mFilePath;

private:
    QString mFileAbsPath;
};

#endif // NL_DEL_H
