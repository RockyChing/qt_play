#ifndef IMAGE_SETTINGS_H
#define IMAGE_SETTINGS_H

#include <QString>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QCameraImageCapture>
#include <QAudioEncoderSettings>
#include <QImageEncoderSettings>

namespace Ui {
class ImageSettings;
}

class ImageSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ImageSettings(QCameraImageCapture *imageCapture, QWidget *parent = 0);
    ~ImageSettings();

    //QAudioEncoderSettings audioSettings() const;
    //void setAudioSettings(QAudioEncoderSettings &settings);

    QImageEncoderSettings imageSettings() const;
    void setImageSettings(QImageEncoderSettings &settings);

    QString format() const;
    void setFormat(const QString &format);

protected:
    void changeEvent(QEvent *e);

private:
    void initComponentts();
    QVariant boxValue(const QComboBox *box) const;
    void selectComboBoxItem(QComboBox *box, const QVariant &value);

private:
    Ui::ImageSettings *ui;
    QComboBox *mImgFmtBox;
    QComboBox *mImgResoBox;
    QSlider *mImgQualitySlider;

    QDialogButtonBox *mBtnbox;

private:
    QCameraImageCapture *mCapture;
};

#endif // IMAGE_SETTINGS_H
