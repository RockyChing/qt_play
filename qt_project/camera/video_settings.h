#ifndef VIDEO_SETTINGS_H
#define VIDEO_SETTINGS_H

#include <QString>
#include <QDialog>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QAudioEncoderSettings>
#include <QVideoEncoderSettings>
#include <QMediaRecorder>

namespace Ui {
class VideoSettings;
}

class VideoSettings : public QDialog
{
    Q_OBJECT

public:
    explicit VideoSettings(QMediaRecorder *mediaRecorder, QWidget *parent = 0);
    ~VideoSettings();

    QAudioEncoderSettings audioSettings() const;
    void setAudioSettings(const QAudioEncoderSettings &settings);

    QVideoEncoderSettings videoSettings() const;
    void setVideoSettings(const QVideoEncoderSettings &settings);

    QString format() const;
    void setFormat(const QString &format);

protected:
    void changeEvent(QEvent *e);

private:
    void initComponentts();
    QVariant boxValue(const QComboBox*) const;
    void selectComboBoxItem(QComboBox *box, const QVariant &value);

private:
    Ui::VideoSettings *ui;
    QComboBox *mAudioCodecBox;
    QComboBox *mAudioSampleRateBox;
    QSlider *mAudioQualitySlider;

    QComboBox *mFmtBox;

    QComboBox *mVideoCodecBox;
    QComboBox *mVideoResoBox;
    QComboBox *mVideoFramerateBox;
    QSlider *mVideoQualitySlider;

    QDialogButtonBox *mBtnBox;

private:
    QMediaRecorder *mRecorder;
};

#endif // VIDEO_SETTINGS_H
