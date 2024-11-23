#ifndef HTMLIMGRENAMEDLG_H
#define HTMLIMGRENAMEDLG_H

#include <QDialog>
#include <QLineEdit>

namespace Ui {
class HtmlImgRenameDlg;
}

class HtmlImgRenameDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HtmlImgRenameDlg(QWidget *parent = 0);
    ~HtmlImgRenameDlg();

    void initUI();
    void initData();

private slots:
    void onBtnOpenClicked();
    void onBtnProcessClicked();

private:
    QString getImageName(QString& line);

private:
    Ui::HtmlImgRenameDlg *ui;
    QPushButton *mBtnOpen;
    QPushButton *mBtnProcess;

    QLineEdit  *mFilePath;

private:
    QString mFileAbsPath;
};

#endif // HTMLIMGRENAMEDLG_H
