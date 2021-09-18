#ifndef ACCOUNTVIEW_H
#define ACCOUNTVIEW_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QItemSelectionModel>
#include <QModelIndex>


namespace Ui {
class AccountView;
}

class AccountView : public QWidget
{
    Q_OBJECT

public:
    explicit AccountView(QWidget *parent = 0);
    ~AccountView();

private:
    void initUI();
    void initSlot();
    void initModelFromStringList(QStringList& sList);

    void aesTest();

    void openEncrypt(const QStringList& encrypt);
    void saveEncrypt(QTextStream& outStream, const QString& plain);

private slots:
    void onBtnOpenClicked();
    void onBtnSaveClicked();
    void onBtnAddClicked();
    void onBtnDelClicked();

    void onItemChanged(QStandardItem *item);
    void onTabViewCurrentChanged(const QModelIndex &current, const QModelIndex &prev);

private:
    /* UI members */
    Ui::AccountView *ui;

private:
    /* members */
    QString mFileAbsPath;
    QStandardItemModel *mItemModel;
    QItemSelectionModel *mItemSelect;

    int mTableColumnCount;
};

#endif // ACCOUNTVIEW_H
