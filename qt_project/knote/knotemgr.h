#ifndef KNOTEMGR_H
#define KNOTEMGR_H

#include <QDialog>
#include <QLineEdit>
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>

#include "knoteitem.h"

namespace Ui {
class KNoteMgr;
}

class KNoteMgr : public QDialog
{
    Q_OBJECT

public:
    explicit KNoteMgr(QWidget *parent = 0);
    ~KNoteMgr();

private:
    enum SaveNoteModeFlag {
        MODE_ALL = 0x0000,
        MODE_SELECTED = 0x0001
    };

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void initUI();
    void initData();

    bool isKNoteFile(QString filePath);
    void statusSetKBookCurrent();

    int parseKNote(QString filePath);
    int showKBook(); // show book after import done
    int showKBookName(QString s);
    int saveKNote(SaveNoteModeFlag flag); // do save
    void saveKNoteAll(); // save all notes when exit if notes changed
    void saveKNoteSelect(); // save notes of a selected book

    int AddKBookNote(QString dateTime, QString content);
    void updateLabelNote();

    void delNoteOne(int index); // delete one note item
    void delNoteBook(); // delete notes of a selected book

private slots:
    void onKBookSelected(QModelIndex index);
    void onKNoteSelected(QModelIndex index);

    void onBtnImportClicked();
    void onBtnExportClicked();
    void onBtnDelBookClicked();
    void onBtnDelNoteClicked();

private:
    Ui::KNoteMgr *ui;
    volatile bool hasNoteChanged;

    int mKBookIndex;
    int mKNoteIndex;
    int mKNoteTotal;
    int mKNoteCountSelected; // notes count of a selected book
    QString mKNoteFileDir; // note file path
    QString mKNoteSaveDir; // note file save path
    QString mKBookSelected;// book that selected

    /* KNote items parse from Kindle *.txt file */
    QList<KNoteItem> mKNoteItems;
    /* KNote items of selected book */
    QList<KNoteItem> mKNoteSelected;

    /* All books */
    QList<QString> mKBooks;
    /* All notes of s selected book */
    QList<QString> mKNotes;

    QStandardItemModel *mKBookModel;
    QStandardItemModel *mKNoteModel;
};

#endif // KNOTEMGR_H
