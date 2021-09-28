#include "PicMergeView.h"
#include "ui_picmergeview.h"

PicMergeView::PicMergeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PicMergeView)
{
    ui->setupUi(this);
    initUI();
}

PicMergeView::~PicMergeView()
{
    delete ui;
}

void PicMergeView::initUI()
{
    ui->guideText->setText(tr("安全考虑，把多张图片合并成一个文件"));
}

void PicMergeView::initSlot()
{

}
