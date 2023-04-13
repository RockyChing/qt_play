#ifndef KNOTEITEM_H
#define KNOTEITEM_H
#include <QString>
#include <QDateTime>
#include <QDateTime>

class KNoteItem
{
public:
    KNoteItem();

public:
    int getId();
    void setId(int id);

    QString getBookName();
    void setBookName(QString s);

    QString getContent();
    void setContent(QString s);

    QString getDateTime();
    void setDateTime(QString s);

    QString getEditContent();
    void setEditContent(QString s);

private:
    int mId;
    QString mBookName;
    QString mContent;
    QString mDateTime;
    QString mEditContent;
};

#endif // KNOTEITEM_H
