#include "image_settings.h"
#include "ui_image_settings.h"

ImageSettings::ImageSettings(QCameraImageCapture *imageCapture, QWidget *parent) :
    QDialog(parent), ui(new Ui::ImageSettings),
    mCapture(imageCapture)
{
    ui->setupUi(this);
    initComponentts();

    mImgFmtBox->addItem(tr("Default image format"), QVariant(QString()));
    foreach (const QString &fmtName, mCapture->supportedImageCodecs()) {
        QString desc = mCapture->imageCodecDescription(fmtName);
        mImgFmtBox->addItem(fmtName + ": " + desc, QVariant(fmtName));
    }

    mImgQualitySlider->setRange(0, int(QMultimedia::VeryHighQuality));

    mImgResoBox->addItem(tr("Default Resolution"));
    QList<QSize> supportedResos = mCapture->supportedResolutions();
    foreach (const QSize &reso, supportedResos) {
        mImgResoBox->addItem(QString("%1x%2").arg(reso.width()).arg(reso.height()), QVariant(reso));
    }
}

ImageSettings::~ImageSettings()
{
    delete ui;
}

void ImageSettings::initComponentts()
{
    mImgFmtBox = ui->imageFormatBox;
    mImgResoBox = ui->imageResolutionBox;
    mImgQualitySlider = ui->imageQualitySlider;

    mBtnbox = ui->buttonBox;
}

#if 0
QAudioEncoderSettings ImageSettings::audioSettings() const
{
}

void ImageSettings::setAudioSettings(QAudioEncoderSettings &settings)
{
}
#endif

QImageEncoderSettings ImageSettings::imageSettings() const
{
    QImageEncoderSettings settings = mCapture->encodingSettings();
    settings.setCodec(boxValue(mImgFmtBox).toString());
    settings.setQuality(QMultimedia::EncodingQuality(mImgQualitySlider->value()));
    settings.setResolution(boxValue(mImgResoBox).toSize());
    return settings;
}

void ImageSettings::setImageSettings(QImageEncoderSettings &settings)
{
    selectComboBoxItem(mImgFmtBox, QVariant(settings.codec()));
    selectComboBoxItem(mImgResoBox, QVariant(settings.resolution()));
    mImgQualitySlider->setValue(settings.quality());
}


QString ImageSettings::format() const
{

}

void ImageSettings::setFormat(const QString &format)
{
}

void ImageSettings::changeEvent(QEvent *e)
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

QVariant ImageSettings::boxValue(const QComboBox *box) const
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void ImageSettings::selectComboBoxItem(QComboBox *box, const QVariant &value)
{
    for (int i = 0; i < box->count(); ++i) {
        if (box->itemData(i) == value) {
            box->setCurrentIndex(i);
            break;
        }
    }
}

