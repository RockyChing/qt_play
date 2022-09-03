#include "htmlfilerenamedlg.h"
#include "ui_htmlfilerenamedlg.h"

HtmlFileRenameDlg::HtmlFileRenameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HtmlFileRenameDlg)
{
    ui->setupUi(this);
}

HtmlFileRenameDlg::~HtmlFileRenameDlg()
{
    delete ui;
}
