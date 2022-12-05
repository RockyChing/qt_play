#ifndef RANDOMSERIALSDLG_H
#define RANDOMSERIALSDLG_H

#include <QDialog>
#include <QPushButton>
#include <QPlainTextEdit>

namespace Ui {
class RandomSerialsDlg;
}

class RandomSerialsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit RandomSerialsDlg(QWidget *parent = 0);
    ~RandomSerialsDlg();

private:
    void initUI();
    void initSlots();

    QString getRandomSerials(int length);

private slots:
    void onBtnOKClicked();
    void onBtnCancleClicked();

private:
    Ui::RandomSerialsDlg *ui;

    QPlainTextEdit *mResText;
};

#endif // RANDOMSERIALSDLG_H
