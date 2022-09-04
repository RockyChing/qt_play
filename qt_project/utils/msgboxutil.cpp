#include <QMessageBox>
#include "msgboxutil.h"

void MsgBoxUtil::information(QWidget *parent, const QString &title, const QString &text)
{
    QMessageBox::information(parent, title, text);
}

void MsgBoxUtil::information(QWidget *parent, const QString &text)
{
    QMessageBox::information(parent, "提示", text);
}

void MsgBoxUtil::warning(QWidget *parent, const QString &title, const QString &text)
{
    QMessageBox::warning(parent, title, text);
}

void MsgBoxUtil::warning(QWidget *parent, const QString &text)
{
    QMessageBox::warning(parent, "警告", text);
}
