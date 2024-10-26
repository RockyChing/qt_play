#ifndef IMAGEWBREVERT_H
#define IMAGEWBREVERT_H

#include <QDialog>
#include <QLineEdit>
#include <QImage>

namespace Ui {
class ImageWBRevert;
}

class ImageWBRevert : public QDialog
{
    Q_OBJECT

public:
    explicit ImageWBRevert(QWidget *parent = 0);
    ~ImageWBRevert();

    void initUI();
    void initData();

private:
    bool getCropParam();

private slots:
    void onBtnOpenClicked();
    void onBtnProcessClicked();

private:
    Ui::ImageWBRevert *ui;
    QString mDirName;
};

#endif // IMAGEWBREVERT_H
