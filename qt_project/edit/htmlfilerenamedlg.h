#ifndef HTMLFILERENAMEDLG_H
#define HTMLFILERENAMEDLG_H

#include <QDialog>

namespace Ui {
class HtmlFileRenameDlg;
}

class HtmlFileRenameDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlFileRenameDlg(QWidget *parent = 0);
    ~HtmlFileRenameDlg();

private:
    Ui::HtmlFileRenameDlg *ui;
};

#endif // HTMLFILERENAMEDLG_H
