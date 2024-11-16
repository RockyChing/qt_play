#include <QPropertyAnimation>
#include <QScreen>
#include <QGuiApplication>
#include <QPainter>
#include <QTimer>

#include "toast.h"

Toast::Toast(QWidget *parent) : QWidget(parent)
{
    ui.setupUi(this);

    // 无边框 无任务栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Tool);
    // 背景透明
    setAttribute(Qt::WA_TranslucentBackground, true);
}

Toast::~Toast()
{
}

void Toast::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.begin(this);
    auto kBackgroundColor = QColor(255, 255, 255);
    // 透明度为0
    kBackgroundColor.setAlpha(0.0 * 255);
    paint.setRenderHint(QPainter::Antialiasing, true);
    paint.setPen(Qt::NoPen);
    //设置画刷形式
    paint.setBrush(QBrush(kBackgroundColor, Qt::SolidPattern));
    paint.drawRect(0, 0, width(), height());
    paint.end();
}

void Toast::setText(const QString& text)
{
    ui.label->setText(text);
}

void Toast::setDuration(int duration /*= 2000*/)
{
    // 开始动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(1000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    show();

    QTimer::singleShot(duration, [&]
    {
        // 结束动画
        QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
        animation->setDuration(1000);
        animation->setStartValue(1);
        animation->setEndValue(0);
        animation->start();
        connect(animation, &QPropertyAnimation::finished, [&]
        {
            close();
            deleteLater();// 关闭后析构
        });
    });
}

void Toast::display(const QString& text, QWidget* parent)
{
    Toast* toast = new Toast(parent);
    // 置顶
    toast->setWindowFlags(toast->windowFlags() | Qt::WindowStaysOnTopHint); \
    toast->setText(text);
    //设置完文本后调整下大小
    toast->adjustSize();

    // 测试显示位于主屏的70%高度位置
    QScreen* pScreen = QGuiApplication::primaryScreen();
    toast->move((pScreen->size().width() - toast->width()) / 2, pScreen->size().height() * 7 / 10);
    toast->setDuration(2000);
}
