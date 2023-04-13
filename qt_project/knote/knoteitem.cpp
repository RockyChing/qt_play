#include "knoteitem.h"

KNoteItem::KNoteItem() :
    mId(-1)
{

}

int KNoteItem::getId()
{
    return mId;
}

void KNoteItem::setId(int id)
{
    mId = id;
}


QString KNoteItem::getBookName()
{
    return mBookName;
}

void KNoteItem::setBookName(QString s)
{
    mBookName.clear();
    mBookName.append(s);
}

QString KNoteItem::getContent()
{
    return mContent;
}

void KNoteItem::setContent(QString s)
{
    mContent.clear();
    mContent.append(s);
}


QString KNoteItem::getDateTime()
{
    return mDateTime;
}

void KNoteItem::setDateTime(QString s)
{
    mDateTime.clear();
    mDateTime.append(s);
}


QString KNoteItem::getEditContent()
{
    return mEditContent;
}

void KNoteItem::setEditContent(QString s)
{
    mEditContent.clear();
    mEditContent.append(s);
}
