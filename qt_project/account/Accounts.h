#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include "AccountJsonData.h"


namespace Ui {
class Accounts;
}

class Accounts : public QWidget
{
    Q_OBJECT

public:
    explicit Accounts(QWidget *parent = 0);
    ~Accounts();

private:
    void initUI();
    void initSlot();

private slots:
    void onBtnOpenClicked();
    void onBtnSaveClicked();
    void onBtnAddClicked();

private:
    Ui::Accounts *ui;

    QTableView *mAccountsTable;
    QStandardItemModel *mTableModel;

    QLineEdit *mLineEditFile;
    QPushButton *mBtnFileOpen;
    QPushButton *mBtnSave;
    QCheckBox *mCboxEncry;

private:
    QString mFileAbsPath;
    AccountJsonData *mJsonData;
};

#endif // ACCOUNTS_H
