#ifndef MSGBOXUTIL_H
#define MSGBOXUTIL_H


class MsgBoxUtil
{
public:
    static void information(QWidget *parent, const QString &title, const QString &text);
    static void information(QWidget *parent, const QString &text);

    static void warning(QWidget *parent, const QString &title, const QString &text);
    static void warning(QWidget *parent, const QString &text);
};

#endif // MSGBOXUTIL_H
