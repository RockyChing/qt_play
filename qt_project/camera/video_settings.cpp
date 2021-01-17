#include "video_settings.h"
#include "ui_video_settings.h"

VideoSettings::VideoSettings(QMediaRecorder *mediaRecorder, QWidget *parent) :
    QDialog(parent), ui(new Ui::VideoSettings),
    mRecorder(mediaRecorder)
{
    ui->setupUi(this);
    initComponentts();

    //audio codecs
    mAudioCodecBox->addItem(tr("Default audio codex"), QVariant(QString()));
    foreach (const QString &codecName, mRecorder->supportedAudioCodecs()) {
        QString desc = mRecorder->audioCodecDescription(codecName);
        mAudioCodecBox->addItem(codecName + ": " + desc, QVariant(codecName));
    }

    //sample rate
    foreach (int rate, mRecorder->supportedAudioSampleRates()) {
        mAudioSampleRateBox->addItem(QString::number(rate), QVariant(rate));
    }

    mAudioQualitySlider->setRange(0, int(QMultimedia::VeryHighQuality));

    //video codec
    mVideoCodecBox->addItem(tr("Default video codex"), QVariant(QString()));
    foreach (const QString &codecName, mRecorder->supportedVideoCodecs()) {
        QString desc = mediaRecorder->videoCodecDescription(codecName);
        mVideoCodecBox->addItem(codecName + ": " + desc, QVariant(codecName));
    }

    mVideoQualitySlider->setRange(0, int(QMultimedia::VeryHighQuality));

    mVideoResoBox->addItem(tr("Default"));
    QList<QSize> supportedResos = mediaRecorder->supportedResolutions();
    foreach (const QSize &reso, supportedResos) {
        mVideoResoBox->addItem(QString("%1x%2").arg(reso.width()).arg(reso.height()),
                                        QVariant(reso));
    }

    mVideoFramerateBox->addItem(tr("Default"));
    QList<qreal> supportedFrameRates = mediaRecorder->supportedFrameRates();
    qreal rate;
    foreach (rate, supportedFrameRates) {
        QString rateString = QString("%1").arg(rate, 0, 'f', 2);
        mVideoFramerateBox->addItem(rateString, QVariant(rate));
    }

    //containers
    mFmtBox->addItem(tr("Default container"), QVariant(QString()));
    foreach (const QString &format, mediaRecorder->supportedContainers()) {
        mFmtBox->addItem(format + ":" + mediaRecorder->containerDescription(format),
                                        QVariant(format));
    }
}

VideoSettings::~VideoSettings()
{
    delete ui;
}

void VideoSettings::initComponentts()
{
    mAudioCodecBox = ui->audioCodecBox;
    mAudioSampleRateBox = ui->audioSampleRateBox;
    mAudioQualitySlider = ui->audioQualitySlider;

    mFmtBox = ui->containerFormatBox;

    mVideoCodecBox = ui->videoCodecBox;
    mVideoResoBox = ui->videoResolutionBox;
    mVideoFramerateBox = ui->videoFramerateBox;

    mVideoQualitySlider = ui->videoQualitySlider;

    mBtnBox = ui->buttonBox;
}

QAudioEncoderSettings VideoSettings::audioSettings() const
{
    QAudioEncoderSettings settings = mRecorder->audioSettings();
    settings.setCodec(boxValue(mAudioCodecBox).toString());
    settings.setQuality(QMultimedia::EncodingQuality(mAudioQualitySlider->value()));
    settings.setSampleRate(boxValue(mAudioSampleRateBox).toInt());
    return settings;
}

void VideoSettings::setAudioSettings(const QAudioEncoderSettings &settings)
{
    selectComboBoxItem(mAudioCodecBox, QVariant(settings.codec()));
    selectComboBoxItem(mAudioSampleRateBox, QVariant(settings.sampleRate()));
    mAudioQualitySlider->setValue(settings.quality());
}

QVideoEncoderSettings VideoSettings::videoSettings() const
{
    QVideoEncoderSettings settings = mRecorder->videoSettings();
    settings.setCodec(boxValue(mVideoCodecBox).toString());
    settings.setQuality(QMultimedia::EncodingQuality(mVideoQualitySlider->value()));
    settings.setResolution(boxValue(mVideoResoBox).toSize());
    settings.setFrameRate(boxValue(mVideoFramerateBox).value<qreal>());
    return settings;
}

void VideoSettings::setVideoSettings(const QVideoEncoderSettings &settings)
{
    selectComboBoxItem(mVideoCodecBox, QVariant(settings.codec()));
    selectComboBoxItem(mVideoResoBox, QVariant(settings.resolution()));
    mVideoQualitySlider->setValue(settings.quality());

    //special case for framerate
    for (int i = 0; i < mVideoFramerateBox->count(); ++i) {
        qreal item = mVideoFramerateBox->itemData(i).value<qreal>();
        if (qFuzzyCompare(item, settings.frameRate())) {
            mVideoFramerateBox->setCurrentIndex(i);
            break;
        }
    }
}


QString VideoSettings::format() const
{
    return boxValue(mFmtBox).toString();
}

void VideoSettings::setFormat(const QString &format)
{
    selectComboBoxItem(mFmtBox, QVariant(format));
}

void VideoSettings::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QVariant VideoSettings::boxValue(const QComboBox *box) const
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void VideoSettings::selectComboBoxItem(QComboBox *box, const QVariant &value)
{
    for (int i = 0; i < box->count(); ++i) {
        if (box->itemData(i) == value) {
            box->setCurrentIndex(i);
            break;
        }
    }
}

