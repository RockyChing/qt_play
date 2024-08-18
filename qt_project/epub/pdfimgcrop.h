#ifndef PDFIMGCROP_H
#define PDFIMGCROP_H

#include <QDialog>
#include <QLineEdit>
#include <QImage>

namespace Ui {
class PdfImgCrop;
}

class PdfImgCrop : public QDialog
{
    Q_OBJECT

public:
    explicit PdfImgCrop(QWidget *parent = 0);
    ~PdfImgCrop();

    void initUI();
    void initData();

private:
    bool getCropParam();

private slots:
    void onBtnOpenClicked();
    void onBtnProcessClicked();

private:
    Ui::PdfImgCrop *ui;
    QString mDirName;

    QPushButton *mBtnOpen;
    QPushButton *mBtnProcess;

    QRect *mCropRect;
};

#endif // PDFIMGCROP_H
