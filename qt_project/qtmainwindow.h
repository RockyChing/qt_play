#ifndef QTMAINWINDOW_H
#define QTMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class QtMainWindow;
}

class QtMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit QtMainWindow(QWidget *parent = 0);
    ~QtMainWindow();

private:
    Ui::QtMainWindow *ui;
};

#endif // QTMAINWINDOW_H
