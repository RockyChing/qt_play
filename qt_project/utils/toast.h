/**
 * From https://github.com/lesliefish/Qt/tree/master/UI/Toast
 */
#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include "ui_Toast.h"

class Toast : public QWidget
{
    Q_OBJECT
public:
    explicit Toast(QWidget *parent = nullptr);
    ~Toast();

private:
    void setText(const QString& text);

    // 动画方式show出，默认2秒后消失
    void setDuration(int duration = 2000);

public:
    static void display(const QString& text, QWidget* parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    Ui::ToastUi ui;
};

#endif // TOAST_H
